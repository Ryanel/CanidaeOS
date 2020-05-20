#include "kernel/vmm.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "awd/awd_info.h"

#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/panic.h"
#include "kernel/pmm.h"

using namespace kernel;

#define VMM_PAGE_CREATE_ENTRY(x, flags) (uint64_t)((uint64_t)x | flags);
// Convert an entry back into a physical address. Strip off the last 8 bits that are flags.
#define VMM_PAGE_ENTRY_ADDR(x) (x & (~0xFF))

#ifdef DEBUG
#define VMM_DEBUG_LOG(fmt, ...) log::Get().Log("vmm", fmt, __VA_ARGS__)
#else
#define VMM_DEBUG_LOG(fmt, ...)
#endif

static vmm kernel_vmm;

vmm& vmm::get() { return kernel_vmm; }

void vmm::init(awd_info_t* awd_info) {
    if (m_init) { return; }
    m_init = true;
    init_kernel_page_directory(awd_info);
}

void vmm::init_kernel_page_directory(awd_info_t* awd_info) {
    pdir_kernel = (page_table_t*)kmalloc_aligned(0x1000, 0x1000);
    memset(pdir_kernel, 0, PAGE_SIZE);

    const uint8_t flags = (VMM_PAGE_PRESENT | VMM_PAGE_WRITE);

    map_pages(pdir_kernel, 0xffffffff80000000, MEM_VIRT_TO_PHYS(0xffffffff80000000), 0x200000, flags);  // 0MB -> 2MB
    map_pages(pdir_kernel, 0xffffffff80200000, MEM_VIRT_TO_PHYS(0xffffffff80200000), 0x200000, flags);  // 2MB -> 4MB

    pdir_current = pdir_kernel;
    swap_page_directory(pdir_kernel);

    VMM_DEBUG_LOG("Switched to Kernel Page Table 0x%016p", pdir_kernel);
}

page_entry_t* vmm::find_page_entry(page_table_t* p4, logical_addr_t addr, bool create) {
    page_table_t* p3 = nullptr;
    page_table_t* p2 = nullptr;
    page_table_t* p1 = nullptr;

    // Step 1: Determine offsets into tables...
    uintptr_t index_p4 = (addr >> 39) & 511;  // PML4 index
    uintptr_t index_p3 = (addr >> 30) & 511;  // PDPT index
    uintptr_t index_p2 = (addr >> 21) & 511;  // Page Directory index
    uintptr_t index_p1 = (addr >> 12) & 511;  // Page Table index

    // Step 2: Locate P3. If it does not exist, create it or leave
    virtual_addr_t p3_addr = find_page_table((page_table_t*)p4, index_p4, create);
    if (p3_addr == 0) { return nullptr; }
    p3 = (page_table_t*)(p3_addr);

    // Step 3: Locate page directory. If it does not exist, create it or leave
    virtual_addr_t p2_addr = find_page_table((page_table_t*)p3, index_p3, create);
    if (p2_addr == 0) { return nullptr; }
    p2 = (page_table_t*)(p2_addr);

    // Step 4: Locate page table. If it does not exist, create it or leave
    virtual_addr_t p1_addr = find_page_table((page_table_t*)p2, index_p2, create);
    if (p1_addr == 0) { return nullptr; }
    p1 = (page_table_t*)(p1_addr);

    // Step 5: Locate the entry in the page table
    uint64_t ent_address = (uint64_t)(&(p1->entries[index_p1]));
    auto     res         = (page_entry_t*)ent_address;

    // VMM_DEBUG_LOG("walk 4[%03d]-> 3[%03d]-> 2[%03d]-> 1[%03d] @ 0x%016p", index_p4, index_p3, index_p2, index_p1,
    // res);

    return res;
}

virtual_addr_t vmm::find_page_table(page_table_t* pt, uintptr_t index, bool create) {
    page_table_t* table = nullptr;
    // Does the table exist?
    if (pt->entries[index] == 0) {                                  // No, it does not exist.
        if (!create) return 0;                                      // Create it?
        table = (page_table_t*)kmalloc_aligned(PAGE_SIZE, 0x1000);  // Create it.
        memset(table, 0, PAGE_SIZE);
        // Write to existing page table.
        physical_addr_t table_phys = MEM_VIRT_TO_PHYS(table);
        pt->entries[index]         = VMM_PAGE_CREATE_ENTRY(table_phys, (VMM_PAGE_PRESENT | VMM_PAGE_WRITE));
    } else {  // Yes, it exists.
        table = (page_table_t*)MEM_PHYS_TO_VIRT(VMM_PAGE_ENTRY_ADDR(pt->entries[index]));
    }
    return (virtual_addr_t)table;
}

