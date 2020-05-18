#pragma once
#include <common/bitset.h>
#include <stddef.h>
#include <stdint.h>

#define PMM_NUM_BITMAPS 4

namespace kernel {

/// The Physical Memory Manager.
/// Manages the physical address space of the computer
class PMM {
   private:
    /// Has the PMM been initialised?
    bool hasBeenInitialised = false;
    /// The backing bitmap.
    common::bitset bitmap;
    /// How many pages there are
    uint64_t       numPages;

   public:
    /// Maximum number of free pages
    uint64_t m_maxFreePages = 0;
    /// How many pages are considered mapped, but not to usable memory.
    uint64_t m_nonMemoryBackedPages = 0;

   public:
    void        Init(uint64_t numPages);
    static PMM& Get();

   public:
    // Allocates n continuous pages, returning the first allocated page.
    uint64_t Allocate(uint64_t n);

   public:
    /// Marks a page as free in Physical Memory
    void FreePage(uint64_t physAddress);
    /// Sets a page as used in Physical Memory
    void SetPage(uint64_t physAddress);
    /// Tests to see if a page is used in Physical Memory
    bool TestPage(uint64_t physAddress);

   public:
    /// Prints all pages marked as free. Very slow, only use for debugging.
    void DebugPrintFreePages();
    /// Prints how much memory is free
    void DebugPrintFreeMemory();
    /// Sets the current number of allocated pages as the 0 point for free memory.
    void SetFreeMemory();

   private:
    /// Sets physAddress to free in bitmap
    void BitmapFreePage(uint64_t physAddress);
    /// Sets physAddress to used in bitmap
    void BitmapSetPage(uint64_t physAddress);
    /// Checks if physAddress is used
    bool BitmapTestPage(uint64_t physAddress);
};

};  // namespace Kernel