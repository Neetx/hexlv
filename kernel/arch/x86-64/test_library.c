#include "test_library.h"

void printf(char *str){
	unsigned short* VideoMemory = (unsigned short *) VIDEO_START;
	for(int i = 0; str[i] != '\0'; i++){
		VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
	}
}