//-----------------------------------------------------------------------------
// error.c
// (C) 2020 Corwin McKnight
//-----------------------------------------------------------------------------

#include "awd/error.h"
#include "awd/console.h"

void panic(const char* s) {
    console_log("panic", s);
    
    console_printchar('\n');

    while (1) {
        asm("cli");
        asm("hlt");
    }
}