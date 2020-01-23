#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 keyboard. Needs to be completed.
 */

#define KEYBOARD1_IRQ 1   /**< @brief Keyboard 1 IRQ line */
#define MOUSE_IRQ 12      /**< @brief Mouse 12 IRQ line */


/* I/O port addresses */

#define STAT_REG 0x64     /**< @brief Status register */
#define KBC_CMD_REG 0x64  /**< @brief KBC command register*/

#define OUT_BUF 0x60      /**< @brief Output Buffer */

/* Important Scancodes*/

#define ESC_BREAK              0x81    /**< @brief Break code para a tecla ESC */
#define FIRST_BYTE             0xE0   /**< @brief Primeiro Byte em scancodes com dois bytes */

#define A_M_CODE               0x1e   /**< @brief Make code para a tecla A */
#define A_B_CODE               0x9e   /**< @brief Break code para a tecla A */
#define S_M_CODE               0x1f   /**< @brief Make code para a tecla S */
#define S_B_CODE               0x9f   /**< @brief Break code para a tecla S */
#define D_M_CODE               0x20   /**< @brief Make code para a tecla D */
#define D_B_CODE               0xa0   /**< @brief Break code para a tecla D */
#define W_M_CODE               0x11   /**< @brief Make code para a tecla W */
#define W_B_CODE               0x91   /**< @brief Break code para a tecla W */

/* KBC Commands for PC-AT/PS2 */

#define RD_CMD_BYTE            0x20 /**< @brief Ler Command Byte */
#define WR_CMD_BYTE            0x60 /**< @brief Escrever Command Byte */
#define CHK_KBC                0xAA /**< @brief Verificar KBC (Self test) */
#define CHK_KBN_INT            0xAB /**< @brief Verificar Interface do Teclado */
#define ENA_KBN_INT            0xAD /**< @brief Ativar Interface do Teclado */
#define DIS_KBN_INT            0xAE /**< @brief Desativar Interface do Teclado */

#define DELAY_US               20000 /**< @brief Atraso (Delay) de aproximadamente 0.2 segundos */


#define FIRST_BYTE             0xE0 /**< @brief Primeiro Byte num Scancode de dois bytes do Teclado */

#define PAR_ERR                BIT(7) /**< @brief Bit para verificar "Parity Error" */
#define TO_ERR                 BIT(6) /**< @brief Bit para verificar "Timeout Error" */
#define AUX                    BIT(5) /**< @brief Bit para verificar "Mouse Data" */
#define IBF                    BIT(1) /**< @brief Bit para verificar se o "Input Buffer" está cheio. */
#define OBF                    BIT(0) /**< @brief Bit para verificar se o "Output Buffer" está cheio. */



#define MAX_COUNT               10 /**< @brief Número Máximo de Tentativas para escrever no "Input Buffer" */

#define KBC_REENABLE_KEYBOARD   0x47 /**< @brief Comando KBC para reativar Teclado */
#define KBC_CMD_REG             0x64 /**< @brief "Command Register" do KBC */
#define KBC_IN_BUF              0x60 /**< @brief "Input Buffer" do KBC */



/****************MOUSE******************/

#define ACK                       0xFA    /**< @brief "Acknowlegdement" Byte para dizer que está tudo bem*/
#define NACK                      0xFE    //**< @brief "Acknowlegdement" Byte para dizer que algo foi recebido mas correu mal*/
#define MOUSE_ERROR               0xFC    /**< @brief Segundo Byte inválido consecutivo*/

#define STREAM_ON                 0xEA  /**< @brief Mouse: Ativar "Stream Mode"*/
#define KBD_WRITE_TO_MOUSE        0xD4 /**< @brief Commando para escrever Byte para o Mouse*/
#define ENABLE_MOUSE              0xF4 /**< @brief Mouse: Ativar "Data Reporting"*/
#define DISABLE_MOUSE             0xF5 /**< @brief Mouse: Desativar "Data Reporting"*/
#define READ_DATA                 0xEB /**< @brief Mouse: Ler Dados*/

#define MAX_TRIES                 3  /**< @brief Número Máximo de Tentativas para o ativar ou desativar algo no Mouse */

#define MOUSE_ACK                   0xFA/**< @brief "Acknowlegdement" Byte para dizer que está tudo bem do Mouse*/
#define MOUSE_NACK                  0xFE /**< @brief "Acknowlegdement" Byte para dizer que algo foi recebido mas correu mal do Mouse*/
#define MOUSE_ERROR                 0xFC /**< @brief Byte que ocorreu um erro no Mouse*/

#define MOUSE_COMPLETE              0 /**< @brief Valor que indicar que o Mouse tem o packet completo com todos os Bytes*/
#define MOUSE_INCOMPLETE            2 /**< @brief Valor que indicar que o Mouse não tem o packet completo com todos os Bytes*/

#define LEFT_BUTTON                 BIT(0) /**< @brief Bit para verificar se o botão esquerdo do Rato foi pressionado */
#define RIGHT_BUTTON                BIT(1) /**< @brief Bit para verificar se o botão direito do Rato foi pressionado */
#define MIDLLE_BUTTON               BIT(2) /**< @brief Bit para verificar se o botão central do Rato foi pressionado */
#define X_SIGN                      BIT(4) /**< @brief Bit para verificar houve variação do movimento do rato no eixo dos X */
#define Y_SIGN                      BIT(5) /**< @brief Bit para verificar houve variação do movimento do rato no eixo dos Y */
#define X_OVERF                     BIT(6) /**< @brief Bit para verificar houve "overflow" do movimento do rato no eixo dos X */
#define Y_OVERF                     BIT(7) /**< @brief Bit para verificar houve "overflow" do movimento do rato no eixo dos Y */



#endif /* _LCOM_I8042_H */
