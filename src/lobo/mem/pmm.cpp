#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "kernel/heap.h"
#include "kernel/log.h"
#include "kernel/panic.h"
#include "kernel/pmm.h"

using namespace Kernel;

static PMM kernel_pmm;

PMM& PMM::Get() { return kernel_pmm; }

void PMM::Init(uint64_t num) {
    if (hasBeenInitialised) { return; }
    this->numPages = num;
    bitmap = common::bitset((uint8_t*)kmalloc((this->numPages / 8) + 1), this->numPages);

    hasBeenInitialised = true;

    for (size_t i = 0; i < this->numPages; i++) { SetPage(i * 0x1000); }
}

uint64_t PMM::Allocate(uint64_t numContiguous) {
    uint64_t idx = 0;

    while (idx < numPages) {
        uint64_t init_address = idx * 0x1000;
        if (TestPage(init_address) == false) {
            // We have a starting point for a free address!

            // Simple case, just use and return this address
            if (numContiguous <= 1) {
                SetPage(init_address);
                return init_address;
            }

            // Otherwise, check the next pages if they're free
            bool allWasFree = true;
            for (unsigned int checkIndex = idx + 1; checkIndex < idx + numContiguous; checkIndex++) {
                if (TestPage(checkIndex * 0x1000) == true) {
                    allWasFree = false;
                    break;
                }
            }

            if (allWasFree) {
                for (unsigned int k = idx; k < idx + numContiguous; k++) { SetPage(k * 0x1000); }
                return init_address;
            }
        }
        idx++;
    }

    KernelLog::Get().Log("pmm", "Out of memory, no physical fragment of size %d", numContiguous);
    panic("PMM Out of Memory");
    return 0;
}

void PMM::SetPage(uint64_t physAddress) { BitmapSetPage(physAddress); }

void PMM::FreePage(uint64_t physAddress) { BitmapFreePage(physAddress); }

bool PMM::TestPage(uint64_t physAddress) { return BitmapTestPage(physAddress); }

void PMM::BitmapSetPage(uint64_t physAddress) {
    bitmap.set(physAddress / 0x1000);
}

void PMM::BitmapFreePage(uint64_t physAddress) {
    bitmap.clear(physAddress / 0x1000);
}

bool PMM::BitmapTestPage(uint64_t physAddress) {
    return bitmap.test(physAddress / 0x1000);
}

void PMM::DebugPrintFreePages() {
    uint64_t numFreePages = 0;
    auto&    kLog         = KernelLog::Get();
    for (size_t i = 0; i < numPages; i++) {
        uint64_t address = i * 0x1000;
        if (TestPage(address) == false) {
            // kLog.Log("pmm","Page 0x%016p free", address);
            numFreePages++;
        }
    }
    uint64_t pagesUsed = numPages - numFreePages;

    kLog.Log("pmm", "%d/%d pages free", numFreePages, numPages);
    kLog.Log("pmm", "Used %dKB / %dKB of memory (%dMB / %dMB)", pagesUsed * 4, numPages * 4, (pagesUsed * 4) / 1024,
             (numPages * 4) / 1024);
}

void PMM::DebugPrintFreeMemory() {
    uint64_t numFreePages = 0;
    auto&    kLog         = KernelLog::Get();
    for (size_t i = 0; i < numPages; i++) {
        uint64_t address = i * 0x1000;
        if (TestPage(address) == false) {
            // kLog.Log("pmm","Page 0x%016p free", address);
            numFreePages++;
        }
    }
    uint64_t pagesUsed = numPages - numFreePages - m_nonMemoryBackedPages;
    uint64_t pageLimit = m_maxFreePages;
    kLog.Log("pmm", "%d/%d pages free", numFreePages, pageLimit);
    kLog.Log("pmm", "Used %dKB / %dKB of memory (%dMB / %dMB)", pagesUsed * 4, pageLimit * 4, (pagesUsed * 4) / 1024,
             (pageLimit * 4) / 1024);
}

void PMM::SetFreeMemory() {
    uint64_t numFreePages = 0;
    for (size_t i = 0; i < numPages; i++) {
        uint64_t address = i * 0x1000;
        if (TestPage(address) == false) {
            // kLog.Log("pmm","Page 0x%016p free", address);
            numFreePages++;
        }
        else {
            m_nonMemoryBackedPages++;
        }
    }
    
    m_maxFreePages = numFreePages;
}