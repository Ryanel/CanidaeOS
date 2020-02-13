#include <awd/memmap.h>
#include <memory.h>
#include <stddef.h>

#define AWD_MEMMAP_FREE 0
#define AWD_MEMMAP_USED 1

awd_physmemmap_t* root = NULL;
awd_physmemmap_t* memmap_phys_get_root() { return root; }

void memmap_phys_add(awd_physmemmap_t* x) {
    if (root == NULL) {
        root = x;
        return;
    }

    while (root->next != 0) { root = root->next; }
    root->next = x;
}
awd_physmemmap_t* memmap_phys_create(char type, uint64_t startAddress, uint64_t size) {
    awd_physmemmap_t* ret = (awd_physmemmap_t*)malloc(sizeof(awd_physmemmap_t));
    ret->type             = type;
    ret->startAddress     = startAddress;
    ret->size             = size;
    return ret;
}
