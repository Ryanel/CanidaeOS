#include "kernel/vmm.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <awd/awd_info.h>

#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/panic.h"
#include "kernel/pmm.h"

using namespace kernel;

static vmm kernel_vmm;

vmm& vmm::get() { return kernel_vmm; }

void vmm::init() {
    if (m_init) { return; }
    m_init = true;
    init_kernel_page_directory();
}

void vmm::init_kernel_page_directory() {
    pdir_kernel = (page_table_t*)kmalloc_aligned(0x1000, 0x1000);
    memset(pdir_kernel, 0, PAGE_SIZE);

    const uint8_t flags = (VMM_PAGE_PRESENT | VMM_PAGE_WRITE);

    map_pages(pdir_kernel, 0xffffffff80000000, MEM_VIRT_TO_PHYS(0xffffffff80000000), 0x200000, flags);  // 0MB -> 2MB
    map_pages(pdir_kernel, 0xffffffff80200000, MEM_VIRT_TO_PHYS(0xffffffff80200000), 0x200000, flags);  // 2MB -> 4MB

    pdir_current = pdir_kernel;
    swap_page_directory(pdir_kernel);

    VMM_DEBUG_LOG("Switched to Kernel Page Table 0x%016p", pdir_kernel);
}

// Attempts to map length / 0x1000 physical pages to address in the current address space
void* vmm::map(void* desired_addr, size_t length, int perm, int flags) {
    logical_addr_t  l_addr = 0;
    physical_addr_t p_addr = 0;
    pmm&            k_pmm  = pmm::get();

    // Set flags
    bool ram_backed       = (flags & VMM_MAP_UNBACKED) == 0;
    bool region_kernel    = (flags & VMM_MAP_REGION_KERNEL) != 0;
    //bool l_addr_fixed     = (flags & VMM_MAP_FIXED) != 0;
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
