
#include "testScan.h"
#include "keyboard.h"
#include "i8042.h"


unsigned int kbcCounter = 0;
uint8_t scanByte;

/*
* Main function to scan KBC
*/
void testScan(){

  uint8_t kbc_bit;
  int ipc_status;
	message msg; 
  int r; //recebe o resultado de driver_receive

  uint8_t size = 0; //número de bytes do scandoce
  bool wait = false;

  kbc_subscribe_int(&kbc_bit);

  while (scanByte != ESC_BREAK){ //Get the scanCodes
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
              if(!wait) processCode(scanByte, size);
            }
          break;
        default:
          break; /*no other notifications expected: do nothing*/
                }
      }
      else { /*received a standard message, not a notification*/
                /*no standard messages expected: do nothing*/
      }
      delay();
    }

  kbc_unsubscribe_int();

  kbd_print_no_sysinb(kbcCounter); //Imprime o número de interrupts 

}







