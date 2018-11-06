#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/keyboard.h>

#if defined(__cplusplus)
    extern "C" /* Use C*/
#endif

void kernel_main(void * multiboot_structure, unsigned int magicnumber){
    
    terminal_initialize();
    terminal_writestring("Hello world x86-64, long mode!\n");
    
    init_serial(SERIAL_COM1_BASE);
    /*serial_writeline(SERIAL_COM1_BASE, "START SERIAL CONNECTION\n");
    
    char input2[30];
    int readed = serial_read(SERIAL_COM1_BASE, input2, 30);
    terminal_writestring(input2);

    terminal_writestring("\n");
    serial_writeline(SERIAL_COM1_BASE, "\nCONNECTION FINISHED\n");
    terminal_writestring("END\n");*/

    //keyboard example
    char string[10];
    getline(string, 10);
    terminal_writestring(string);
}