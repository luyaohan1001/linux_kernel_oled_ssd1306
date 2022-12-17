/**
 * @file datalink.h
 * @brief Header file for SSD1306 controller interface.
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/delay.h>

/**
 * @brief SSD1306 I2C/SMBus standard packet format.
*/
typedef struct {
    uint8_t control_byte0;
    uint8_t command;
    uint8_t control_byte1;
    uint8_t param;
} ssd1306_packet_t;

/**
 * @brief Enum type for ssd1306 function to differentiate whether confirguration takes in parameters.
*/
typedef enum {
    HAS_PARAM,
    NO_PARAM,
} eParam_t;

/**
 * @brief Enum type for ssd1306 function to differentiate whether confirguration is a command type or a data byte.
*/
typedef enum {
    COMMAND_CONTROL,
    DATA_CONTROL
} eControl_t;


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
void ssd1306_configure(eControl_t control_option, uint8_t address, eParam_t param_option, uint8_t param);

/**
 * @brief Fill the entire screen.
 * @param None.
 * @return None.
 */
void ssd1306_fill_all(void);

/**
 * @brief Initialize ssd1306 OLED controller.
 * @param None.
 * @return None.
 */
void ssd1306_controller_init(void);
