#pragma once

#include <stdint.h>
#include <kernel/driver.h>

class ps2_keyboard : public kernel::drivers::driver {
   private:
    const uint8_t io_port_data = 0x60;
    const uint8_t io_port_cmd = 0x64;
    uint8_t cmd_queue[10];
    uint8_t * kb_scancodes;

   public:
    ps2_keyboard() : kernel::drivers::driver("ps2_keyboard", kernel::drivers::driver_type::IN_KERNEL) {}
    void on_interrupt();
};