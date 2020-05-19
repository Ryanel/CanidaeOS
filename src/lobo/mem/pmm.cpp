#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "kernel/heap.h"
#include "kernel/log.h"
#include "kernel/panic.h"
#include "kernel/pmm.h"

using namespace kernel;

static pmm kernel_pmm;

pmm& pmm::get() { return kernel_pmm; }

void pmm::init(uint64_t num) {
    if (m_initialised) { return; }
    // Set properties
    this->m_numPages = num;
    // Initialise the bitmap
    bitmap = common::bitset((uint8_t*)kmalloc((this->m_numPages / 8) + 1), this->m_numPages);
    // Set all pages as used by default.
    for (size_t i = 0; i < this->m_numPages; i++) { set(i * 0x1000); }
    // We're finished, PMM online.
    m_initialised = true;
}

uint64_t pmm::allocate(uint64_t numContiguous) {
    uint64_t idx = 0;

    while (idx < m_numPages) {
        uint64_t init_address = idx * 0x1000;
        if (test(init_address) == false) {
            // We have a starting point for a free address!

            // Simple case, just use and return this address
            if (numContiguous <= 1) {
                set(init_address);
                return init_address;
            }

            // Otherwise, check the next pages if they're free
            bool allWasFree = true;
            for (unsigned int checkIndex = idx + 1; checkIndex < idx + numContiguous; checkIndex++) {
                if (test(checkIndex * 0x1000) == true) {
                    allWasFree = false;
                    break;
                }
            }

            if (allWasFree) {
                for (unsigned int k = idx; k < idx + numContiguous; k++) { set(k * 0x1000); }
                return init_address;
            }
        }
        idx++;
    }

    log::Get().Log("pmm", "Out of memory, no physical fragment of size %d", numContiguous);
    panic("PMM Out of Memory");
    return 0;
}

void pmm::set(uint64_t physAddress) { bitmap_set(physAddress); }

void pmm::free(uint64_t physAddress) { bitmap_free(physAddress); }

bool pmm::test(uint64_t physAddress) { return bitmap_test(physAddress); }

void pmm::bitmap_set(uint64_t physAddress) { bitmap.set(physAddress / 0x1000); }

void pmm::bitmap_free(uint64_t physAddress) { bitmap.clear(physAddress / 0x1000); }

bool pmm::bitmap_test(uint64_t physAddress) { return bitmap.test(physAddress / 0x1000); }

void pmm::debug_print_free_pages() {
    uint64_t numFreePages = 0;
    auto&    kLog         = log::Get();
    for (size_t i = 0; i < m_numPages; i++) {
        uint64_t address = i * 0x1000;
        if (test(address) == false) {
            // kLog.Log("pmm","Page 0x%016p free", address);
            numFreePages++;
        }
    }
    uint64_t pagesUsed = m_numPages - numFreePages;

    kLog.Log("pmm", "%d/%d pages free", numFreePages, m_numPages);
    kLog.Log("pmm", "Used %dKB / %dKB of memory (%dMB / %dMB)", pagesUsed * 4, m_numPages * 4, (pagesUsed * 4) / 1024,
             (m_numPages * 4) / 1024);
}

void pmm::debug_print_free_memory() {
    uint64_t numFreePages = 0;
    auto&    kLog         = log::Get();
    for (size_t i = 0; i < m_numPages; i++) {
        uint64_t address = i * 0x1000;
        if (test(address) == false) {
            // kLog.Log("pmm","Page 0x%016p free", address);
            numFreePages++;
        }
    }
    uint64_t pagesUsed = m_numPages - numFreePages - m_nonMemoryBackedPages;
    uint64_t pageLimit = m_maxFreePages;
    kLog.Log("pmm", "%d/%d pages free", numFreePages, pageLimit);
    kLog.Log("pmm", "Used %dKB / %dKB of memory (%dMB / %dMB)", pagesUsed * 4, pageLimit * 4, (pagesUsed * 4) / 1024,
             (pageLimit * 4) / 1024);
}

void pmm::set_free_memory() {
    uint64_t numFreePages = 0;
    for (size_t i = 0; i < m_numPages; i++) {
        uint64_t address = i * 0x1000;
        if (test(address) == false) {
            numFreePages++;
        } else {
            m_nonMemoryBackedPages++;
        }
    }

    m_maxFreePages = numFreePages;
}