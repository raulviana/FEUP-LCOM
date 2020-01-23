
#include <lcom/lcf.h>

#include "keyboard.h"

#include "i8042.h"





static int kbc_hook_id = 0x01;
extern uint8_t scanByte;


int kbc_subscribe_int(uint8_t *bit_no) {

	*bit_no = kbc_hook_id;

	return sys_irqsetpolicy(KEYBOARD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kbc_hook_id);
}


int kbc_unsubscribe_int(){

  return sys_irqrmpolicy(&kbc_hook_id);
}



/*
* KBC Interrrupt Handler
*/
void (kbc_ih)(){
  /*The handler should read the status from the status register, 
  * to check if there was some communications error, and to read 
  * the scancode byte from the output buffer.
  */

  uint8_t stat;
  uint8_t discard;

  if(! (util_sys_inb(STAT_REG, &stat))){
    /* loop while 8042 output buffer is empty */
    if( stat & OBF ) {
      util_sys_inb(OUT_BUF, &scanByte);
      return;
    }  
  }
  else{
    util_sys_inb(OUT_BUF, &discard); 
    return;
  }
}


/*
* Assembles the scanCode accordingly with the size
*/
int processCode(uint8_t byte, uint8_t size){
  
  uint8_t scanCode[size];

  if(size == 2){
    scanCode[0] = FIRST_BYTE;
    scanCode[1] = byte;
  }
  else scanCode[0] = byte;

  kbd_print_scancode(!((BIT(7) & byte) >> 7), size, scanCode);
  return 0;
}


/*
* Checks the scancode's length
*/
void isTwoByte(bool *wait, uint8_t *size){
  if(*wait == false){
    if(scanByte == FIRST_BYTE){
      *wait = true;
      return;
    }
    else{
      *size = 1;
      *wait = false;
    }
  }
  else{
    *size = 2;
    *wait = false;
  }
}


/*
* Implements the necessary delay, so KBC has enough time to processs
*/
void delay(){
  tickdelay(micros_to_ticks(DELAY_US));
}
void delay_poll(uint16_t period){
  tickdelay(micros_to_ticks(period * 1000));
}

int kbd_wrt_command(unsigned char wr_cm_byte, unsigned char kbc_reenable){
  
  uint8_t poll_stat;
  unsigned int tryCounter = 0; 
  // Writing the Command
    // Loop while the input buffer is full ; When it's not, it's safe to write the command
    while( 1 ) {
        // Read the status register
        util_sys_inb(STAT_REG, &poll_stat);
        
        if( !(poll_stat & IBF) == 0 ) {
            sys_outb(KBC_CMD_REG, wr_cm_byte); //KBC_WRITE_COMMAND
            break;
        } else {    
            tryCounter++;
        }

        // Check if the maximum number of tries to write in the input buffer was reached
        if (tryCounter == MAX_COUNT)
            break;

        delay();
    }

    // Reset the in-buff writing try counter
    tryCounter = 0;

    // Writing the Argument
    // Loop while the input buffer is full ; When it's not, it's safe to write the argument
    while( 1 ) {
        // Read the status register
        util_sys_inb(STAT_REG, &poll_stat);
        

        /* loop while 8042 input buffer is not empty */
        if( !((poll_stat & IBF) == 0) ) {
            sys_outb(KBC_IN_BUF, kbc_reenable);
        } else {    // Input Buffer not empty, increment the tryCounter
            tryCounter++;
        }

        // Check if the maximum number of tries to write in the input buffer was reached
        if (tryCounter == MAX_COUNT)
            return -1;

        delay();  // wait 20 ms
    }
    return 0;
}


