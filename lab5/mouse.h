#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>




  int mouse_subscribe_int(uint8_t *mouse_bit);

  int mouse_unsubscribe_int();

  int set_stream();

  int enable_data();
  int disable_data();

  int write_kbc(uint8_t cmd_byte);

  void (mouse_ih)();

  uint8_t mouse_wrt_command(uint8_t com);


  int assemblePacket();

  void print_packet(struct packet *pp);

  void print_gesture(struct packet *pp, uint8_t x_len, uint8_t tolerance);
  
  //void mouse_event_handler (struct packet *pp, uint8_t x_len, uint8_t tolerance)

  void mouse_state_machine(struct packet *pp, uint8_t x_len, uint8_t tolerance);

  void assemble_packet_pp(struct packet *pp);

  int enable_interrupts(uint8_t dflt_command);

#endif /* MOUSE_H */
