#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

//void outb(unsigned short port, unsigned char data);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif