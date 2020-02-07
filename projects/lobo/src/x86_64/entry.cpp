#include <stdint.h>
#include <stdio.h>

#include "awd/awd_info.h"
#include "kernel/console.h"

extern "C" int kernel_entry(awd_info_t* awd_info) {
    console_init();
    console_setcursor(0, awd_info->log_cursor_y);

    console_log("lobo", "Lobo Kernel [v.0.0.0.1]\n");
    console_log("lobo", "Entering infinite loop\n");

    while (true) {
    }
}