#pragma once

#include <stddef.h>
#include <stdint.h>

#include <assert.h>

namespace common {
/// A class that allows bit-access to m_ptr
class bitset {
   public:
    bitset() : m_ptr(nullptr), m_length(0) {}
    bitset(uint8_t* ptr, size_t length) : m_ptr(ptr), m_length(length) {}

    bool test(size_t bit) {
        assert(bit < m_length);
        return m_ptr[bit / 8] & (1 << bit % 8);
    }
    void set(size_t bit) {
        assert(bit < m_length);
        m_ptr[bit / 8] |= (1 << (bit % 8));
    }
    void clear(size_t bit) {
        assert(bit < m_length);
        m_ptr[bit / 8] &= ~(1 << (bit % 8));
    }
    void flip(size_t bit) {
        assert(bit < m_length);
        test(bit) ? clear(bit) : set(bit);
    }

   private:
    uint8_t* m_ptr    = nullptr;
    size_t   m_length = 0;
};

}  // namespace common
