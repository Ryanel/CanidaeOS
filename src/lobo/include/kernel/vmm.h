#pragma once
#include <stddef.h>
#include <stdint.h>

#include "awd/awd_info.h"

#define VMM_PAGE_PRESENT 0x01
#define VMM_PAGE_WRITE 0x02
#define VMM_PAGE_USER 0x04
#define VMM_MAP_KERNEL 0x400
#define PAGE_SIZE 0x1000

#define VMM_ROUND_DOWN(x) (((x)) & ~(PAGE_SIZE - 1))

typedef uint64_t physical_addr_t;  /// A physical RAM address
typedef uint64_t logical_addr_t;   /// An unmapped virtual address
typedef uint64_t virtual_addr_t;   /// A mapped virtual address

typedef struct page_entry {
    physical_addr_t data;
} page_entry_t;

typedef struct page_table {
    physical_addr_t entries[512];
} page_table_t;

namespace kernel {
/**
 * \brief The Kernel's Virtual Memory Manager
 * Manages the Virtual Memory and Page Tables of the Kernel and CPU.
 */
class vmm {
   public:
    bool          m_init      = false;    ///< Is the PMM initialised?
    page_table_t* kernel_pdir = nullptr;  ///< The kernels' embryonic page directory

    /**
     * Initialises the Virtual Memory Manager with info from AWD
     * \param awd_info The AWD info structure
     */
    void init(awd_info_t* awd_info);

    /**
     * Allows access to the Virtual Memory Manager.
     * \returns a reference to the global Virtual Memory Manager
     */
    static vmm& get();

    /**
     * Maps \a address and the following \a length bytes to the current memory space
     *
     * \param address The starting virtual address to map
     * \param length The number of following bytes to map.
     * \see unmap()
     */
    void* map(void* address, size_t length, int perm, int flags);

    /**
     * Unmaps \a address and the following \a length bytes from the current memory space.
     *
     * \param address The starting virtual address to unmap
     * \param length The number of following bytes to unmap.
     * \see map()
     */
    void* unmap(void* address, size_t length);

    /**
     * Changes the active memory space of the calling thread to \a newMemorySpace.
     *
     * \param newMemorySpace The top-level page table to use for memory translation.
     */
    void swap_page_directory(page_table_t* newMemorySpace);

    /**
     * Invalidates a page in the TLB.
     * \param addr The address of the page to invalidate
     */
    void invalidate(uint64_t addr);

    /**
     * Invalidates a range of pages in the TLB.
     * \param start_addr The start address of the pages to invalidate
     * \param end_addr The end address of the pages to invalidate
     */
    void invalidate(uint64_t start_addr, uint64_t end_addr);

    /**
     * Invalidates the entire TLB
     * \warning This reduces performance significantly. Use sparingly.
     */
    void invalidate();
    
   private:
    /// Initialise the primative kernel page directory
    void init_kernel_page_directory(awd_info_t* awd_info);
    /// Find a page table entry in \a p4 at logical address \a addr. If the neccisary tables don't exist \a create them
    /// if possible.
    page_entry_t* find_page_entry(page_table_t* p4, logical_addr_t addr, bool create);
    /// Find a page table within another page table. 
    virtual_addr_t find_page_table(page_table_t* pt, uintptr_t index, bool create);
    /**
     * Map \a p_addr to \a v_addr in page directory dir. Will map \a size bytes consecutively. Maps with \a flags permissions
     * \param dir The page directory to map in
     * \param v_addr The logical address to map to
     * \param p_addr The physical address to map from
     * \param size The amount of bytes to map, rounded up to the nearest page size.
     * \param flags The MMU permission flags.
     */
    void map_pages(page_table_t* dir, logical_addr_t v_addr, physical_addr_t p_addr, size_t size, uint8_t flags);
};
};  // namespace Kernel