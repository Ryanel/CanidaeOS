#include "awd/awd_info.h"
#include "kernel/console.h"
#include <stdint.h>
#include <stdio.h>

extern "C" int kernel_entry(awd_info_t * awd_info) {
    console_init();
    console_setcursor(0, awd_info->log_cursor_y);

    console_log("lobo","Lobo Kernel (64bit): v.0.0.0.1\n");

    while(true) {

    }
}