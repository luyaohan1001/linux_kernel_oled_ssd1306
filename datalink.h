/**
 * @file datalink.h
 * @brief Header file for SSD1306 controller interface.
 * @author Luyao Han (luyaohan1001@gmail.com)
 * @date 12-21-2022
 */

#ifndef DATALINK_H
#define DATALINK_H

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>

/* SSD1306 I2C command table. */
#define SET_MEMORY_ADDRESSING_MODE 0x20
#define SET_DISPLAY_START_LINE 0x40
#define SET_DISPLAY_OFF 0xAE
#define SET_DISPLAY_ON 0xAF
#define SET_ENTIRE_DISPLAY_ON 0xA4
#define SET_DISPLAY_OFFSET 0xD3
#define SET_MUX_RATIO 0xA8
#define SET_DEACTIVATE_SCROLL 0x2E
#define SET_CONTRAST_CONTROL 0x81
#define SET_CHARGE_PUMP 0x8D
#define SET_CHARGE_PUMP_ENABLE 0x14
#define SET_COLUMN_ADDRESS 0x21
#define SET_PAGE_ADDRESS 0x22

#define DONT_CARE 0x00

/**
 * @brief Enum type for SSD1306 function to differentiate whether
 * confirguration is a command type or a data byte.
 */
typedef enum { COMMAND_CONTROL, DATA_CONTROL } eControl_t;

/**
 * @brief Initialize SSD1306 OLED controller.
 * @param None.
 * @return None.
 */
void ssd1306_controller_init(void);

/**
 * @brief Write to SSD1306 register address.
 * @param control_option DATA_CONTROL indicates to transmit data,
 * COMMAND_CONTROL indicates to transmit command.
 * @param address The register address to write param to.
 * @param param_len Length of parameter if there is any.
 * @param p_param Pointer to parameter to be written.
 */
void ssd1306_write_address(eControl_t control_option, uint8_t address,
                           uint8_t param_len, uint8_t *param);
#endif /* DATALINK_H */