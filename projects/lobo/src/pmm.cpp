#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/panic.h"

using namespace Kernel;

static PMM kernel_pmm;

PMM& PMM::Get() { return kernel_pmm; }

void PMM::Init(uint64_t num) {
    if(hasBeenInitialised) {return;}
    this->numPages = num;
    for (size_t i = 0; i < numPages; i++)
    {
       SetPage(i * 0x1000);
    }
}

uint64_t PMM::Allocate(uint64_t n) {
    uint64_t idx = 0;

    while(idx < numPages) {
        uint64_t init_address = idx * 0x1000;
        if(TestPage(init_address) == false) {
            // We have a starting point for a free address!

            // Simple case, just use and return this address
            if(n <= 1) { 
                SetPage(init_address);
                return init_address;
            }

            // Otherwise, check the next pages if they're free
            bool allWasFree = true;
            for (unsigned int checkIndex = idx + 1; checkIndex < idx + n; checkIndex++)
            {
                if(TestPage(checkIndex * 0x1000) == true) {
                    allWasFree = false;
                    break;
                }
            }

            if(allWasFree) {
                for (unsigned int k = idx; k < idx + n; k++)
                {
                    SetPage(k * 0x1000);
                }
                return init_address;
            }
        }
        idx++;
    }

    KernelLog::Get().Log("pmm", "Out of memory, no physical fragment of size %d", n);
    panic("PMM Out of Memory");

    return 0;
}

void PMM::SetPage(uint64_t physAddress) {
    BitmapSetPage(physAddress);
}

void PMM::FreePage(uint64_t physAddress) {
    BitmapFreePage(physAddress);
}

bool PMM::TestPage(uint64_t physAddress) {
    return BitmapTestPage(physAddress);
}

void PMM::BitmapSetPage(uint64_t physAddress) {
    uint64_t pageNumber = physAddress / 0x1000;
    uint64_t bitNumber  = pageNumber % 8;
    bitmap[pageNumber  / 8] |= (1 << (bitNumber));
}

void PMM::BitmapFreePage(uint64_t physAddress) {
    uint64_t pageNumber = physAddress / 0x1000;
    uint64_t bitNumber  = pageNumber % 8;
    bitmap[pageNumber / 8] &= ~(1 << (bitNumber));
}

bool PMM::BitmapTestPage(uint64_t physAddress) {
    uint64_t pageNumber = physAddress / 0x1000;
    uint64_t bitNumber  = pageNumber % 8;
    return bitmap[pageNumber  / 8] & (1 << bitNumber);
}

void PMM::DebugPrintFreePages() {
    uint64_t numFreePages = 0;
    auto& kLog = KernelLog::Get();
    for (size_t i = 0; i < numPages; i++)
    {
        uint64_t address = i * 0x1000;
        if(TestPage(address) == false) {
            //kLog.Log("pmm","Page 0x%016p free", address);
            numFreePages++;
        }
    }
    kLog.Log("pmm", "%d pages free", numFreePages);
}