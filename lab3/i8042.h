#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 keyboard. Needs to be completed.
 */

#define KEYBOARD1_IRQ 1   /**< @brief Keyboard 1 IRQ line */

/* I/O port addresses */


#define STAT_REG 0x64     /**< @brief Status register */
#define KBC_CMD_REG 0x64  /**< @brief KBC command register*/

#define OUT_BUF 0x60      /**< @brief Output Buffer */



/* Important Scancodes*/

#define ESC_BREAK 0x81    /**< @brief Break code for the esc key */
#define FIRST_BYTE 0xE0   /**< @brief First byte in scancodes with two bytes */

/* KBC Commands for PC-AT/PS2 */

#define RD_CMD_BYTE 0x20 /**< @brief Read Command Byte */
#define WR_CMD_BYTE 0x60 /**< @brief Write Command Byte */
#define CHK_KBC 0xAA /**< @brief Check KBC (Self test) */
#define CHK_KBN_INT 0xAB /**< @brief Check Keyboard Interface */
#define ENA_KBN_INT 0xAD /**< @brief Enable Keyboard Interface */
#define DIS_KBN_INT 0xAE /**< @brief Disable Keyboard Interface */



#define DELAY_US 20000   //Delay time aprox. 0.2 seconds


#define FIRST_BYTE 0xE0

#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define AUX BIT(5)
#define IBF  BIT(1)
#define OBF  BIT(0)



#define FIRST_BYTE 0xE0


#define MAX_COUNT 10

#define KBC_REENABLE_KEYBOARD 0x47
#define KBC_CMD_REG 0x64
#define KBC_IN_BUF 0x60


#endif /* _LCOM_I8042_H */
