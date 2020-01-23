// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include <lcom/liblm.h>
#include <minix/type.h>


#include <stdint.h>
#include <stdio.h>


// Any header files included below this line should have been created by you

#include "i8042.h"
#include "keyboard.h"
#include "util.h"
#include "graphics.h"
#include <minix/type.h>


uint8_t scanByte;
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
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  
  vg_init(mode);

  sleep(delay);

  vg_exit();
    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
                        
  set_mode(mode);
   
  vg_init(mode);

	if (vg_draw_rectangle (x,y,width,height,color) != OK){
		printf("Error drawing rectangle\n");
		return 1;
	}


  uint8_t kbc_bit;
  int ipc_status;
	message msg; 
  int result; //recebe o resultado de driver_receive

  uint8_t size = 0; //número de bytes do scandoce
  bool wait = false;

  kbc_subscribe_int(&kbc_bit);

  while (scanByte != ESC_BREAK){ //Get the scanCodes
     /*Get a request message.*/
      if( (result = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", result);
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

  
  vg_exit();
  return 1;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  uint8_t BitsPerPixel;
  uint8_t RedScreenMask;
  uint8_t GreenScreenMask;
  uint8_t BlueScreenMask;
  uint16_t hres;
  uint16_t width_max_x;
  uint16_t width_max_y;
  uint16_t width_max;
  uint8_t Rpos;
  uint8_t Gpos;
  uint8_t Bpos;
  uint32_t color;
  uint32_t Rcolor;
  uint32_t Gcolor;
  uint32_t Bcolor;
  uint32_t Rfirst;
  uint32_t Gfirst;
  uint32_t Bfirst;
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t vres;
  vbe_mode_info_t v_info;

  set_mode(mode);

  vg_init(mode);
  
  vbe_get_info(mode, &v_info);
  
  BitsPerPixel = v_info.BitsPerPixel;
  RedScreenMask = v_info.RedMaskSize;
  GreenScreenMask = v_info.GreenMaskSize;
  BlueScreenMask = v_info.BlueMaskSize;
  hres = v_info.XResolution;
  vres = v_info.YResolution;
  Rpos= v_info.RedFieldPosition;
  Gpos= v_info.GreenFieldPosition;
  Bpos= v_info.BlueFieldPosition;

  Rfirst = (first >> Rpos) % (1 << RedScreenMask);
  Gfirst = (first >> Gpos) % (1 << GreenScreenMask);
  Bfirst = (first >> Bpos) % (1 << BlueScreenMask);

  width_max_x = hres / no_rectangles;
  width_max_y = vres / no_rectangles;

  if (width_max_x >= width_max_y){
    width_max = width_max_y;
  }else{
    width_max = width_max_x;
  }

  for(int row = 0; row<no_rectangles; row ++){
    x = 0;
    for (int col = 0; col < no_rectangles; col++){
      if (mode == MODE105){
          color = (first + (row * no_rectangles + col) * step) % (1 << BitsPerPixel);
        }
      else if (mode == MODE110 || mode == MODE115 || mode == MODE11A || mode == MODE14C){
          Rcolor = (Rfirst + col * step) % (1 << RedScreenMask);
          Gcolor = (Gfirst + row * step) % (1 << GreenScreenMask);
          Bcolor = (Bfirst + (col + row) * step) % (1 << BlueScreenMask);	
          color = (Rcolor << Rpos)|(Gcolor << Gpos)| (Bcolor << Bpos);
      }
      vg_draw_rectangle( x,  y,width_max_x,width_max_y, color);
      x += width_max_x;
    }
    y += width_max_y;
  }

  uint8_t kbc_bit;
  int ipc_status;
	message msg; 
  int result; //recebe o resultado de driver_receive

  uint8_t size = 0; //número de bytes do scandoce
  bool wait = false;

  kbc_subscribe_int(&kbc_bit);

  while (scanByte != ESC_BREAK){ //Get the scanCodes
     /*Get a request message.*/
      if( (result = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", result);
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

  
  vg_exit();
  return 0;
}




int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  

  draw_xpm( xpm, x, y);
  
  uint8_t kbc_bit;
  int ipc_status;
	message msg; 
  int result; //recebe o resultado de driver_receive

  uint8_t size = 0; //número de bytes do scandoce
  bool wait = false;

  kbc_subscribe_int(&kbc_bit);

  while (scanByte != ESC_BREAK){ //Get the scanCodes
     /*Get a request message.*/
      if( (result = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", result);
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

  vg_exit();

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {


  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
