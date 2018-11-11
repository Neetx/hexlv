#include <string.h>

int strcmp(const char *str1, const char *str2){
  int res = 1;
  int i = 0;
  if(strlen(str1) == strlen(str2)){
    while(str1[i] != 0x0A && str2[i] != 0x0A){  //TODO: test and fix this (old endstring version)
      if(str1[i] != str2[i]){
        res = 0;
      }
      i++;
    }
  }else{res = 0;}
  return res;
}