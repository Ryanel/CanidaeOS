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
    template <class U>
    struct linked_list_node {
        U                    value;
        linked_list_node<U>* next;
    };

    // O(1)
    void push_back(T val) {
        if (m_head == nullptr) {
            create_head(val);
            return;
        }

        linked_list_node<T>* new_node = (linked_list_node<T>*)kmalloc(sizeof(linked_list_node<T>));
        new_node->value               = val;
        new_node->next                = nullptr;
        m_tail->next                  = new_node;
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
        new_node->value               = val;
        new_node->next                = m_head;
        m_head                        = new_node;
        m_size++;
    }

    // O(n - 1)
    linked_list_node<T>* pop_back() {
        if(m_size == 0) { return nullptr; }
        auto * ret = m_tail;

        auto * itr = m_head;
        for(int i = 0; i < m_size - 1; i++) {
            itr = itr->next;
        }
        
        m_tail = itr;
        m_tail->next = nullptr;
        m_size--;
    }

    linked_list_node<T>* begin() const { return m_head; }
    linked_list_node<T>* end() const { return m_tail; }
    
    size_t size() const { return m_size; }

   private:
    void create_head(T& val) {
        m_head        = (linked_list_node<T>*)kmalloc(sizeof(linked_list_node<T>));
        m_head->value = val;
        m_head->next  = nullptr;
        m_size        = 1;
        m_tail        = m_head;
    }

   public:
    linked_list_node<T>* m_head = nullptr;
    linked_list_node<T>* m_tail = nullptr;
    size_t               m_size = 0;
};

}  // namespace common
