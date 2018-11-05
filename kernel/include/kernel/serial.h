#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/io.h>

// COMs base addresses
#define SERIAL_COM1_BASE 0x3f8
#define SERIAL_COM2_BASE 0x2f8
#define SERIAL_COM3_BASE 0x3e8
#define SERIAL_COM4_BASE 0x2e8

// PORT = REGISTER
#define SERIAL_DATA_PORT(base)				(base)
#define SERIAL_ENABLE_INTERRUPT_PORT(base) 	(base + 1)
#define SERIAL_FIFO_COMMAND_PORT(base)		(base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)		(base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base)		(base + 4)
#define SERIAL_LINE_STATUS_PORT(base)		(base + 5)
#define SERIAL_MODEM_STATUS_PORT(base)		(base + 6)
#define SERIAL_SCRATCH_PORT(base)			(base + 7)

#define SERIAL_LINE_ENABLE_DLAB	0x080

/* INPUT */
char serial_getchar(unsigned short com);
int serial_read(unsigned short com, char* str, size_t size);

/* OUTPUT */
void serial_putchar(unsigned short com, char * c);
void serial_write(unsigned short com, char * data, size_t size);
void serial_writeline(unsigned short com, char * data);

/* CONF */
void init_serial(unsigned short com);
int serial_received(unsigned short com);
void serial_conf_baud_rate(unsigned short com, unsigned short divisor);
void serial_conf_line(unsigned short com);
void serial_conf_buffer(unsigned short com);
void serial_conf_modem(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned short com);

#endif