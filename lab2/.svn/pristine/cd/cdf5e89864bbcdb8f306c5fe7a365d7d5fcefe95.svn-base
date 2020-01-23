#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


unsigned int counter = 0;
static int hook_id = TIMER0_IRQ;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer<0 || timer>2)		// timer out of range.
		return -1;

	if (freq < 19 || freq > TIMER_FREQ)	return -1; //freq out of range

	uint8_t st;
	if(timer_get_conf(timer, &st) != OK)
		return -1;


	uint8_t countingMode = st & (BIT(3) | BIT(2) | BIT(1));
	uint8_t bcd = st & BIT(0);
	
	uint8_t selectedCounter;

	switch(timer){
	case 0:
		selectedCounter = TIMER_0;
		break;
	case 1:
		selectedCounter = TIMER_1;
		break;
	case 2:
		selectedCounter = TIMER_2;
		break;
	}

	// write the control word
	uint8_t controlWord = (selectedCounter | TIMER_LSB_MSB | countingMode | bcd);

	if(sys_outb(TIMER_CTRL, controlWord) != OK)
		return -1;

	//Calculate div freq and lsb and msb
	uint16_t div = (uint16_t) TIMER_FREQ / freq;
	uint8_t msbDiv;
	util_get_MSB(div, &msbDiv);
	uint8_t lsbDiv;
	util_get_LSB(div, &lsbDiv) ;

	//write the value to the counter
	switch(timer){
		case 0:
			if(sys_outb(TIMER_0, lsbDiv) != OK) return -1;
			if(sys_outb(TIMER_0, msbDiv) != OK) return -1;
			return 0;
		case 1:
			if(sys_outb(TIMER_1, lsbDiv) != OK) return -1;
			if(sys_outb(TIMER_1, msbDiv) != OK) return -1;
			return 0;
		case 2:
			if(sys_outb(TIMER_2, lsbDiv) != OK) return -1;
			if(sys_outb(TIMER_2, msbDiv) != OK) return -1;
			return 0;
	}

	return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

	*bit_no = hook_id;

	return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);

}

int (timer_unsubscribe_int)() {
 
return sys_irqrmpolicy(&hook_id);

}


unsigned int counter;

void (timer_int_handler)() {
 counter ++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if(timer < 0 || timer > 2) return -1;

	uint8_t readCommand;
	readCommand = TIMER_RB_CMD | 
								TIMER_RB_COUNT_ |
								TIMER_RB_SEL(timer);

	uint8_t tempIn = 0;

	int outRes = sys_outb(TIMER_CTRL, readCommand);
	if(outRes != OK) return -1;
	else{
		switch(timer){
			case 0:

				if(util_sys_inb(TIMER_0, &tempIn) != OK) {
					printf("sys_inb failed in timer_get_conf Timer_0");
				return -1;
				}
				break;
			case 1:
				if(util_sys_inb(TIMER_1, &tempIn) != OK) {
					printf("sys_inb failed in timer_get_conf Timer_1");
				return -1;
				}
				break;
			case 2:
				if(util_sys_inb(TIMER_2, &tempIn) != OK) {
					printf("sys_inb failed in timer_get_conf Timer_2");
				return -1;
				}
				break;
			default:
				printf("Error in get_conf");
				return -1;
				break;
		}

		*st = tempIn;
	}
return OK;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

	union timer_status_field_val config;

	uint8_t temp;

	//status byte
	if(field == tsf_all){
		config.byte = st;
	}

	//init_mode 
	else if(field == tsf_initial){
		temp = (st & (BIT(5) | BIT(4))) >> 4;
		if(temp == 3) config.in_mode = temp;            //MSB_after_LSB;
		else if(temp == 2) config.in_mode = temp;       //MSB_only;
		else if (temp == 1)  config.in_mode = temp;     //LSB_only;
		else config.in_mode = 0;                     //INVAL_val;
	}

	//BCD
	else if(field == tsf_base){
		temp = st & BIT(0);
		if(temp == 1) config.bcd = true;
		else config.bcd = false;
	}
	
	//count_mode
	else if(field == tsf_mode){
		temp = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
		if(temp == 0) config.count_mode = temp;
		else if(temp == 1)  config.count_mode = temp;
		else if(temp == 2 || temp == 6) config.count_mode = 2;
		else if(temp == 3 || temp == 7) config.count_mode = 3;
		else if(temp == 4) config.count_mode = 4;
		else if (temp == 5) config.count_mode = 5;
		else config.count_mode = 10;
	}

timer_print_config( timer,  field,  config);

return 0;
}