void vmm::map_pages(page_table_t* p4, logical_addr_t addr, physical_addr_t p_addr, size_t size, uint8_t flags) {
    uint64_t addr_limit = addr + size;

    // for vaddr -> (vaddr + size)
    for (; addr < addr_limit; addr += PAGE_SIZE) {
        // log::Get().Log("vmm", "Map 0x%016p -> 0x%016p", addr, p_addr);
        // Find the entry corresponding to the logical address addr
        page_entry_t* entry = find_page_entry(p4, addr, true);
        if (entry == nullptr) { panic("vmm: Unable to map page, unable to find entry"); }

        // Check if the entry is present
        if (entry->data & VMM_PAGE_PRESENT) {
            // TODO: Log address it's mapped to and display PDE, as well as physical address.
            log::Get().Log("vmm", "Attempting to map 0x%016p, which is already mapped.", addr);
            panic("vmm: Attempted to remap a page!");
        }

        // Map the (now) virtual address addr to p_addr. Mark the pages with the provided flags
        entry->data = VMM_PAGE_CREATE_ENTRY(p_addr, flags);
        p_addr += PAGE_SIZE;
    }
}

void vmm::unmap_pages(page_table_t* p4, logical_addr_t addr, size_t size) {
    uint64_t addr_limit = addr + size;

    // for vaddr -> (vaddr + size)
    for (; addr < addr_limit; addr += PAGE_SIZE) {
        // log::Get().Log("vmm", "Unmap 0x%016p", addr);
        // Find the entry corresponding to the logical address addr
        page_entry_t* entry = find_page_entry(p4, addr, true);
        if (entry == nullptr) { panic("vmm: Unable to map page, unable to find entry"); }
        // Map the (now) virtual address addr to p_addr. Mark the pages with the provided flags
        entry->data = VMM_PAGE_WRITE;
    }
}

// Attempts to map length / 0x1000 physical pages to address in the current address space
void* vmm::map(void* desired_addr, size_t length, int perm, int flags) {
    logical_addr_t  l_addr = 0;
    physical_addr_t p_addr = 0;
    pmm&            k_pmm  = pmm::get();

    // Set flags
    bool ram_backed       = (flags & VMM_MAP_UNBACKED) == 0;
    bool region_kernel    = (flags & VMM_MAP_REGION_KERNEL) != 0;
    bool l_addr_fixed     = (flags & VMM_MAP_FIXED) != 0;
    bool desired_is_paddr = (flags & VMM_MAP_PHYS) != 0;

    // Determine physical address & logical address.
    if (ram_backed) {
        if (desired_addr == 0) {
            p_addr = k_pmm.allocate((length / 0x1000));

            if (region_kernel) {
                l_addr = MEM_PHYS_TO_VIRT(p_addr);
            } else {
                panic("vmm: Unimplemented, no free address finding");
                // TODO: Implement free address finding!
            }

        } else {
            if (region_kernel) {
                l_addr = (logical_addr_t)desired_addr;
                p_addr = (physical_addr_t)MEM_VIRT_TO_PHYS(l_addr);
                assert(l_addr > (logical_addr_t)&KERNEL_VMB);
            } else {
                panic("vmm: Unimplemented, no free address finding");
                // TODO: Implement free address finding!
            }
        }
    } else {
        if (desired_is_paddr) {
            p_addr = (physical_addr_t)desired_addr;
            if (region_kernel) {
                l_addr = MEM_PHYS_TO_VIRT(p_addr);
            } else {
                // TODO: FIX, do not identity map.
                l_addr = p_addr;
                // panic("vmm: Unimplemented, no free vmm address finding");
            }
        } else {
            panic("vmm: Unimplemented, no unmapped memory support");
        }
    }

    // TODO: Memory region accounting here
    map_pages(pdir_current, l_addr, p_addr, length, perm);
    swap_page_directory(pdir_current);  // Flush TLB
    return (void*)l_addr;
}

void* vmm::map_direct(void* v_addr, void* p_addr, size_t length, int perm, int flags) {
    // TODO: Memory region accounting here
    // VMM_DEBUG_LOG("Map 0x%016p -> 0x%016p (%x)", p_addr, v_addr, length);
    map_pages(pdir_current, (uint64_t)v_addr, (uint64_t)p_addr, length, perm);
    swap_page_directory(pdir_current);  // Flush TLB
    return v_addr;
}

int vmm::unmap(void* address, size_t length) {
    unmap_pages(pdir_current, (logical_addr_t)address, length);
    swap_page_directory(pdir_current);  // Flush TLB
    return 0;
}

void vmm::swap_page_directory(page_table_t* newMemorySpace) {
    physical_addr_t phys_addr = MEM_VIRT_TO_PHYS(newMemorySpace);
    asm("cli");  // TODO: Replace with real lock
    pdir_current = newMemorySpace;
    asm volatile("movq %0, %%cr3;" ::"r"(phys_addr));
    asm("sti");  // TODO: Replace with real unlock
}

void vmm::invalidate() { swap_page_directory(pdir_current); }

void vmm::invalidate(uint64_t addr) { asm volatile("invlpg (%0)" ::"r"(addr) : "memory"); }

void vmm::invalidate(uint64_t start_addr, uint64_t end_addr) {
    while (start_addr < end_addr) {
        invalidate(start_addr);
        start_addr += 0x1000;
    }
}
