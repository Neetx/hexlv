#include <kernel/keyboard.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/scancodes.h>

//All codes are in my scancodes.h library

//read controller status
uint8_t kybrd_ctrl_read_status(){
  return inb(KYBRD_CTRL_STATS_REG);
}
//send command byte to controller
void kybrd_ctrl_send_cmd (uint8_t cmd) {
  //wait for controller input buffer to be clear
  while (1)
    if ( (kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
      break;
  outb(KYBRD_CTRL_CMD_REG, cmd);
}

//read encoder buffer
uint8_t kybrd_enc_read_buf () {
  return inb(KYBRD_ENC_INPUT_BUF);
}

//write command byte to encoder
void kybrd_enc_send_cmd (uint8_t cmd) {
  //wait for controller input buffer to be clear
  while (1)
    if ( (kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
      break;
  // send command byte to encoder
  outb(KYBRD_ENC_CMD_REG, cmd);
}

char getchar(){              
    uint8_t code = 0;
    uint8_t key = 0;
    while(1){
      if (kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_OUT_BUF) {
        code = kybrd_enc_read_buf ();
        if(code <= 0x58){ //is in range??
          key = _kkybrd_scancode_std [code];
          break;
        }
      }
    }
    return key;
}
void getline(char* string, int len){
uint8_t i=0;
  int flag = 0;
  char temp = 0;
  memset(string,0,len); //TODO: string=... with new memset
  while(i<len && temp != 0x0D){
    temp = getchar();
    if(isascii(temp) && temp != 0x0D){ //0x0D is ENTER
    	if(temp == 0x08){
    		terminal_column--;
        	terminal_putentryat(' ',terminal_color, terminal_column, terminal_row); //cancel last char*/
        	cursor--;
        	fb_move_cursor(cursor);
        	i--;
        	string[i] = 0x00;
    	}else{
    		terminal_putchar(temp);
    		string[i] = temp;
    		i++;
  		}
    }
  }
  terminal_putchar('\n');
  //string[i] = "0x0A"; //endline
}

// Leave here for now
char* itoa(int i, char b[]){          //From integer to ascii, thanks to SENDRAY
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

int strcmp(const char *str1, const char *str2){
  int res = 1;
  int i = 0;
  if(strlen(str1) == strlen(str2)){
    while(str1[i] != 0x0A && str2[i] != 0x0A){
      if(str1[i] != str2[i]){
        res = 0;
      }
      i++;
    }
  }else{res = 0;}
  return res;
}

bool isascii(int c){
  return c >= 0 && c<128;
}

void memset(char* buffer, char ch, int len){    //I really need this!!
  for(int i=0;i<len;i++){
    buffer[i]=ch;
  }
}