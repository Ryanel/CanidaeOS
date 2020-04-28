#include <awd/memmap.h>
#include <memory.h>
#include <stddef.h>
#include"awd/console.h"
#define AWD_MEMMAP_FREE 0
#define AWD_MEMMAP_USED 1

awd_physmemmap_t* root = NULL;
awd_physmemmap_t* memmap_phys_get_root() { return root; }

void memmap_phys_add(awd_physmemmap_t* x) {
    if (root == NULL) {
        root = x;
        
        return;
    }
    awd_physmemmap_t* srcrt = root;

    while (srcrt->next != 0) { 
        srcrt = (awd_physmemmap_t*)srcrt->next; 
    }

    srcrt->next = (uint32_t)(x);
}
awd_physmemmap_t* memmap_phys_create(char type, uint32_t startAddress_lo, uint32_t startAddress_hi, uint64_t size) {
    awd_physmemmap_t* ret = (awd_physmemmap_t*)malloc(sizeof(awd_physmemmap_t));
    ret->type             = type;
    ret->startAddress_lo  = startAddress_lo;
    ret->startAddress_hi  = startAddress_hi;
    ret->size             = size;
    return ret;
}
