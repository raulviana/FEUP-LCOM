// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>


// Any header files included below this line should have been created by you

#include "mouse.h"
#include "keyboard.h"
#include "i8042.h"

unsigned int kbcCounter = 0;
uint8_t scanByte;
uint8_t mouse_byte;
uint8_t packetCounter = 0;
uint8_t packet[3];
bool mouse_ih_error = FALSE;
unsigned int state = 0;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {

  uint8_t mouse_bit;
  int ipc_status;
	message msg; 
  int r; //recebe o resultado de driver_receive
  unsigned int count = 0;
  struct packet pp;


  if (set_stream() != OK){
    printf("Unable to set mouse stream mode\n");
    return -1;
  };

  if (enable_data() != OK){
    printf("Unable to enable mouse in less than %d tries", MAX_TRIES);
    return -1;
  };

  if (mouse_subscribe_int(&mouse_bit) != OK){
    printf("Unable to subscribe mouse interrupts");
    return -1;
  }


  while (cnt > count){ //Get the bytes
     /*Get a request message.*/
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /*hardware interrupt notification*/
            if (msg.m_notify.interrupts & BIT(mouse_bit)) { /*subscribed interrupt*/
              (mouse_ih());
              if(mouse_ih_error){ //error reading or first byte out of order
                packetCounter = 0;
                continue;
              }
              if(packetCounter == 3){ // packet OK
                count++;
                packetCounter = 0;
                print_packet(&pp);
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
      delay();
    }

  
  if(mouse_unsubscribe_int() != OK){
    printf("Unable to unsuscribe mouse interupts");
    return -1;
  };

  if (disable_data() != OK){
    printf("Unable to disable mouse in less than %d tries", MAX_TRIES);
    return 1;
  };

  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    
 struct packet pp;
  
  unsigned int count = 0;

  while (count < cnt){ //Get the scanCodes
    if(packetCounter == 0){
      write_kbc(READ_DATA);
    }
    mouse_ih();

    if(mouse_ih_error == TRUE){
      packetCounter = 0;
      continue;
    }

    if(packetCounter == 3){
      print_packet(&pp);
      packetCounter = 0;
      count++;
      delay_poll(period);
    }
  }

  if(set_stream(STREAM_ON) != OK){
    printf("Unable to set mouse stream mode\n");
    return -1;
  }
  
  if (disable_data() != OK){
    printf("Unable to disable mouse in less than %d tries", MAX_TRIES);
    return 1;
  }
  
  if(enable_interrupts(minix_get_dflt_kbc_cmd_byte()) != OK) return -1;

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  
  uint8_t mouse_bit;
  int ipc_status;
	message msg; 
  int r; //recebe o resultado de driver_receive
  unsigned int count = 0;
  struct packet pp;

  uint8_t bitn; 
  uint8_t temporizador;

  extern unsigned int counter;

  counter = 0;


  temporizador = 0;


  if (set_stream() != OK){
    printf("Unable to set mouse stream mode\n");
    return -1;
  };

  if (enable_data() != OK){
    printf("Unable to enable mouse in less than %d tries", MAX_TRIES);
    return -1;
  };

  if (mouse_subscribe_int(&mouse_bit) != OK){
    printf("Unable to subscribe mouse interrupts");
    return -1;
  }  //TODO error handling

  if (timer_subscribe_int(&bitn)){
    printf("Unable to subscribe to timer!");
    return 1;

  }


  while (temporizador < idle_time){ //Get the bytes
     /*Get a request message.*/
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", r);
          continue;
      }
      
      if (is_ipc_notify(ipc_status)) { /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /*hardware interrupt notification*/
            
            
            if (msg.m_notify.interrupts & BIT(mouse_bit)) { /*subscribed interrupt*/
              temporizador = 0;
              counter = 0;
              (mouse_ih());
              if(mouse_ih_error){ //error reading or first byte out of order
                packetCounter = 0;
                
              }
              if(packetCounter == 3){ // packet OK
                count++;
                packetCounter = 0;
                print_packet(&pp);
                }
                          
            } 
            
            
            if (msg.m_notify.interrupts & BIT(bitn)) { /*subscribed interrupt*/
                  timer_int_handler();
                  if (counter == sys_hz()){
                    counter=0;
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

    //TODO disable data stream
  
  if(mouse_unsubscribe_int() != OK){
    printf("Unable to unsuscribe mouse interupts");
    return -1;
  };

  if(timer_unsubscribe_int() != OK){
    printf("Unable to unsuscribe timer interupts");
    return -1;
  };


  if (disable_data() != OK){
    printf("Unable to disable mouse in less than %d tries", MAX_TRIES);
    return 1;
  };

  return 0;
  
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t mouse_bit;
  int ipc_status;
	message msg;
  int count = 0;
  int r; //recebe o resultado de driver_receive
  struct packet pp;

  if (set_stream() != OK){
    printf("Unable to set mouse stream mode\n");
    return -1;
  };

  if (enable_data() != OK){
    printf("Unable to enable mouse in less than %d tries", MAX_TRIES);
    return -1;
  };

  if (mouse_subscribe_int(&mouse_bit) != OK){
    printf("Unable to subscribe mouse interrupts");
    return -1;
  }


  while (state != STOP || count < 10){ //Get the bytes
     /*Get a request message.*/
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { /*received notification*/
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /*hardware interrupt notification*/
            if (msg.m_notify.interrupts & BIT(mouse_bit)) { /*subscribed interrupt*/
              (mouse_ih());
              if(mouse_ih_error){ //error reading or first byte out of order
                packetCounter = 0;
                continue;
              }
              if(packetCounter == 3){ // packet OK
                packetCounter = 0;
                count++;
                print_gesture(&pp, x_len, tolerance);
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
      delay();
    }

  
  if(mouse_unsubscribe_int() != OK){
    printf("Unable to unsuscribe mouse interupts");
    return -1;
  };

  if (disable_data() != OK){
    printf("Unable to disable mouse in less than %d tries", MAX_TRIES);
    return 1;
  };

  return 0;
}
