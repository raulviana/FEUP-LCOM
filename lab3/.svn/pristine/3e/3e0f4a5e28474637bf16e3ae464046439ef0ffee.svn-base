#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "testScan.h"
#include "keyboard.h"
#include "poll.h"
#include "i8042.h"


#define ESC_BREAK 0x81
extern uint8_t scanByte;
extern unsigned int counter;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  
  testScan();

  return 1;
}

int(kbd_test_poll)() {

  
  uint8_t size = 0; //número de bytes do scandoce
  bool wait = false;
  uint8_t poll_stat;
  uint32_t count = 0;
 // uint8_t discard;
  
   
  while (scanByte != ESC_BREAK){ //Get the scanCodes
    
    if(util_sys_inb(STAT_REG, &poll_stat)) return -1;

    if( (poll_stat & OBF)){
        if ( (poll_stat & (PAR_ERR | TO_ERR)) == 0 ){
          count++;
          util_sys_inb(OUT_BUF, &scanByte);
          isTwoByte(&wait, &size); //atualiza wait e size
          if(! wait) processCode(scanByte, size);
        }
   }
   delay();
  }

  kbd_print_no_sysinb(count); //Imprime o número de sys_inb calls


  if(! reenable_Ints(WR_CMD_BYTE, KBC_REENABLE_KEYBOARD)) return -1;

    
  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  
   uint8_t kbc_bit;
   uint8_t time_bit;
  
  unsigned int temporizador = 0;
   

  int ipc_status;
	message msg; 
  int r; //recebe o resultado de driver_receive

  uint8_t size = 0; //número de bytes do scandoce
  bool wait = false;

  kbc_subscribe_int(&kbc_bit);
  timer_subscribe_int(&time_bit);

  while ((scanByte != ESC_BREAK) && !(temporizador >= n)){ //Get the scanCodes
     /*Get a request message.*/
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /*hardware interrupt notification*/
             if (msg.m_notify.interrupts & BIT(kbc_bit)) { /*subscribed interrupt*/
              kbc_ih();
              isTwoByte(&wait, &size); //atualiza wait e size
              processCode(scanByte, size);
             counter = 0;
             temporizador = 0;
             }
             if(msg.m_notify.interrupts & BIT(time_bit)){
             timer_int_handler();
             printf("%d\n", temporizador);
      printf("%d\n",counter);
             if(counter == sys_hz()){
               counter = 0;
               temporizador++;
             }
           }
           
         
          break;
        default:
          break; /*no other notifications expected: do nothing*/
                }
      }
      else { /*received a standard message, not a notification*/
                /*no standard messages expected: do nothing*/
      }
     
    }

  kbc_unsubscribe_int();
  timer_unsubscribe_int();

  return 1;
}
