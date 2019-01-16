#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "mrrb.h"
#include "base64.h"
#include "wave.h"

int main()
{
    uint8_t conversion_test[] = "Hi there!!";;
    LOG_M("Input text: %s\n", (char *) conversion_test);

    size_t output_size1;
    char *encoded_data = base64_encode(conversion_test, sizeof(conversion_test)-1, &output_size1);
    LOG_M("Encoded text: %s\n", encoded_data);

    size_t output_size2;
    uint8_t *decoded_data = base64_decode(encoded_data, output_size1, &output_size2);
    LOG_M("Decoded text: %s\n", (char *) decoded_data);

    base64_free(encoded_data);
    base64_free(decoded_data);

    LOG_M("Size of long unsigned int -> %lu\n", sizeof(long unsigned int));
    LOG_M("Size of uint8_t  -> %" PRIu8  "\n", sizeof(uint8_t));
    LOG_M("Size of uint16_t -> %" PRIu16 "\n", sizeof(uint16_t));
    LOG_M("Size of uint32_t -> %" PRIu32 "\n", sizeof(uint32_t));
    LOG_M("Size of uint64_t -> %" PRIu64 "\n", sizeof(uint64_t));

    LOG_M("Size of wave_file_t -> %" PRIu64 "\n", sizeof(wave_file_t));

    return 0;
}