#include <kernel/arch/x86_64/ports.h>
#include <kernel/arch/x86_64/drivers/ps2_keyboard.h>
#include <kernel/hal/keyboard.h>
#include <kernel/log.h>


void ps2_keyboard::on_interrupt() {
    uint8_t scan = inb(io_port_data);
    kernel::hal::keyboard::get().recieve_scancode(scan);
}