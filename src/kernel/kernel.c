#include "test_library.h"

#if defined(__cplusplus)
	extern "C" /* Use C*/
#endif
void kernel_main(void * multiboot_structure, unsigned int magicnumber){

  printf("Hello World x86-64, long mode!");
  while(1);
}