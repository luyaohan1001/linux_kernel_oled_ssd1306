/**
 * @file datalink.c
 * @brief Datalink layer implementation for SSD1306 OLED Driver, I2C-based
 * operations.
 * @author Luyao Han (luyaohan1001@gmail.com)
 * @date 12-21-2022
 */
#include "datalink.h"

#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>

/**
 * @brief Pointer to the i2c_client instance.
 * @note Original symbol declared in driver.c.
 */
extern struct i2c_client *i2c_client;

/**
 * @brief Write to SSD1306 register address.
 * @param control_option DATA_CONTROL indicates to transmit data,
 * COMMAND_CONTROL indicates to transmit command.
 * @param address The register address to write param to.
 * @param param_len Length of parameter if there is any.
 * @param p_param Pointer to parameter to be written.
 */
void ssd1306_write_address(eControl_t control_option, uint8_t address,
                           uint8_t param_len, uint8_t *p_param) {
  uint8_t control_byte = 0;
  uint8_t repeat = 0;
  uint8_t packet[2];

  /* Differentiate COMMAND versus DATA control. */
  if (control_option == DATA_CONTROL) {
    control_byte = SET_DISPLAY_START_LINE;
  } else if (control_option == COMMAND_CONTROL) {
    memcpy(packet, (uint8_t[]){control_byte, address}, 2);
    i2c_master_send(i2c_client, (uint8_t[]){control_byte, address}, 2);
  }

  /* NULL pointer check. */
  if (param_len <= 0 || NULL == p_param) {
    goto EXIT;
  }

  /* Transmit the packet. */
  for (repeat = 0; repeat < param_len; ++repeat) {
    memcpy(packet, (uint8_t[]){control_byte, p_param[repeat]}, 2);
    i2c_master_send(i2c_client, packet, 2);
  }

EXIT:
  return;
}

/**
 * @brief Initialize SSD1306 OLED controller.
 * @param None.
 * @return None.
 * @note Using anonymous array to pass single parameters.
 */
void ssd1306_controller_init(void) {
  ssd1306_write_address(COMMAND_CONTROL, SET_DISPLAY_OFF, 0, NULL);

  ssd1306_write_address(COMMAND_CONTROL, SET_DISPLAY_OFFSET, 1,
                        (uint8_t[]){0x00});

  ssd1306_write_address(COMMAND_CONTROL, SET_DISPLAY_START_LINE, 0, NULL);

  ssd1306_write_address(COMMAND_CONTROL, SET_CHARGE_PUMP, 0, NULL);

  ssd1306_write_address(COMMAND_CONTROL, SET_CHARGE_PUMP_ENABLE, 0, NULL);

  ssd1306_write_address(COMMAND_CONTROL, SET_MEMORY_ADDRESSING_MODE, 1,
                        (uint8_t[]){0x00});

  ssd1306_write_address(COMMAND_CONTROL, SET_CONTRAST_CONTROL, 1,
                        (uint8_t[]){0x80});

  ssd1306_write_address(COMMAND_CONTROL, SET_ENTIRE_DISPLAY_ON, 0, NULL);

  ssd1306_write_address(COMMAND_CONTROL, SET_DISPLAY_ON, 0, NULL);

  ssd1306_write_address(COMMAND_CONTROL, SET_DEACTIVATE_SCROLL, 0, NULL);

  ssd1306_write_address(COMMAND_CONTROL, SET_DISPLAY_ON, 0, NULL);
}
