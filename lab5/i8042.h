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

#define ESC_BREAK              0x81    /**< @brief Break code for the esc key */
#define FIRST_BYTE             0xE0   /**< @brief First byte in scancodes with two bytes */

/* KBC Commands for PC-AT/PS2 */

#define RD_CMD_BYTE            0x20 /**< @brief Read Command Byte */
#define WR_CMD_BYTE            0x60 /**< @brief Write Command Byte */
#define CHK_KBC                0xAA /**< @brief Check KBC (Self test) */
#define CHK_KBN_INT            0xAB /**< @brief Check Keyboard Interface */
#define ENA_KBN_INT            0xAD /**< @brief Enable Keyboard Interface */
#define DIS_KBN_INT            0xAE /**< @brief Disable Keyboard Interface */

#define DELAY_US               20000   //Delay time aprox. 0.2 seconds


#define FIRST_BYTE             0xE0

#define PAR_ERR                BIT(7)
#define TO_ERR                 BIT(6)
#define AUX                    BIT(5)
#define IBF                    BIT(1)
#define OBF                    BIT(0)

#define FIRST_BYTE              0xE0


#define MAX_COUNT               10

#define KBC_REENABLE_KEYBOARD   0x47
#define KBC_CMD_REG             0x64
#define KBC_IN_BUF              0x60



/****************MOUSE******************/

#define ACK                       0xFA    //if everything OK
#define NACK                      0xFE    //if invalid byte (may be because of a serial communication error)
#define MOUSE_ERROR               0xFC   //second consecutive invalid byte

#define STREAM_ON                 0xEA  
#define KBD_WRITE_TO_MOUSE        0xD4
#define ENABLE_MOUSE              0xF4
#define DISABLE_MOUSE             0xF5
#define READ_DATA                 0xEB

#define MAX_TRIES                 3 

#define MOUSE_ACK                   0xFA
#define MOUSE_NACK                  0xFE
#define MOUSE_ERROR                 0xFC

#define MOUSE_COMPLETE              0
#define MOUSE_INCOMPLETE            2

#define LEFT_BUTTON                 BIT(0)
#define RIGHT_BUTTON                BIT(1)
#define MIDLLE_BUTTON               BIT(2)
#define X_SIGN                      BIT(4)
#define Y_SIGN                      BIT(5)
#define X_OVERF                     BIT(6)
#define Y_OVERF                     BIT(7)

#define CASE_INIT                   -1
#define LB_PRESSED                  0
#define LINE_UP                     1
#define APEX                        2
#define RB_PERSSED                  3
#define LINE_DOWN                   4
#define STATE_OK                    5
#define STOP                        6



#endif /* _LCOM_I8042_H */
