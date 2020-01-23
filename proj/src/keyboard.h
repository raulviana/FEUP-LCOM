#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

/** @defgroup keyboard keyboard
 * @{
 *
 * Funções para configuração do teclado
 */


/**
 * Função para subscrever às interrupções do teclado
 */
int kbc_subscribe_int();


/**
 * Função para cancelar a subscrição às interrupções do teclado
 */
int kbc_unsubscribe_int();


/**
 * "Interrupt Handler" para as interrupções do teclado
 * @return - código correspondente à tecla carregada ou levantada
 */
uint8_t *kbcIH();

/**
 * Formata o "scancode" de acordo com o tamanho
 * @param byte byte recebido na interrupção
 * @param size tamanho do scancode (1 byte ou 2)
 * @returns scancode final.
 */
uint8_t *processCode(uint8_t byte, uint8_t size);

/**
 * Verifica o tamanho de um scancode
 * @param wait variavel para verificar se se espera um ou dois bytes para o scancode
 * @param size tamanho do scancode (1 ou 2 bytes)
 */
void isTwoByte(bool *wait, uint8_t *size);

/**
 * Implementa o atraso necessário para que o KBC tenha tempo de processar
 */
void delay();


/**
 * Implementa o atraso necessário definido pela variavel period para que o KBC tenha tempo de processar
 * @param period multiplicador do tempo que será atrasado.
 */
void delay_poll(uint16_t period);

/**
 * Função para escrever um commando para o Teclado
 * @param wr_cm_byte instrução para escrever o commando
 * @param kbc_reenable comando a ser escrito
 */
int kbd_wrt_command(unsigned char wr_cm_byte, unsigned char kbc_reenable);


#endif /* KEYBOARD_H */
