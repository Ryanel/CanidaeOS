#include <stdint.h>

#include "awd/console.h"
#include "awd/kernel.h"

extern uint64_t paging_pml4[512];         // PML4T
extern uint64_t paging_kernel_p3[512];    // PDPT
extern uint64_t paging_kernel_p2[512];    // PDT
extern uint64_t paging_identity_p2[512];  // PDT
extern void paging_enable();

void map_kernel(loaded_kernel_info_t kernel_info) {
    uint32_t index_p4, index_p3, index_p2, index_p1;
    uint64_t address = kernel_info.virt_start;
    const uint8_t present_writable = 0b11;

    index_p1 = (address >> 12) & 0b111111111;
    index_p2 = (address >> 21) & 0b111111111;
    index_p3 = (address >> 30) & 0b111111111;
    index_p4 = (address >> 39) & 0b111111111;

    console_log("paging", "P4: %3d\n", index_p4);
    console_log("paging", "P3: %3d\n", index_p3);
    console_log("paging", "P2: %3d\n", index_p2);
    console_log("paging", "P1: %3d\n", index_p1);
    console_log("paging", "PML4[%03d] = 0x%08X (P3)\n", index_p4, ((uintptr_t)&paging_kernel_p3) | present_writable);
    console_log("paging", "PDPT[%03d] = 0x%08X (P2)\n", index_p3, ((uintptr_t)&paging_kernel_p2) | present_writable);

    paging_pml4[index_p4] = ((uintptr_t)&paging_kernel_p3) | present_writable;       // Map PDPT -> PML4T
    paging_kernel_p3[index_p3] = ((uintptr_t)&paging_kernel_p2) | present_writable;  // Map PDT -> PDPT

    // Identity map the first gigabyte
    for (unsigned int i = 0; i < 512; i++) {
        // 2 MiB
        uint64_t entry_address = 0x200000 * i;
        uint64_t attribute = 0b10000011;
        uint64_t entry = entry_address | attribute;

        paging_kernel_p2[i] = entry;
        // console_log("page", "p2[%03d] -> 2MB Page (Write Execute)\n", i);
    }

    paging_enable();
}
