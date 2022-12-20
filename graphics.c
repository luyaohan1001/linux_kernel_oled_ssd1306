/**
 * @file graphics.c
 * @brief ssd1306 OLED graphics display APIs implementation.
 * @author Luyao Han (luyaohan1001@gmail.com)
 * @date 12-21-2022
 */

#include "graphics.h"
#include "stdarg.h"

#define FONT_TABLE_CHAR_WIDTH 6
#define DEFAULT_MESSAGE_LENGTH 256

/**
 * @brief ASCII Font table defined in hex encoding.
 * @note This table is accessed through numerical value of a char.
 *       Each single char is rendered on screen byte by byte (per slice).
 *       Non-Alphanumeric characters are encoded 0.
 */
static const unsigned char FONT_TABLE[][FONT_TABLE_CHAR_WIDTH] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'NUL'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'SOH'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'STX'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'ETX'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'EOT'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'ENQ'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'ACK'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'BEL'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'BS'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'HT'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'LF'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'VT'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'FF'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'CR'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'SO'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'SI'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'DLE'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'DC1'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'DC2'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'DC3'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'DC4'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'NAK'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'SYN'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'ETB'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'CAN'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'EM'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'SUB'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'ESC'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'FS'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'GS'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'RS'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // 'US'
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // ' '
    {0x00, 0x00, 0x2f, 0x00, 0x00, 0x00}, // '!'
    {0x00, 0x07, 0x00, 0x07, 0x00, 0x00}, // '"'
    {0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00}, // '#'
    {0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00}, // '$'
    {0x23, 0x13, 0x08, 0x64, 0x62, 0x00}, // '%'
    {0x36, 0x49, 0x55, 0x22, 0x50, 0x00}, // '&'
    {0x00, 0x05, 0x03, 0x00, 0x00, 0x00}, // '''
    {0x00, 0x1c, 0x22, 0x41, 0x00, 0x00}, // '('
    {0x00, 0x41, 0x22, 0x1c, 0x00, 0x00}, // ')'
    {0x14, 0x08, 0x3E, 0x08, 0x14, 0x00}, // '*'
    {0x08, 0x08, 0x3E, 0x08, 0x08, 0x00}, // '+'
    {0x00, 0x00, 0xA0, 0x60, 0x00, 0x00}, // ','
    {0x08, 0x08, 0x08, 0x08, 0x08, 0x00}, // '-'
    {0x00, 0x60, 0x60, 0x00, 0x00, 0x00}, // '.'
    {0x20, 0x10, 0x08, 0x04, 0x02, 0x00}, // '/'
    {0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00}, // '0'
    {0x00, 0x42, 0x7F, 0x40, 0x00, 0x00}, // '1'
    {0x42, 0x61, 0x51, 0x49, 0x46, 0x00}, // '2'
    {0x21, 0x41, 0x45, 0x4B, 0x31, 0x00}, // '3'
    {0x18, 0x14, 0x12, 0x7F, 0x10, 0x00}, // '4'
    {0x27, 0x45, 0x45, 0x45, 0x39, 0x00}, // '5'
    {0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00}, // '6'
    {0x01, 0x71, 0x09, 0x05, 0x03, 0x00}, // '7'
    {0x36, 0x49, 0x49, 0x49, 0x36, 0x00}, // '8'
    {0x06, 0x49, 0x49, 0x29, 0x1E, 0x00}, // '9'
    {0x00, 0x36, 0x36, 0x00, 0x00, 0x00}, // ':'
    {0x00, 0x56, 0x36, 0x00, 0x00, 0x00}, // ';'
    {0x08, 0x14, 0x22, 0x41, 0x00, 0x00}, // '<'
    {0x14, 0x14, 0x14, 0x14, 0x14, 0x00}, // '='
    {0x00, 0x41, 0x22, 0x14, 0x08, 0x00}, // '>'
    {0x02, 0x01, 0x51, 0x09, 0x06, 0x00}, // '?'
    {0x32, 0x49, 0x59, 0x51, 0x3E, 0x00}, // '@'
    {0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00}, // 'A'
    {0x7F, 0x49, 0x49, 0x49, 0x36, 0x00}, // 'B'
    {0x3E, 0x41, 0x41, 0x41, 0x22, 0x00}, // 'C'
    {0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00}, // 'D'
    {0x7F, 0x49, 0x49, 0x49, 0x41, 0x00}, // 'E'
    {0x7F, 0x09, 0x09, 0x09, 0x01, 0x00}, // 'F'
    {0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00}, // 'G'
    {0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00}, // 'H'
    {0x00, 0x41, 0x7F, 0x41, 0x00, 0x00}, // 'I'
    {0x20, 0x40, 0x41, 0x3F, 0x01, 0x00}, // 'J'
    {0x7F, 0x08, 0x14, 0x22, 0x41, 0x00}, // 'K'
    {0x7F, 0x40, 0x40, 0x40, 0x40, 0x00}, // 'L'
    {0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00}, // 'M'
    {0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00}, // 'N'
    {0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00}, // 'O'
    {0x7F, 0x09, 0x09, 0x09, 0x06, 0x00}, // 'P'
    {0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00}, // 'Q'
    {0x7F, 0x09, 0x19, 0x29, 0x46, 0x00}, // 'R'
    {0x46, 0x49, 0x49, 0x49, 0x31, 0x00}, // 'S'
    {0x01, 0x01, 0x7F, 0x01, 0x01, 0x00}, // 'T'
    {0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00}, // 'U'
    {0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00}, // 'V'
    {0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00}, // 'W'
    {0x63, 0x14, 0x08, 0x14, 0x63, 0x00}, // 'X'
    {0x07, 0x08, 0x70, 0x08, 0x07, 0x00}, // 'Y'
    {0x61, 0x51, 0x49, 0x45, 0x43, 0x00}, // 'Z'
    {0x00, 0x7F, 0x41, 0x41, 0x00, 0x00}, // '['
    {0x55, 0xAA, 0x55, 0xAA, 0x55, 0x00}, // '\'
    {0x00, 0x41, 0x41, 0x7F, 0x00, 0x00}, // ']'
    {0x04, 0x02, 0x01, 0x02, 0x04, 0x00}, // '^'
    {0x40, 0x40, 0x40, 0x40, 0x40, 0x00}, // '_'
    {0x00, 0x03, 0x05, 0x00, 0x00, 0x00}, // '`'
    {0x20, 0x54, 0x54, 0x54, 0x78, 0x00}, // 'a'
    {0x7F, 0x48, 0x44, 0x44, 0x38, 0x00}, // 'b'
    {0x38, 0x44, 0x44, 0x44, 0x20, 0x00}, // 'c'
    {0x38, 0x44, 0x44, 0x48, 0x7F, 0x00}, // 'd'
    {0x38, 0x54, 0x54, 0x54, 0x18, 0x00}, // 'e'
    {0x08, 0x7E, 0x09, 0x01, 0x02, 0x00}, // 'f'
    {0x18, 0xA4, 0xA4, 0xA4, 0x7C, 0x00}, // 'g'
    {0x7F, 0x08, 0x04, 0x04, 0x78, 0x00}, // 'h'
    {0x00, 0x44, 0x7D, 0x40, 0x00, 0x00}, // 'i'
    {0x40, 0x80, 0x84, 0x7D, 0x00, 0x00}, // 'j'
    {0x7F, 0x10, 0x28, 0x44, 0x00, 0x00}, // 'k'
    {0x00, 0x41, 0x7F, 0x40, 0x00, 0x00}, // 'l'
    {0x7C, 0x04, 0x18, 0x04, 0x78, 0x00}, // 'm'
    {0x7C, 0x08, 0x04, 0x04, 0x78, 0x00}, // 'n'
    {0x38, 0x44, 0x44, 0x44, 0x38, 0x00}, // 'o'
    {0xFC, 0x24, 0x24, 0x24, 0x18, 0x00}, // 'p'
    {0x18, 0x24, 0x24, 0x18, 0xFC, 0x00}, // 'q'
    {0x7C, 0x08, 0x04, 0x04, 0x08, 0x00}, // 'r'
    {0x48, 0x54, 0x54, 0x54, 0x20, 0x00}, // 's'
    {0x04, 0x3F, 0x44, 0x40, 0x20, 0x00}, // 't'
    {0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00}, // 'u'
    {0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00}, // 'v'
    {0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00}, // 'w'
    {0x44, 0x28, 0x10, 0x28, 0x44, 0x00}, // 'x'
    {0x1C, 0xA0, 0xA0, 0xA0, 0x7C, 0x00}, // 'y'
    {0x44, 0x64, 0x54, 0x4C, 0x44, 0x00}, // 'z'
    {0x00, 0x10, 0x7C, 0x82, 0x00, 0x00}, // '{'
    {0x00, 0x00, 0xFF, 0x00, 0x00, 0x00}, // '|'
    {0x00, 0x82, 0x7C, 0x10, 0x00, 0x00}, // '}'
    {0x00, 0x06, 0x09, 0x09, 0x06, 0x00}  // '~'
};

