#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint8_t kybrd_ctrl_read_status();
void kybrd_ctrl_send_cmd (uint8_t cmd);
uint8_t kybrd_enc_read_buf ();
void kybrd_enc_send_cmd (uint8_t cmd);
char getchar();
void getline(char* string, int len);

// Leave here for now
bool isascii(int c);
int strcmp(const char *str1, const char *str2);
char* itoa(int i, char b[]);
void memset(char* buffer, char ch, int len);

#endif