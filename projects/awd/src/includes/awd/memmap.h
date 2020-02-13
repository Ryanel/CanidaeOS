#ifndef AWD_MEMMAP_H
#define AWD_MEMMAP_H

#include <stdint.h>

#define AWD_MEMMAP_FREE 0
#define AWD_MEMMAP_USED 1

typedef struct AWDPhysmemmap {
    char                  type;
    uint64_t              startAddress;
    uint64_t              size;
    struct AWDPhysmemmap* next;
} awd_physmemmap_t;

void              memmap_phys_add(awd_physmemmap_t* x);
awd_physmemmap_t* memmap_phys_create(char type, uint64_t startAddress, uint64_t size);
awd_physmemmap_t* memmap_phys_get_root();
#endif