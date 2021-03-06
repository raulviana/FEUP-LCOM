#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

int kbc_subscribe_int();

int kbc_unsubscribe_int();

int processCode(uint8_t byte, uint8_t size);

void isTwoByte(bool *wait, uint8_t *size);

void delay();

int reenable_Ints(unsigned char wr_cm_byte, unsigned char kbc_reenable);

#endif /* KEYBOARD_H */
