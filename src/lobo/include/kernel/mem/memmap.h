#pragma once

#include <kernel/log.h>
#include <kernel/vmm.h>

namespace kernel {
/**
 * \brief A RAII memory mapping
 * On construction, constructs a memory map.
 * On destruction, unmaps the region of memory.
 */
class memmap {
   public:

    memmap() = delete;
    /// Allocate some physically backed memory that's \a size long
    memmap(size_t size, int perm, int flags) {
        uint64_t virt_addr = (uint64_t)vmm::get().map(nullptr, size, perm, flags);
        m_ram_backed       = true;
        KernelLog::Get().Log("memmap", "Constructed random RAM backed memory @ 0x%16p", virt_addr);
    }
    /// Allocate some physically backed memory at address that's \a size long
    memmap(uint64_t address, size_t size, int perm, int flags) {
        uint64_t allocated_virt_address = (uint64_t)vmm::get().map((void*)address, size, perm, flags);
        m_ram_backed       = true;
        KernelLog::Get().Log("memmap", "Constructed RAM backed memory @ 0x%16p", allocated_virt_address);
    }

   private:
    bool m_ram_backed = false;
};
}  // namespace kernel