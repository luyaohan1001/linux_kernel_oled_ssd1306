/**
 * @file graphics.h
 * @brief SSD1306 OLED graphics display APIs header.
 * @author Luyao Han (luyaohan1001@gmail.com)
 * @date 12-21-2022
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "datalink.h"

#define OLED_CANVAS_WIDTH_PIXELS 128
#define OLED_CANVAS_HEIGHT_PIXELS 64
#define BITS_PER_BYTE 8

#define OLED_COLUMN_LENGTH OLED_CANVAS_WIDTH_PIXELS
#define OLED_COLUMN_MIN 0
#define OLED_COLUMN_MAX OLED_CANVAS_WIDTH_PIXELS - 1

#define OLED_PAGE_LENGTH (OLED_CANVAS_HEIGHT_PIXELS / BITS_PER_BYTE)
#define OLED_PAGE_MIN 0
#define OLED_PAGE_MAX (OLED_CANVAS_HEIGHT_PIXELS / BITS_PER_BYTE) - 1

/**
 * @brief Struct that book-keeps parameters for the oled graphics.
 * @param line Current line (page) the cursor is on.
 * @param cursor_position Current position (column) the cursor is on.
 * @param font_char_width ASCII char width for estimation of potential colunm
 * overrun.
 */
typedef struct {
  uint8_t line;
  uint8_t cursor_position;
  uint8_t font_char_width;
} oled_graphics_params_t;

/**
 * @brief Print single char to the oled screen.
 * @param ascii_char ASCII character to put.
 * @return None.
 */
void oled_putc(unsigned char c);

/**
 * @brief printf on oled with variadic arguments to print on the oled screen.
 * @param format Format supplied including string and/or parameters.
 * @return None.
 */
void oled_printf(const char *format, ...);

/**
 * @brief Change to a new line on the OLED screen.
 * @param None.
 * @return None.
 */
void oled_new_line(void);

/**
 * @brief Set the cursor position, i.e. the start location to print.
 * @param line The vertical line (page) to set the cursor to.
 * @param position The horizontal position (column) to the set the cursor to.
 */
void oled_set_cursor(uint8_t lineNo, uint8_t cursorPos);

/**
 * @brief Fill the entire screen with byte pattern.
 * @param pattern Byte pattern to fill.
 * @return None.
 */
void oled_fill_all(uint8_t data);

/**
 * @brief Draw a dinosaur on the oled screen.
 * @param None.
 * @return None.
 */
void oled_draw_dino_map(void);

#endif /* GRAPHICS_H */