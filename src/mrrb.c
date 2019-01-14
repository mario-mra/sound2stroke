#include <stdint.h>
#include <stdbool.h>

#include "mrrb.h"

// Merge array (of real numbers 8bits to 64bits) into a unique variable
uint64_t merge_array(uint8_t *array, size_t input_size, Endianness_t endianness)
{
    uint64_t buffer = 0x0;
    for(uint8_t i=0; i<input_size; i++)
    {
        if(i >= 8) break;

        if(endianness == LITTLE_ENDIAN)
            buffer |= array[i]<<(8*i);
        else if(endianness == BIG_ENDIAN)
            buffer |= array[i]<<8*((input_size<8 ? input_size : 8) - 1 - i);
        else
            break;
    }

    return buffer;
}

// Compare each element of the first array with the second one
bool compare_byte_arrays(uint8_t *array1, uint8_t *array2, size_t size)
{
    for(size_t i=0; i<size; i++)
        if(*(array1+i) != *(array2+i)) return false;

    return true;
}