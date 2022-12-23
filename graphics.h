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

#define DEFAULT_TEXT_LENGTH 256

/**
 * @struct Pixel location on the screen.
 * @param line The horizontal line (page).
 * @param position The vertical position (column).
 * @note Page and columns are formal terms defined in SSD1306 datasheet, Figure
 * 10-3. In this program we have renamed them respectively line and
 * position as more intuitive names. The screen is 128x64 pixels.
 * However, since data are written in slices (one byte) at a time, there are 64
 * / 8 = 8 lines.
 *
 * Visualize:
 *                    ------- 128 positions / columns --------
 *                    |||||||| |||||||| ||||....... |||||||| ||||||||
 *                  -
 *                  -
 *                  -
 *  8 lines/pages   -
 *                  -
 *                  -
 *                  -
 *                  -
 */
typedef struct {
  uint8_t line;     /* Valid range 0 - 7 */
  uint8_t position; /* Valid range 0 - 127 */
} oled_cursor_coordinate_t;

/**
 * @brief Struct used to book-keep parameters for the oled graphics.
 * @param cursor_coordinate Keeps track of the coordinate of current cursor.
 * @param display_text Buffers/keeps track of the current text on the
 * oled_screen.
 */
typedef struct {
  oled_cursor_coordinate_t cursor_coordinate;
  char display_text[DEFAULT_TEXT_LENGTH];
} oled_graphics_params_t;

/**
 * @brief Enum type defining options when printing to a new line (page).
 * @param START_OF_NEW_LINE For example, when printing a sentence, it is
 * expected to be printed to the start of the new line.
 * @param SAME_CURSOR_POSITION For exapmle, when printing an image, it is
 * expected to be printed to the same cursor position.
 */
typedef enum { START_OF_NEW_LINE, SAME_CURSOR_POSITION } oled_new_line_options;

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
 * @param oled_new_line_options
 * START_OF_NEW_LINE to print to the start of the new line.
 * SAME_CURSOR_POSITION to print the next line the same cursor position.
 * @return None.
 */
void oled_new_line(oled_new_line_options new_line_option);

/**
 * @brief Set the cursor position, i.e. the start location to print.
 * @param cursor_coordinate The pixel coordinate to set the cursor to.
 */
void oled_set_cursor(oled_cursor_coordinate_t cursor_coordinate);

/**
 * @brief Fill the entire screen with byte pattern.
 * @param pattern Byte pattern to fill.
 * @return None.
 */
void oled_fill_all(uint8_t pattern);

/**
 * @brief Draw a dinosaur on the oled screen.
 * @param cursor_coordinate Set to this coordinate as the start pixel drawing
 * the dinosaur.
 * @return None.
 */
void oled_draw_dino_map(oled_cursor_coordinate_t cursor_coordinate);

#endif /* GRAPHICS_H */