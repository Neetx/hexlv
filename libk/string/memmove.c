#include <string.h>

void* memmove(void* dest, const void* src, size_t size){
	unsigned char* dst = (unsigned char*)dest;
	const unsigned char* s = (const unsigned char*) src;
	if (dst < s){
		for(size_t i=0; i<size; i++){
			dst[i]=s[i];
		}
	}else{
		for(size_t i=size; i!=0; i--){
			dst[i-1] = s[i-1];
		}
	}
	return dest;
}