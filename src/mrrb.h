#ifndef _MRRB_H_
#define _MRRB_H_

#include <stdio.h>      /*  */
#include <stdint.h>     /* Stan */
#include <stdbool.h>    /* Boolean true & false */
#include "ANSI_codes.h" /* Graphics & Cursor capabilities */

#define mrrb_version "0.2"
#define mrrb_version_major 0
#define mrrb_version_minor 2
#define mrrb_author "Mario Rubio."
#define mrrb_date   "01-jan-2019"
#define mrrb_date_unix 1547483993

// Binary print
// https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format#3208376
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
   (byte & 0x80 ? '1' : '0'), \
   (byte & 0x40 ? '1' : '0'), \
   (byte & 0x20 ? '1' : '0'), \
   (byte & 0x10 ? '1' : '0'), \
   (byte & 0x08 ? '1' : '0'), \
   (byte & 0x04 ? '1' : '0'), \
   (byte & 0x02 ? '1' : '0'), \
   (byte & 0x01 ? '1' : '0') 


// LOG macros
#define LOG_I(format, ...) fprintf(stdout, ANSI_TEXT_COLOR_CYAN "Info > " ANSI_GRAPHICS_RESET format, ##__VA_ARGS__)  /* Log Info */
#define LOG_M(format, ...) fprintf(stdout, format, ##__VA_ARGS__)                                                     /* Log Message */
#define LOG_D(format, ...) fprintf(stdout, ANSI_TEXT_COLOR_CYAN "Debug > " ANSI_GRAPHICS_RESET format, ##__VA_ARGS__) /* Log Debug */
#define LOG_E(format, ...) fprintf(stderr, ANSI_TEXT_COLOR_RED  "Error > " ANSI_GRAPHICS_RESET format, ##__VA_ARGS__) /* Log Error */


// Application codes
typedef const enum
{
   IO_TRUE    = true,     /* 1 */
   IO_FALSE   = false,    /* 0 */
   FUNC_TRUE  = IO_TRUE,  /* 1 */
   FUNC_FALSE = IO_FALSE, /* 0 */
   FUNC_OK    = IO_TRUE,  /* 1 */
   FUNC_ERR   = IO_FALSE, /* 0 */

   FUNC_INPUT_ERR = 2,

   MAIN_OK     = 0,
   MAIN_NOT_OK = 1
} AppCodes_t;


// merge_array() -> Merge array (of real numbers 8bits to 64bits) into a unique variable
typedef const enum
{
   LITTLE_ENDIAN = 0x00,
   BIG_ENDIAN    = 0x01
} Endianness_t;
uint64_t merge_array(uint8_t *array, size_t input_size, Endianness_t endianness);

// compare_byte_arrays() -> Compare each element of the first array with the second one
bool compare_byte_arrays(uint8_t *array1, uint8_t *array2, size_t size);

#endif /* _MRRB_H_ */