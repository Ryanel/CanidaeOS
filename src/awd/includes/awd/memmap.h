#ifndef AWD_MEMMAP_H
#define AWD_MEMMAP_H

#include <stdint.h>

#define AWD_MEMMAP_FREE 0
#define AWD_MEMMAP_USED 1

/// AWD Memory Map
typedef struct AWDPhysmemmap {
    uint32_t next;
    char     type;
    uint32_t startAddress_hi;
    uint32_t startAddress_lo;
    uint32_t size_hi;
    uint32_t size_lo;
} awd_physmemmap_t;

void              memmap_phys_add(awd_physmemmap_t* x);
awd_physmemmap_t* memmap_phys_create(char type, uint32_t startAddress_lo, uint32_t startAddress_hi, uint32_t size_lo, uint32_t size_hi) ;
awd_physmemmap_t* memmap_phys_get_root();
#endif