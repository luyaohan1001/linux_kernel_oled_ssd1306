/**
 * @file datalink.c
 * @brief Datalink layer implementation for SSD1306 OLED Driver, SMBus-based
 * operations.
 */

#include "datalink.h"
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

#define DUMMY_DATA 0x00

#define OLED_CANVAS_WIDTH 128
#define OLED_CANVAS_HEIGHT 64
#define BYTE_WIDTH 8

#define OLED_SOLID_PIXELS 0xFF

/* Global symbol to the i2c client instance. */
extern struct i2c_client* i2c_client;

/**
 * @brief Configure SSD1306 OLED controller.
 * @param control_option Specify whether the control byte indicates a COMMAND or
 * a DATA.
 * @param address Specify the register address of the configuration command.
 * @param param_option Specify whether the configuration has a parameter.
 * @param param The parameter if the configuration do have a parameter.
 * @return None.
 * @note The configuration is done by writing parameters to the registers addresses through I2C bus.
 */
void ssd1306_configure(eControl_t control_option, uint8_t address,
                       eParam_t param_option, uint8_t param) {
  ssd1306_packet_t packet = {
      .control_byte0 =
          (control_option == DATA_CONTROL) ? SET_DISPLAY_START_LINE : 0x00,
      .command = address,
      .control_byte1 = 0x00,
      .param = param,
  };

  if (param_option == HAS_PARAM) {
    /* Needs to transmit all 4 bytes. */
    i2c_master_send(i2c_client, (uint8_t*)&packet, 4);
  } else if (param_option == NO_PARAM) {
    /* Needs to transmit only first 2 bytes. */
    i2c_master_send(i2c_client, (uint8_t*)&packet, 2);
  }
}

/**
 * @brief Initialize ssd1306 OLED controller.
 * @param None.
 * @return None.
 */
void ssd1306_controller_init(void) {
  ssd1306_configure(COMMAND_CONTROL, SET_DISPLAY_OFF, NO_PARAM, DUMMY_DATA);

  ssd1306_configure(COMMAND_CONTROL, SET_DISPLAY_OFFSET, HAS_PARAM, 0x00);

  ssd1306_configure(COMMAND_CONTROL, SET_DISPLAY_START_LINE, NO_PARAM,
                    DUMMY_DATA);

  ssd1306_configure(COMMAND_CONTROL, SET_CHARGE_PUMP, NO_PARAM, DUMMY_DATA);

  ssd1306_configure(COMMAND_CONTROL, SET_CHARGE_PUMP_ENABLE, NO_PARAM,
                    DUMMY_DATA);

  ssd1306_configure(COMMAND_CONTROL, SET_MEMORY_ADDRESSING_MODE, HAS_PARAM,
                    0x00);

  ssd1306_configure(COMMAND_CONTROL, SET_CONTRAST_CONTROL, HAS_PARAM, 0x80);

  ssd1306_configure(COMMAND_CONTROL, SET_ENTIRE_DISPLAY_ON, NO_PARAM,
                    DUMMY_DATA);

  ssd1306_configure(COMMAND_CONTROL, SET_DISPLAY_ON, NO_PARAM, DUMMY_DATA);

  ssd1306_configure(COMMAND_CONTROL, SET_DEACTIVATE_SCROLL, NO_PARAM,
                    DUMMY_DATA);

  ssd1306_configure(COMMAND_CONTROL, SET_DISPLAY_ON, NO_PARAM, DUMMY_DATA);
}

/**
 * @brief Fill the entire screen.
 * @param None.
 * @return None.
 */
void ssd1306_fill_all(void) {
  volatile uint32_t pixel = 0;
  static const int TOTAL_PIXELS =
      OLED_CANVAS_WIDTH * (OLED_CANVAS_HEIGHT / BYTE_WIDTH);

  for (pixel = 0; pixel < TOTAL_PIXELS; ++pixel) {
    ssd1306_configure(DATA_CONTROL, OLED_SOLID_PIXELS, NO_PARAM, DUMMY_DATA);
  }
}