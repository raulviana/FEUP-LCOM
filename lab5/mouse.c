
#include <lcom/lcf.h>

#include "mouse.h"
#include "i8042.h"
#include "keyboard.h"

static int mouse_hook_id = 12;

extern uint8_t mouse_byte;
extern uint8_t scanByte;
extern uint8_t packetCounter;
extern uint8_t packet[3];
extern bool mouse_ih_error;

short int x_Total = 0;
short int y_Total = 0;
extern unsigned int state;


typedef enum {RDOW, RUP, MOVE} event_t;
typedef enum {INIT, STRDRAWUP, DRAWUP,TOP, STRDRAWDWN, DRAWDWN, END} state_t;
/*
INIT: Initial State
STRDRAWUP: Start Drawing Upward Line
DRAWUP: Drawing Upward Line
TOP: Reached the top of "inverted V"
STRDRAWDWN: Start Drawing Downward Line
DRAWDWN: Drawing Downward Line
END: End the Drawing
*/



int mouse_subscribe_int(uint8_t *mouse_bit){

	*mouse_bit = mouse_hook_id;

	if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &mouse_hook_id) != OK)	return -1; 

	return OK;
}

int mouse_unsubscribe_int(){
 
  if(sys_irqrmpolicy(&mouse_hook_id) != OK) return -1;
	return OK;
}

int set_stream(){
	if(write_kbc(STREAM_ON) == OK) return OK;
	return -1;
}

int enable_data(){
	unsigned int count = 0;
	while(count < MAX_TRIES){
		if(write_kbc(ENABLE_MOUSE) == OK) return OK;
		count++;
	}
	return -1;
}

int disable_data(){
	unsigned int count=0;
	while(count < MAX_TRIES){
		if(write_kbc(DISABLE_MOUSE) == OK) return OK;
		count++;
	}
	return -1;
}

int write_kbc(uint8_t cmd_byte){
	
	uint8_t status;
	bool done = FALSE;
	uint8_t verification;
	
	while(!done){
		if (util_sys_inb(STAT_REG, &status) != OK) return -1;

		if ((status & IBF) == 0) {
			sys_outb(KBC_CMD_REG, KBD_WRITE_TO_MOUSE); //prepares mouse for writing
		}
		if (util_sys_inb(STAT_REG, &status) != OK) return -1;

		if ((status & IBF) == 0) {
			sys_outb(KBC_IN_BUF,cmd_byte); //writes the command byte
		}
		util_sys_inb(OUT_BUF,&verification);
		if(verification == NACK){
			continue;
		}
		else if(verification == MOUSE_ERROR){
			return 1;
		}
		else{
			done= TRUE;
		}
	}

	return 0;
}

void (mouse_ih)(){
  /*The handler should read the status from the status register, 
  * to check if there was some communications error, and to read 
  * the scancode byte from the output buffer.
  */

  if(util_sys_inb(OUT_BUF, &scanByte) != OK){
		mouse_ih_error = TRUE;
		return;
	}
	packet[packetCounter] = scanByte;
	packetCounter++;

	if (!( (BIT(3) & packet[0]) >> 3 )){ //third bit of first mouse byte is always on
		mouse_ih_error = TRUE;
		return;
	}
	mouse_ih_error = FALSE;
	return;
}

 
int assemblePacket(){
	if(packetCounter == 0){
		if(!scanByte & BIT(3)) return -1;
		packet[0] = scanByte;
		packetCounter++;
		return MOUSE_INCOMPLETE;
	}
	else if(packetCounter == 1){
		packet[1] = scanByte;
		packetCounter++;
		return MOUSE_INCOMPLETE;
	}
	else{
		packet[2] = scanByte;
		packetCounter = 0;
		return MOUSE_COMPLETE;
	}
}


void print_packet(struct packet *pp){

	
	assemble_packet_pp(pp);

	mouse_print_packet(pp);
}

 void print_gesture(struct packet *pp, uint8_t x_len, uint8_t tolerance){
	 
	assemble_packet_pp(pp);
	mouse_state_machine(pp, x_len, tolerance);

	mouse_print_packet(pp);

 }


