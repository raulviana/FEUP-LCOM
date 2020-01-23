#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>

/** @defgroup mouse mouse
 * @{
 *
 * Funções para configuração do rato
 */

#define LARGEST_NUMBER 0xFF

/**
 * Função para subscrever às interrupções do rato
 * @param mouse_bit Bit do rato
 * @returns 0 para caso de sucesso
 */
  int mouse_subscribe_int(uint8_t *mouse_bit);

/**
 * Função para cancelar a subscrição às interrupções do rato
 * @returns 0 para caso de sucesso
 */
  int mouse_unsubscribe_int();

/**
 * Função ativar modo "stream"
 * @returns 0 para caso de sucesso
 */
  int set_stream();

/**
 * Função ativar rato
 * @returns 0 para caso de sucesso
 */
  int enable_data();

/**
 * Função desativar rato
 * @returns 0 para caso de sucesso
 */
  int disable_data();

/**
 * Função escrever command Byte
 * @param cmd_byte commmand byte a ser escrito
 * @returns 0 para caso de sucesso
 */
  int write_kbc(uint8_t cmd_byte);

/**
 * "Interrupt handler" para o Rato
 * @param pp "Packet" do Rato
 * @param ready variavel para saber se está pronto
 */
  void mouseIH(struct packet *pp, bool *ready);

  uint8_t mouse_wrt_command(uint8_t com);

/**
 * Função construir um "Packet" de um Rato
 * @returns 0 se o packet estiver completo e 2 se não estiver completo
 */
  int assemblePacket();

/**
 * Função para mostrar no ecrã o conteúdo de um "packet" de rato
 * @param pp "packet" a ser escrito
 */
  void print_packet(struct packet *pp);

  // void print_gesture(struct packet *pp, uint8_t x_len, uint8_t tolerance);
  
  //void mouse_event_handler (struct packet *pp, uint8_t x_len, uint8_t tolerance)

  //void mouse_state_machine(struct packet *pp, uint8_t x_len, uint8_t tolerance);

/**
 * Função para construir o conteúdo de um "packet" de rato
 * @param pp "packet" a ser construido
 */
  void assemble_packet_pp(struct packet *pp);

/**
 * Função permitir interrupções do rato
 * @param dflt_command Commando para permitir interrupções
 */
  int enable_interrupts(uint8_t dflt_command);

#endif /* MOUSE_H */
