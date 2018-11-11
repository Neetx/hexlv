#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/io.h>
#include <kernel/vga.h>
#include <string.h>

void terminal_initialize(void) 
{
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = (uint16_t*) VGA_MEMORY;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) 
{
  terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}

void terminal_clear_line(size_t y)   //clear gived line
{ 
  size_t x = 0;
  while(x < VGA_WIDTH){
    terminal_putentryat(' ', terminal_color, x, y);
    x++;
  }
}

void terminal_clearscreen(void)    //clear all screen and set prompt to up left corner
{
  size_t y = 0;
  while(y < VGA_HEIGHT){
    terminal_clear_line(y);
    y++;
  }
  terminal_row = 0;
  terminal_column = 0;
  cursor = 0;
}
 
void terminal_scroll(void)         //scroll losing the first line written, need history? Have fun! :)
{ 
  for (size_t y = 0; y < VGA_HEIGHT; y++){
    for (size_t x = 0; x < VGA_WIDTH; x++){
      const size_t index = y * VGA_WIDTH + x;
      const size_t next_index = (y + 1) * VGA_WIDTH + x;
      terminal_buffer[index] = terminal_buffer[next_index];
    }
  }
  terminal_clear_line(VGA_HEIGHT-1);
  terminal_row = VGA_HEIGHT-1;
  terminal_column = 0;
}

void terminal_putchar(char c)     //putchar with integrated scroll, NOT optimized
{
  if(c == '\n'){
    terminal_column = 0;
    cursor -= cursor % VGA_WIDTH;
    cursor += VGA_WIDTH;
    fb_move_cursor(cursor);
    if(terminal_row == VGA_HEIGHT-1){
      terminal_scroll();
    }else {
      terminal_row++;
    }

  }else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    cursor++; //TODO: pay attention to scroll
    fb_move_cursor(cursor);
    if (++terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      if (++terminal_row == VGA_HEIGHT)
        terminal_row = 0;
    }
  }
}
 
void terminal_write(const char* data, size_t size) 
{
  for (size_t i = 0; i < size; i++){
    terminal_putchar(data[i]);
  }
}
 
void terminal_writestring(const char* data) 
{
  terminal_write(data, strlen(data));
}

void fb_move_cursor(unsigned short pos){
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT, ((pos >> 8 ) & 0x00FF));
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT, pos & 0x00FF);
}