/**
 * @brief Bitmap for a dinosaur.
 * @note Bitmap code generated using https://javl.github.io/image2cpp/
 */
#define DINOSAUR_BITMAP_ROWS 4
#define DINOSAUR_BITMAP_COLUMNS 32
const unsigned char
    DINOSAUR_BITMAP[DINOSAUR_BITMAP_ROWS][DINOSAUR_BITMAP_COLUMNS] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xe8, 0xf8, 0xf8, 0xf8, 0xf8,
         0xf8, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xf0, 0xe0, 0xc0, 0xc0, 0xe0,
         0xf0, 0xf0, 0xf8, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x13, 0x32, 0x02,
         0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x01, 0x03, 0x07, 0x0f, 0xff,
         0xbf, 0x1f, 0x0f, 0x1f, 0xff, 0x87, 0x03, 0x01, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
         0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
         0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}};

/**
 * @brief Struct that book-keeps parameters for the oled graphics.
 * @param line Current line (page) the cursor is on.
 * @param cursor_position Current position (column) the cursor is on.
 * @param font_char_width ASCII char width for estimation of potential colunm
 * overrun.
 */
static oled_graphics_params_t oled_graphics_params = {
    .line = 0, .cursor_position = 0, .font_char_width = FONT_TABLE_CHAR_WIDTH};

