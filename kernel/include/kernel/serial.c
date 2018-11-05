#include <kernel/io.h>
#include <stddef.h>
#include <kernel/serial.h>

/* I/O commands */

void init_serial(unsigned short com){ 
	outb(SERIAL_ENABLE_INTERRUPT_PORT(com), 0x00); 		//No interrupts
	outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB); //enable DLAB
	serial_conf_baud_rate(com, 0x03);	//set divisor (38400 baud)
	serial_conf_line(com);				//8 bits no parity no stop bit
	serial_conf_buffer(com);			//enable FIFO clearing with 14 byte
	serial_conf_modem(com); 			//IRQs enabled, RTS/DSR set
}

int serial_received(unsigned short com){
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 1;
}

char serial_getchar(unsigned short com){
	while(serial_received(com) == 0){};
	char out = inb(SERIAL_DATA_PORT(com));
	serial_putchar(com, &out);		//input return
	return out;
}

int serial_read(unsigned short com, char* str, size_t size){
	size_t i = 0;
	char temp;
	for(size_t i = 0; i <= size; i++){
		temp = serial_getchar(com);
		if (temp == 0x0D){
			str[i]='\0';
			break;
		}else{
			str[i] = temp;
		}
	}
	return i+1;
}

void serial_putchar(unsigned short com, char * c){
	while(serial_is_transmit_fifo_empty(com) == 0){};
		outb(SERIAL_DATA_PORT(com), *c);
}

void serial_write(unsigned short com, char * data, size_t size){
	for(size_t i = 0; i < size; i++){
		serial_putchar(com, &data[i]);
	}
}

void serial_writeline(unsigned short com, char* data){
	serial_write(com, data, strlen(data));
}

int serial_is_transmit_fifo_empty(unsigned short com){
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20; // 0x20 = 00100000
}

/* Baud rate configuration by divisor:  115200 / divisor */

void serial_conf_baud_rate(unsigned short com, unsigned short divisor){
	//outb(SERIAL_DATA_PORT(com), (divisor >> 8 ) & 0x00FF ); 
	//outb(SERIAL_DATA_PORT(com), divisor & 0x00FF );
	outb(SERIAL_DATA_PORT(com), divisor);				//low byte
	outb(SERIAL_ENABLE_INTERRUPT_PORT(com), 0x00);		//high byte
}

void serial_conf_line(unsigned short com){
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03); 	//TODO: write a documentation
}

void serial_conf_buffer(unsigned short com){
	outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);	//enable FIFO clearing with 14 byte
}

void serial_conf_modem(unsigned short com){
	outb(SERIAL_MODEM_COMMAND_PORT(com), 0x0B);
}