#include <string.h>

void* memset(void* buffer, int data, size_t size){
	unsigned char* buf = (unsigned char*) buffer;
	for(int i=0;i<size;i++){
		buf[i] = (unsigned char)data;
	}
	return buffer;
}