/**
 * @brief Fill the entire screen with byte pattern.
 * @param pattern Byte pattern to fill.
 * @return None.
 */
void oled_fill_all(uint8_t pattern) {
  static const int TOTAL_PIXELS = OLED_COLUMN_LENGTH * OLED_PAGE_LENGTH;
  uint32_t pixel = 0;

  oled_set_cursor(0, 0);

  for (pixel = 0; pixel < TOTAL_PIXELS; ++pixel) {
    ssd1306_write_address(DATA_CONTROL, DONT_CARE, 1, &pattern);
  }
}

/**
 * @brief Set the cursor position, i.e. the start location to print.
 * @param line The vertical line (page) to set the cursor to.
 * @param position The horizontal position (column) to the set the cursor to.
 */
void oled_set_cursor(uint8_t line, uint8_t position) {
  uint8_t packet[2];
  /* Move the Cursor to specified position only if it is in range */
  if ((line <= OLED_PAGE_MAX) && (position < OLED_COLUMN_MAX)) {
    oled_graphics_params.line = line;
    oled_graphics_params.cursor_position = position;

    /* Specify column start and end address. */
    memcpy(packet, (uint8_t[]){position, OLED_COLUMN_MAX}, 2);
    ssd1306_write_address(COMMAND_CONTROL, SET_COLUMN_ADDRESS, 2, packet);

    /* Specify page (rows) start and end address. */
    memcpy(packet, (uint8_t[]){line, OLED_PAGE_MAX}, 2);
    ssd1306_write_address(COMMAND_CONTROL, SET_PAGE_ADDRESS, 2, packet);
  }
}

/**
 * @brief Change to a new line on the OLED screen.
 * @param None.
 * @return None.
 */
void oled_new_line(void) {
  /* Increment and wrap-around to avoid overrun. */
  oled_graphics_params.line += 1;
  oled_graphics_params.line &= OLED_PAGE_MAX;

  /* Set cursor to the new line. */
  oled_set_cursor(oled_graphics_params.line, 0);
}

/**
 * @brief Print single char to the oled screen.
 * @param ascii_char ASCII character to put.
 * @return None.
 */
void oled_putc(unsigned char ascii_char) {
  uint8_t font_char_slice;
  uint8_t slice = 0;

  /* Change-of-line detection. */
  if (((oled_graphics_params.cursor_position +
        oled_graphics_params.font_char_width) >= OLED_COLUMN_LENGTH) ||
      (ascii_char == '\n')) {
    oled_new_line();
  }

  /* Print the character from the hex font table, slice by slice. */
  if (ascii_char != '\n') {
    for (slice = 0; slice < oled_graphics_params.font_char_width; slice += 1) {
      font_char_slice = FONT_TABLE[ascii_char][slice];
      ssd1306_write_address(DATA_CONTROL, DONT_CARE, 1, &font_char_slice);
      oled_graphics_params.cursor_position += 1;
    }
  }
}

/**
 * @brief printf on oled with variadic arguments to print on the oled screen.
 * @param format Format supplied including string and/or parameters.
 * @return None.
 */
void oled_printf(const char *format, ...) {
  char message_buffer[DEFAULT_MESSAGE_LENGTH];
  char *p_message_buffer = NULL;
  va_list args;

  memset(message_buffer, '\0', DEFAULT_MESSAGE_LENGTH);

  /* Initialize pointer */
  /* Append the variable argument lists. */
  va_start(args, format);
  vsprintf(message_buffer, format, args);
  va_end(args);

  p_message_buffer = (char *)message_buffer;

  while (*p_message_buffer) {
    oled_putc(*p_message_buffer++);
  }
}

/**
 * @brief Draw a dinosaur on the oled screen.
 * @param None.
 * @return None.
 */
void oled_draw_dino_map(void) {
  uint8_t slice;
  int row, column;

  for (row = 0; row < DINOSAUR_BITMAP_ROWS; row += 1) {
    for (column = 0; column < 32; column += 1) {
      slice = DINOSAUR_BITMAP[row][column];
      ssd1306_write_address(DATA_CONTROL, DONT_CARE, 1, &slice);
    }
    oled_new_line();
  }
}