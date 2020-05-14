#ifndef PORTS_H
#define PORTS_H

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

#endif