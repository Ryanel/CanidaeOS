#pragma once

#include <assert.h>
#include <kernel/heap.h>
#include <stddef.h>
#include <stdint.h>

namespace common {

/// A singlely linked list
template <class T>
class linked_list {
   public:
    template <class U>
    struct linked_list_node {
        U                    value;
        linked_list_node<U>* next;
    };

    void push_back(T val) {
        if (m_root == nullptr) {
            m_root        = (linked_list_node<T>*)kmalloc(sizeof(linked_list_node<T>));
            m_root->value = val;
            m_root->next  = nullptr;
            m_size = 1;
            return;
        }

        linked_list_node<T>* it = m_root;

        while (it->next != nullptr) { it = it->next; }

        linked_list_node<T>* new_node = (linked_list_node<T>*)kmalloc(sizeof(linked_list_node<T>));
        new_node->value               = val;
        new_node->next                = nullptr;
        it->next                      = new_node;
        m_size++;
    }
    
    linked_list_node<T>* begin() {
        return m_root;
    }

   public:
    linked_list_node<T>* m_root = nullptr;
    size_t               m_size = 0;
};

}  // namespace common
