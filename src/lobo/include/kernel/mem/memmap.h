#pragma once

#include <assert.h>
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
    memmap(size_t size, int perm, int flags) : sz(size) {
        uint64_t virt_addr = (uint64_t)vmm::get().map(nullptr, size, perm, flags);
        m_ram_backed       = true;
        addr               = virt_addr;
    }
    /// Allocate some physically backed memory at address that's \a size long
    memmap(uint64_t address, size_t size, int perm, int flags) : addr(address), sz(size) {
        uint64_t virt_addr = (uint64_t)vmm::get().map((void*)address, size, perm, flags);
        m_ram_backed       = true;
        addr               = virt_addr;
    }

    /// Map p_addr to v_addr
    memmap(uint64_t p_addr, uint64_t v_addr, size_t size, int perm, int flags) : addr(v_addr), sz(size) {
        vmm::get().map_direct((void*)v_addr, (void*)p_addr, size, perm, flags);
        m_ram_backed = false;
        this->addr   = v_addr;
        this->sz     = size;
    }

    ~memmap() {
        if (m_ram_backed) {}
        vmm::get().unmap((void*)addr, sz);
    }

    void* get() const {
        return (void*)addr;
    }

    uint8_t& operator[](size_t idx) {
        assert(idx < sz);
        return ((uint8_t*)addr)[idx];
    }

   private:
    bool           m_ram_backed = false;
    virtual_addr_t addr;
    size_t         sz;
};
}  // namespace kernel