void assemble_packet_pp(struct packet *pp){

	pp->bytes[0] = packet[0];
	pp->bytes[1] = packet[1];
	pp->bytes[2] = packet[2];
	pp->delta_x = ((packet[0] & X_SIGN) >>4) | packet[1];
	pp->delta_y = ((packet[0] & Y_SIGN) >>5) | packet[2];
	// delta_x and delta_y values are computed in 2's complement
	if ((packet[0] & X_SIGN) == 0){
		pp->delta_x = 0x00FF & packet[1];
	}
	else{
		pp->delta_x = 0xFF00 | packet[1];
	}
	if ((packet[0] & Y_SIGN) == 0){
		pp->delta_y = 0x00FF & packet[2];
	}
	else{
		pp->delta_y = 0xFF00 | packet[2];
	}
	pp->lb = packet[0] & LEFT_BUTTON;
	pp->mb = (packet[0] & MIDLLE_BUTTON) >> 2;
	pp->rb = (packet[0] & RIGHT_BUTTON) >> 1;
	pp->x_ov = (packet[0] & X_OVERF) >> 6;
	pp->y_ov = (packet[0] & Y_OVERF) >> 7;

}



int enable_interrupts(uint8_t dflt_command){
  uint8_t stat;
  uint8_t tries = 0;

  while(tries < MAX_TRIES){
		if(util_sys_inb(STAT_REG, &stat) != OK) return -1;

		if((stat & IBF) == 0){
			if (sys_outb(KBC_CMD_REG, WR_CMD_BYTE) != OK) return -1;
			else{
				tries = 0;
				break;
			} 
		}
		tries++;
	}

	while(tries < MAX_TRIES){
		if(util_sys_inb(STAT_REG, &stat) != OK) return -1;

		if((stat & IBF) == 0){
			if(sys_outb(WR_CMD_BYTE, dflt_command) != OK) return -1;
			else return OK;
		}
		tries++;
	}
	return -1;
}

/*
void mouse_event_handler (struct packet *pp, uint8_t x_len, uint8_t tolerance){
	
	
	x_Total += pp->delta_x;
	y_Total += pp->delta_y;
	
	
	float slope= y_Total / x_Total;
	
	

	switch(state){
		case CASE_INIT:
		printf("INIT:");
			x_Total = 0;
			y_Total = 0;
			if(pp->lb) state = LB_PRESSED;
			break;

		case LB_PRESSED:
		printf("LPressed\n");
		printf("slope: %06.3f\n", slope );
		printf("xtotal: %d\n", x_Total);
		printf("y_total: %d\n", y_Total);
			if(pp->lb && slope >=1 && x_Total > x_len){
				state = LINE_UP;
			}
			else if(pp->lb && slope >=1 && x_Total <= x_len){
				break;
			}
			else state = CASE_INIT;
			break;
		
		case LINE_UP:
		printf("lineup");
			if(!pp->lb && pp->delta_x < tolerance && pp->delta_y < tolerance) state = APEX;
			else if(pp->lb && slope >=1 ) break;
			else state = CASE_INIT;
			break;

			case APEX:
			printf("apex;");
			if(pp->rb && pp->delta_x < tolerance && pp->delta_y < tolerance){
				x_Total = 0;
				y_Total = 0;
				state = LINE_DOWN;
			} 
			else if(!pp->rb && pp->delta_x <tolerance && pp->delta_y < tolerance) break;
			else state = CASE_INIT;

		case LINE_DOWN:
		printf("LINEDOWN");
				if(pp->rb && slope <= -1 && x_Total > x_len) state = STATE_OK;
				else if(pp->rb && slope <= -1 && x_Total < x_len) break;
				else state = CASE_INIT;
				break;
			
		case STATE_OK:
			printf("STATUsOK");
			if(!pp->rb) state = STOP;
			else if(pp->rb && slope <= -1 && x_Total > x_len)break;
			else state = CASE_INIT;
			break;
	}

}

void mouse_state_machine(struct packet *pp, uint8_t x_len, uint8_t tolerance){
	
	

}
	

