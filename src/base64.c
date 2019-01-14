#include "base64.h"
#include "mrrb.h"

#include <inttypes.h>
#include <math.h>

const char index_table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const uint8_t padding_table[] = {0, 0, 2, 1};

char *base64_encode(uint8_t *input_data,
                    size_t   input_size,
                    size_t  *output_size)
{
    size_t conversion_size = (size_t) ceil((4 * input_size) / 3.0);
    uint_fast8_t padding_size = padding_table[conversion_size%4];
    *output_size = conversion_size + padding_size;

    // LOG_D("In: %lu - Conv: %lu - Padding: %" PRIu8 " - Output: %lu \n", input_size, conversion_size, padding_size, *output_size);

    char *buffer;
    buffer = (char *) malloc((*output_size + 1) * sizeof(char));

    uint_least32_t block = 0;
    for(size_t i = 0; i < *output_size/4; i++)
    {
        block = 0;
        for(uint_fast8_t j = 0; j < 3; j++)
            if((i*3 + j) <= input_size)
                block = block | (input_data[i*3 + j]<<(24 - 8*j));

        for(uint_fast8_t j = 0; j < 4; j++)
        {
            if((*output_size - (i*4 + j)) > padding_size)
                buffer[4*i + j] = index_table[(block >> (26-6*j)) & 0x3F];
            else
                buffer[4*i + j] = '=';
        }
    }

    buffer[*output_size] = '\0';
    return buffer;
}


uint8_t index_decode(char char_value)
{
    for(uint_fast8_t i=0; i<64; i++)
        if(index_table[i] == char_value)
            return i;
    return -1;
}


uint8_t *base64_decode(char   *input_data,
                       size_t  input_size,
                       size_t *output_size)
{
    uint_fast8_t padding_size = 0;
    size_t conversion_size = (size_t) floor((3 * input_size) / 4.0);
    for(uint8_t i = 1; i<=2 ; i++)
        padding_size += input_data[input_size-i] == '=' ? 1 : 0;
    *output_size = conversion_size - padding_size;

    // LOG_I("In: %lu - Conv: %lu - Padding: %" PRIu8 " - Output: %lu \n", input_size, conversion_size, padding_size, *output_size);

    uint8_t *buffer;
    buffer = (uint8_t *) malloc((*output_size) * sizeof(uint8_t));

    uint_least32_t block = 0;
    for(size_t i = 0; i < input_size/4; i++)
    {
        block = 0;
        for(uint_fast8_t j = 0; j < 4; j++)
            block = block | index_decode(input_data[i*4 + j]) << (26 - 6*j);

        for(uint_fast8_t j = 0; j < 3; j++)
            if(i*3 + j < *output_size)
                buffer[i*3 + j] = (block >> (24-8*j)) & 0xFF;
    }

    return buffer;
}


void base64_free(void *buffer)
{
    free(buffer);
}