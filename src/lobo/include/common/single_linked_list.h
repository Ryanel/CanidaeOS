#pragma once

#include <assert.h>
#include <kernel/heap.h>
#include <stddef.h>
#include <stdint.h>

namespace common {

/// A singlely linked list
template <class T>
class single_linked_list {
   public:
    // Structures
    template <class U>
    struct linked_list_node {
        linked_list_node(U val, linked_list_node<U>* next) : m_value(val), m_next(next) {}
        U                    m_value;
        linked_list_node<U>* m_next;
    };

    template <class U>
    struct linked_list_iterator {
        linked_list_node<U>* node;
        linked_list_iterator() : node(nullptr) {}
        linked_list_iterator(linked_list_node<U>* tmp) : node(tmp) {}
        linked_list_iterator(const linked_list_iterator& itr) : node(itr.node) {}

        linked_list_iterator& operator++() {
            if (node != nullptr) { node = node->m_next; }
            return *this;
        }

        linked_list_iterator& operator++(int) {
            linked_list_iterator& it = *this;
            ++(*this);
            return it;
        }

        bool operator==(const linked_list_iterator& o) { return node == o.node; }

        bool operator!=(const linked_list_iterator& o) { return node != o.node; }

        U& operator*() { return node->m_value; }

        U* operator->() { return node->m_value; }
    };

    // Functions

    // O(1)
    void push_back(T val) {
        if (m_head == nullptr) {
            create_head(val);
            return;
        }

        linked_list_node<T>* new_node = (linked_list_node<T>*)kmalloc(sizeof(linked_list_node<T>));
        new_node->m_value             = val;
        new_node->m_next              = nullptr;
        m_tail->m_next                = new_node;
        m_tail                        = new_node;
        m_size++;
    }

    // O(1)
    void push_front(T& val) {
        if (m_head == nullptr) {
            create_head(val);
            return;
        }

        linked_list_node<T>* new_node = (linked_list_node<T>*)kmalloc(sizeof(linked_list_node<T>));
        new_node->m_value             = val;
        new_node->m_next              = m_head;
        m_head                        = new_node;
        m_size++;
    }

    // O(n - 1)
    linked_list_node<T>* pop_back() {
        if (m_size == 0) { return nullptr; }
        auto* ret = m_tail;

        auto* itr = m_head;
        for (int i = 0; i < m_size - 1; i++) { itr = itr->next; }

        m_tail         = itr;
        m_tail->m_next = nullptr;
        m_size--;
    }

    T at(size_t index) const {
        if (index >= m_size) { return nullptr; }

        auto start_itr = begin();
        for (size_t i = 0; i < index; i++) { start_itr++; }
        return (*start_itr);
    }

    size_t size() const { return m_size; }

    linked_list_iterator<T> begin() const { return linked_list_iterator<T>(m_head); }

    linked_list_iterator<T> end() const { return linked_list_iterator<T>(); }

   private:
    void create_head(T& val) {
        m_head          = (linked_list_node<T>*)kmalloc(sizeof(linked_list_node<T>));
        m_head->m_value = val;
        m_head->m_next  = nullptr;
        m_size          = 1;
        m_tail          = m_head;
    }

   private:
    linked_list_node<T>* m_head = nullptr;
    linked_list_node<T>* m_tail = nullptr;
    size_t               m_size = 0;
};

}  // namespace common
