#pragma once

#include <stdint.h>

namespace kernel {
    namespace hal {
        class keyboard {
            public:
            static keyboard& get();
            void recieve_scancode(uint8_t s);
        };
    }
}