#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  uint8_t temp;
  temp = (uint8_t) val;
  *lsb = temp; 
  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  *msb = val & 0xff00;

  return 1;
}

int (util_sys_inb)(int port, uint8_t *value) {
  
  uint32_t tempValue;

  int res = sys_inb(port, &tempValue);

  *value = (uint8_t) tempValue;


  return res;
}
