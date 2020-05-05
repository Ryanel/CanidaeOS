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

using namespace Kernel;

#define VMM_PAGE_CREATE_ENTRY(x, flags) (uint64_t)((uint64_t)x | flags);
// Convert an entry back into a physical address. Strip off the last 8 bits that are flags.
#define VMM_PAGE_ENTRY_ADDR(x) (x & (~0xFF))

#ifdef DEBUG
#define VMM_DEBUG_LOG(fmt, ...) KernelLog::Get().Log("vmm", fmt, __VA_ARGS__)
#else
#define VMM_DEBUG_LOG(fmt, ...)
#endif

static VMM kernel_vmm;

VMM& VMM::Get() { return kernel_vmm; }

void VMM::Init(awd_info_t* awd_info) {
    if (hasBeenInitialised) { return; }
    hasBeenInitialised = true;
    SetupKernelPageTable(awd_info);
}

void VMM::SetupKernelPageTable(awd_info_t* awd_info) {
    kernelP4 = (page_table_t*)kmalloc_aligned(0x1000, 0x1000);
    memset(kernelP4, 0, PAGE_SIZE);

    const uint8_t flags = (VMM_PAGE_PRESENT | VMM_PAGE_WRITE);

    MapPages(kernelP4, 0xfffffffff8000000, MEM_VIRT_TO_PHYS(0xfffffffff8000000), 0x200000, flags);  // 0MB -> 2MB
    MapPages(kernelP4, 0xfffffffff8200000, MEM_VIRT_TO_PHYS(0xfffffffff8200000), 0x200000, flags);  // 2MB -> 4MB

    physical_addr_t addressOfP4 = MEM_VIRT_TO_PHYS((physical_addr_t)kernelP4);

    asm volatile("movq %0, %%cr3;" ::"r"(addressOfP4));

    VMM_DEBUG_LOG("Switched to Kernel Page Table 0x%016p", kernelP4);
}

page_entry_t* VMM::FindEntry(page_table_t* p4, logical_addr_t addr, bool create) {
    page_table_t* p3 = nullptr;
    page_table_t* p2 = nullptr;
    page_table_t* p1 = nullptr;

    // Step 1: Determine offsets into tables...
    uintptr_t index_p4 = (addr >> 39) & 511;  // PML4 index
    uintptr_t index_p3 = (addr >> 30) & 511;  // PDPT index
    uintptr_t index_p2 = (addr >> 21) & 511;  // Page Directory index
    uintptr_t index_p1 = (addr >> 12) & 511;  // Page Table index

    // Step 2: Locate P3. If it does not exist, create it or leave
    virtual_addr_t p3_addr = GrabTable((page_table_t*)p4, index_p4, create);
    if (p3_addr == 0) { return nullptr; }
    p3 = (page_table_t*)(p3_addr);

    // Step 3: Locate page directory. If it does not exist, create it or leave
    virtual_addr_t p2_addr = GrabTable((page_table_t*)p3, index_p3, create);
    if (p2_addr == 0) { return nullptr; }
    p2 = (page_table_t*)(p2_addr);

    // Step 4: Locate page table. If it does not exist, create it or leave
    virtual_addr_t p1_addr = GrabTable((page_table_t*)p2, index_p2, create);
    if (p1_addr == 0) { return nullptr; }
    p1 = (page_table_t*)(p1_addr);

    // Step 5: Locate the entry in the page table
    uint64_t ent_address = (uint64_t)(&(p1->entries[index_p1]));
    auto     res         = (page_entry_t*)ent_address;

    // VMM_DEBUG_LOG("walk 4[%03d]-> 3[%03d]-> 2[%03d]-> 1[%03d] @ 0x%016p", index_p4, index_p3, index_p2, index_p1,
    // res);

    return res;
}

virtual_addr_t VMM::GrabTable(page_table_t* pt, uintptr_t index, bool create) {
    page_table_t* table = nullptr;
    if (pt->entries[index] == 0) {  // Does not exist, create it
        if (!create) return 0;

        table = (page_table_t*)kmalloc_aligned(PAGE_SIZE, 0x1000);
        memset(table, 0, PAGE_SIZE);

        physical_addr_t table_phys = MEM_VIRT_TO_PHYS((uint64_t)table);

        pt->entries[index] = VMM_PAGE_CREATE_ENTRY(table_phys, (VMM_PAGE_PRESENT | VMM_PAGE_WRITE));
    } else {
        table = (page_table_t*)MEM_PHYS_TO_VIRT(VMM_PAGE_ENTRY_ADDR(pt->entries[index]));
    }

    return (virtual_addr_t)table;
}

void VMM::MapPages(page_table_t* p4, logical_addr_t addr, physical_addr_t p_addr, size_t size, uint8_t flags) {
    uint64_t addr_limit = addr + size;

    // vaddr -> (vaddr + size)
    for (; addr < addr_limit; addr += PAGE_SIZE) {
        // VMM_DEBUG_LOG("map 0x%016p -> 0x%016p", addr, p_addr);

        // Find the entry corresponding to the logical address addr
        page_entry_t* entry = FindEntry(p4, addr, true);
        if (entry == nullptr) { panic("vmm: Unable to map page, unable to find entry"); }

        // Check if the entry is present
        if (entry->data & VMM_PAGE_PRESENT) {
            KernelLog::Get().Log("vmm", "Attempting to map 0x%016p, which is already mapped.", addr);
            panic("vmm: Attempted to remap a page!");
        }

        // Map the (now) virtual address addr to p_addr. Mark the pages with the provided flags
        entry->data = p_addr | flags;

        p_addr += PAGE_SIZE;
    }
}

// Attempts to map length / 0x1000 physical pages to address.
void* VMM::Map(void* address, size_t length, int perm, int flags) {
    if (address == NULL) {
        uint64_t addr = PMM::Get().Allocate((length / 0x1000));
        address = (void*)addr;
    }
    // Check to see if address is not clobbered.
    // Address is now a valid page-aligned address, and some RAM will be allocated.

    return address;
}
void* VMM::Unmap(void* address, size_t length) { return address; }