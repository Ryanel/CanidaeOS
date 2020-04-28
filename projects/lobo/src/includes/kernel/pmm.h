#pragma once
#include <stddef.h>
#include <stdint.h>

/// Physical Memory Manager
/// Manages the physical address space of the computer

#define PMM_NUM_BITMAPS 4

namespace Kernel {
    
class PMM {
    bool hasBeenInitialised = false;
    uint8_t bitmap[0x20000];
    uint64_t numPages;
   public:
    void Init(uint64_t numPages);
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

private:
    /// Immediately marks the bitmap as free
    void BitmapFreePage(uint64_t physAddress);
    void BitmapSetPage(uint64_t physAddress);
    bool BitmapTestPage(uint64_t physAddress);
};

};