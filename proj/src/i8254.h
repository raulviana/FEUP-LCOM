#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/** @defgroup i8254 i8254
 * @{
 *
 * Constantes para programar o Temporizador i8254.
 */

#define TIMER_FREQ 1193182 /**< @brief Frequência de relógio para o temporizador no PC e AT */
#define TIMER0_IRQ 0 /**< @brief Temporizador 0 IRQ line */

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Registo de contagem do Temporizador 0 */
#define TIMER_1    0x41 /**< @brief Registo de contagem do Temporizador 1 */
#define TIMER_2    0x42 /**< @brief Registo de contagem do Temporizador 2 */
#define TIMER_CTRL 0x43 /**< @brief Registo de Controlo */

#define SPEAKER_CTRL 0x61 /**< @brief Registo para controlo do emissor de som  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Palavra de controlo para o Temporizador 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Palavra de controlo para o Temporizador 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Palavra de controlo para o Temporizador 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Comando "Read Back"  */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Inicializar contador apenas Bit menos significativo */
#define TIMER_MSB     BIT(5)                  /**< @brief  Inicializar contador apenas Bit mais significativo */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief  Inicializar contador com Bit menos significativo seguido de Bit mais significativo */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Modo 3: Gerador de onda quadrada */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Modo 2: Gerator de taxa */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Contador em  BCD */
#define TIMER_BIN 0x00 /**< @brief Contador em Binário */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5) /**< @brief Bit do Read-Back Command para fazer leitura (ou não) do counter Value */
#define TIMER_RB_STATUS_ BIT(4) /**< @brief Bit do Read-Back Command para fazer leitura (ou não) do programmed Mode */
#define TIMER_RB_SEL(n)  BIT((n) + 1) /**< @brief Bit do Read-Back Command para escolher Timer */

/**@}*/

#endif /* _LCOM_I8254_H */
