#pragma once
#include <stddef.h>
#include <stdint.h>

#include "awd/awd_info.h"

#define VMM_PAGE_PRESENT 0x01
#define VMM_PAGE_WRITE 0x02
#define VMM_PAGE_USER 0x04
#define PAGE_SIZE 0x1000

#define VMM_ROUND_DOWN(x) (((x)) & ~(PAGE_SIZE-1))

typedef uint64_t physical_addr_t; /// A physical RAM address
typedef uint64_t logical_addr_t;  /// An unmapped virtual address
typedef uint64_t virtual_addr_t;  /// A mapped virtual address

typedef struct page_entry {
    physical_addr_t data;
} page_entry_t;

typedef struct page_table {
    physical_addr_t entries[512];
} page_table_t;

/// Physical Memory Manager
namespace Kernel {
/// Manages the physical address space of the computer
class VMM {
    bool       hasBeenInitialised = false;
    page_table_t * kernelP4    = nullptr;

   private:
    void SetupKernelPageTable(awd_info_t * awd_info);
    page_entry_t * FindEntry(page_table_t* p4, logical_addr_t addr, bool create);
    virtual_addr_t GrabTable(page_table_t* pt,uintptr_t index, bool create);
    
    void MapPages(page_table_t* dir, logical_addr_t v_addr, physical_addr_t p_addr, size_t size, uint8_t flags);

   public:
    void        Init(awd_info_t * awd_info);
    static VMM& Get();

    void* Map(void* address, size_t length, int perm, int flags);
    void* Unmap(void* address, size_t length);
};
};  // namespace Kernel