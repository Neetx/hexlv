#include <kernel/tty.h>

#if defined(__cplusplus)
    extern "C" /* Use C*/
#endif

void kernel_main(void * multiboot_structure, unsigned int magicnumber){
    terminal_initialize();
    terminal_writestring("Hello world x86-64, long mode!");
}