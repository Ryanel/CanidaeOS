#pragma once

#include <stdint.h>

namespace kernel {
namespace hal {
class keyboard {
   public:
    static keyboard& get();
    void             recieve_scancode(uint8_t s);
    bool             queue_empty();
    char             getchar_async();

   private:
    const static uint64_t scancode_queue_max = 128;
    uint8_t               scancode_queue[scancode_queue_max];
    uint64_t              scancode_queue_sz = 0;
    bool                  mod_shift_set     = false;

    char    read_scancode();
    uint8_t scancode_queue_pop();
};
}  // namespace hal
}  // namespace kernel