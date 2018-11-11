#include <string.h>

void* memcpy(void* restrict dest, const void* restrict src, size_t size){
	unsigned char* dst = (unsigned char*) dest;
	const unsigned char* s = (unsigned char*)s;
	for(size_t i=0; i<size; i++){
		dst[i]=s[i];
	}
	return dest;
}