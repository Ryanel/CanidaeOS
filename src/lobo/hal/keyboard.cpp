#include <kernel/hal/keyboard.h>
#include <kernel/log.h>
#include <assert.h>
#include <string.h>

using namespace kernel;
using namespace kernel::hal;

keyboard hal_kbd_instance;

const unsigned char kbdus[256] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

const unsigned char kbdus_shift[256] =
{
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '\"', '~',   0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

keyboard& keyboard::get() {
    return hal_kbd_instance;
}

void keyboard::recieve_scancode(uint8_t s) {
    assert(scancode_queue_sz < scancode_queue_max);
    scancode_queue[scancode_queue_sz++] = s;
}
bool keyboard::queue_empty() {
    return scancode_queue_sz == 0;
}
uint8_t keyboard::scancode_queue_pop() {
    if(queue_empty()) {return 0;}

    uint8_t val = scancode_queue[0];
    memmove(&scancode_queue[0], &scancode_queue[1], (scancode_queue_sz - 1));
    scancode_queue[scancode_queue_sz] = 0;
    scancode_queue_sz--;
    return val;
}

char keyboard::getchar_async() {
    while(true) {
        bool sc_is_up = false;
        char sc = read_scancode();    
        if(sc == 0) { // no data
            return 0;
        }
        // If this is an up scancode...
        if(sc & 0x80) {
            // Set a flag and convert it into a normal scancode.
            sc_is_up = true;
            sc &= 0x7F;
        }

        // TODO: Fix awful modifier key implementation
        if(sc == 0x2A) // Left Shift
        {
            mod_shift_set = !sc_is_up;
        }

        if(!sc_is_up) {
            if(mod_shift_set) {
                return (kbdus_shift[(unsigned)sc]);
            }
            else {
                return kbdus[(unsigned)sc];
            }
        }
        else {
            // Treat up-scans as if they don't happen
            continue; // Search for valid character to return...
        }
        
    }
}

char keyboard::read_scancode() {
    if(queue_empty() == true) {return 0;}
    // We have something in the queue, figure out what kind of value it is.
    uint8_t scancode = scancode_queue_pop();
    return scancode;
    
}
