// https://en.wikipedia.org/wiki/Base64#Implementations_and_history

#ifndef _BASE64_H_
#define _BASE64_H_

#include <stdint.h>
#include <stdlib.h>

// typedef const enum
// {
//     BASE64_PADDING_ENABLE  = 0,
//     BASE64_PADDING_DISABLE = 1
// } base64_padding_control_t;

// struct _encode_s
// {
//     uint8_t *input_data;
//     size_t   input_size;
//     size_t  *output_size;
//     uint_fast8_t padding_enable;
// };
// typedef struct _encode_s encode_input_t;

// struct _decode_s
// {
//     char   *input_data;
//     size_t  input_size;
//     size_t *output_size;
//     uint_fast8_t padding_enable;
// };
// typedef struct _decode_s decode_input_t;

char *base64_encode(uint8_t *input_data, size_t input_size, size_t *output_size);
uint8_t *base64_decode(char *input_data, size_t input_size, size_t *output_size);
void  base64_free(void *buffer);

#endif /* _BASE64_H_ */