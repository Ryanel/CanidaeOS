#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

namespace common {
/// A class that allows bit-access to a pointer.
class bitset {
   public:
    /// Construct a null bitset.
    bitset() : m_ptr(nullptr), m_length(0) {}
    /// Construct a bitset pointing to ptr with size length
    bitset(uint8_t* ptr, size_t length) : m_ptr(ptr), m_length(length) {}

    /// Test if bit is set
    bool test(size_t bit) const {
        assert(bit < m_length);
        return m_ptr[bit / 8] & (1 << bit % 8);
    }
    /// Set bit bit
    void set(size_t bit) {
        assert(bit < m_length);
        m_ptr[bit / 8] |= (1 << (bit % 8));
    }
    /// Clear bit bit
    void clear(size_t bit) {
        assert(bit < m_length);
        m_ptr[bit / 8] &= ~(1 << (bit % 8));
    }
    /// Flip bit
    void flip(size_t bit) {
        assert(bit < m_length);
        test(bit) ? clear(bit) : set(bit);
    }
    /// Get a reference to the pointer
    uint8_t* ptr() const {return m_ptr;}

   private:
    uint8_t* m_ptr    = nullptr;
    size_t   m_length = 0;
};

}  // namespace common
