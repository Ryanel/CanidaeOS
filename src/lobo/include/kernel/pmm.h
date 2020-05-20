#pragma once

#include <stddef.h>
#include <stdint.h>
#include <common/bitset.h>

namespace kernel {

/// The Physical Memory Manager.
/// Manages the physical address space of the computer
class pmm {
   public:
    /// Initialse the PMM to manage up to \a numPages pages
    void init(uint64_t numPages);
    /// Get a refrence to the PMM
    static pmm& get();
    // Allocates n continuous pages, returning the first allocated page.
    uint64_t allocate(uint64_t n);

    // TODO: Make not public!
   public:
    /// Marks a page as free in Physical Memory
    void free(uint64_t physAddress);
    /// Sets a page as used in Physical Memory
    void set(uint64_t physAddress);
    /// Tests to see if a page is used in Physical Memory
    bool test(uint64_t physAddress);

   public:
    /// Prints all pages marked as free. Very slow, only use for debugging.
    void debug_print_free_pages();
    /// Prints how much memory is free
    void debug_print_free_memory();
    /// Sets the current number of allocated pages as the 0 point for free memory.
    void set_free_memory();

   private:
    /// Sets \a physAddress to free in bitmap
    void bitmap_free(uint64_t physAddress);
    /// Sets \a physAddress to used in bitmap
    void bitmap_set(uint64_t physAddress);
    /// Checks if \a physAddress is used
    bool bitmap_test(uint64_t physAddress);

   private:
    common::bitset bitmap;                          ///< The backing bitmap.
    bool           m_initialised          = false;  ///< Has the PMM been initialised?
    uint64_t       m_numPages             = 0;      ///< How many pages there are
    uint64_t       m_maxFreePages         = 0;      ///< Maximum number of free pages
    uint64_t       m_nonMemoryBackedPages = 0;      ///< How many pages are considered mapped, but not to usable memory.
};

};  // namespace kernel