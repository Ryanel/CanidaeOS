#include "kernel/vmm.h"
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
#define VMM_DEBUG_LOG(fmt, ...) KernelLog::Get().Log("vmm", fmt, __VA_ARGS__)
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
    kernel_pdir = (page_table_t*)kmalloc_aligned(0x1000, 0x1000);
    memset(kernel_pdir, 0, PAGE_SIZE);

    const uint8_t flags = (VMM_PAGE_PRESENT | VMM_PAGE_WRITE);

    map_pages(kernel_pdir, 0xfffffffff8000000, MEM_VIRT_TO_PHYS(0xfffffffff8000000), 0x200000, flags);  // 0MB -> 2MB
    map_pages(kernel_pdir, 0xfffffffff8200000, MEM_VIRT_TO_PHYS(0xfffffffff8200000), 0x200000, flags);  // 2MB -> 4MB

    swap_page_directory(kernel_pdir);

    VMM_DEBUG_LOG("Switched to Kernel Page Table 0x%016p", kernel_pdir);
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
        physical_addr_t table_phys = MEM_VIRT_TO_PHYS((uint64_t)table);
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
        // Find the entry corresponding to the logical address addr
        page_entry_t* entry = find_page_entry(p4, addr, true);
        if (entry == nullptr) { panic("vmm: Unable to map page, unable to find entry"); }

        // Check if the entry is present
        if (entry->data & VMM_PAGE_PRESENT) {
            // TODO: Log address it's mapped to and display PDE, as well as physical address.
            KernelLog::Get().Log("vmm", "Attempting to map 0x%016p, which is already mapped.", addr);
            panic("vmm: Attempted to remap a page!");
        }

        // Map the (now) virtual address addr to p_addr. Mark the pages with the provided flags
        entry->data = p_addr | flags;
        p_addr += PAGE_SIZE;
    }
}

// Attempts to map length / 0x1000 physical pages to address in the current address space
void* vmm::map(void* address, size_t length, int perm, int flags) {
    physical_addr_t pAddr = pmm::get().allocate((length / 0x1000));

    if (address == NULL) {
        // Choose an address
        logical_addr_t address_choose_base = 0;
        if ((flags & VMM_MAP_KERNEL) != 0) {
            address_choose_base = MEM_PHYS_TO_VIRT(0) + pAddr;
        } else {
            // TODO: Unimplemented
            address_choose_base = 0;
        }
        address = (void*)(address_choose_base);  // TODO: Implement correctly
    }
    // Check to see if address is not clobbered.
    // Address is now a valid page-aligned address, and some RAM will be allocated.
    map_pages(kernel_pdir, (logical_addr_t)address, (physical_addr_t)pAddr, length, perm);
    swap_page_directory(kernel_pdir);  // Flush TLB
    return address;
}

void* vmm::unmap(void* address, size_t length) { return address; }

void vmm::swap_page_directory(page_table_t* newMemorySpace) {
    physical_addr_t phys_addr = MEM_VIRT_TO_PHYS(newMemorySpace);
    asm volatile("movq %0, %%cr3;" ::"r"(phys_addr));
}

void vmm::invalidate() {
    // TODO: Implement
    //asm volatile("movq %%cr3, %%cr3;");
}

void vmm::invalidate(uint64_t addr) { asm volatile("invlpg (%0)" ::"r"(addr) : "memory"); }

void vmm::invalidate(uint64_t start_addr, uint64_t end_addr) {
    while (start_addr < end_addr) {
        invalidate(start_addr);
        start_addr += 0x1000;
    }
}

