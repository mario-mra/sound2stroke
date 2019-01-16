// http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html

#ifndef _WAVE_H_
#define _WAVE_H_

#include <stdint.h>
#include <stdio.h>

#include "mrrb.h"

// Standard format codes for waveform data
typedef enum
{
    WAVE_FORMAT_PCM        = 0x0001, /* PCM */
    WAVE_FORMAT_IEEE_FLOAT = 0x0003, /* IEEE float */
    WAVE_FORMAT_ALAW       = 0x0006, /* 8-bit ITU-T G.711 A-law */
    WAVE_FORMAT_MULAW      = 0x0007, /* 8-bit ITU-T G.711 µ-law */
    WAVE_FORMAT_EXTENSIBLE = 0xFFFE  /* Determined by SubFormat */
} wFormatTag_t;

// RIFF Chunk
struct _riff_s
{
    uint32_t ckID;   /* Chunk ID: "RIFF" [4] */
    uint32_t cksize; /* Chunk size: 4+n [4] */
    uint32_t WAVEID; /* WAVE ID: "WAVE" [4] */
};
typedef struct _riff_s _riff_t;

// fmt Chunk
struct _fmt_chunk_s
{
    uint32_t ckID;             /* Chunk ID: "fmt "  [4] */
    uint32_t cksize;           /* Chunk size: 16, 18 or 40 [4] */
    uint16_t wFormatTag;          /* Format code [2] */
    uint16_t nChannels;           /* Number of interleaved channels [2] */
    uint32_t nSamplesPerSec;      /* Sampling rate (blocks per second) [4] */
    uint32_t nAvgBytesPerSec;     /* Data rate [4] */
    uint16_t nBlockAlign;         /* Data block size (bytes) [2] */
    uint16_t wBitsPerSample;      /* Bits per sample [2] */
    uint16_t cbSize;              /* Size of the extension (0 or 22) [2] */
    uint16_t wValidBitsPerSample; /* Number of valid bits [2] */
    uint32_t dwChannelMask;       /* Speaker position mask [4] */
    uint64_t SubFormat_LOW;       /* GUID, including the data format code [LOW] [8-A] */
    uint64_t SubFormat_HIGH;      /* GUID, including the data format code [HIGH] [8-B] */
};
typedef struct _fmt_chunk_s _fmt_chunk_t;

// data Chunk
struct _data_s
{
    uint32_t ckID;       /* Chunk ID: "data" [4] */
    uint32_t cksize;     /* Chunk size: n [4] */
    uint8_t  pad_byte;   /* Padding byte if n is odd [0 or 1] */
    bool     pad_status; /* true or false dependign if pad byte exists or not */
};
typedef struct _data_s _data_t;

// Wave struct
typedef enum
{
    WAVE_FILE_NULL    = 0x00, /* File not created/open */

    WAVE_FILE_OPEN    = 0x01, /* Existing wav file open */
    WAVE_FILE_CREATED = 0x02, /* New empty wav struct created [NOT SAVED YET!] */

    WAVE_FILE_CLOSED  = 0x03, /* wav file closed */

    WAVE_FILE_EDITED  = 0x04, /* Existing/new wav file edited */
    WAVE_FILE_SAVED   = 0x05, /* Existing/new wav file saved */

    WAVE_FILE_DEFAULT = WAVE_FILE_NULL
} wave_file_status_t;

typedef enum
{
    WAVE_DATA_NULL    = 0x00, /* Data not stored yet */

    WAVE_DATA_RAW_OK  = 0x01, /* Stored RAW data OK */
    WAVE_DATA_RAW_ERR = 0x02, /* Errors in stored RAW data */

    WAVE_DATA_PROCESSED_OK  = 0x01, /* Stored processed data OK */
    WAVE_DATA_PROCESSED_ERR = 0x02, /* Errors in stored processed data */

    WAVE_DATA_DEFAULT = WAVE_DATA_NULL
} wave_data_status_t;

typedef enum
{
    WAVE_CHUNK_NULL       = 0x00, /* Chunks not writed/readed yet */

    WAVE_CHUNK_NOT_PARSED = 0x01, /* The wav file has not being processed yet */
    WAVE_CHUNK_EMPTY      = 0x02, /* Chunk values empty */

    WAVE_CHUNK_PARSE_OK   = 0x04, /* Readed data valid */
    WAVE_CHUNK_PARSE_ERR  = 0x05, /* Readed data invalid */
    WAVE_CHUNK_VALID      = 0x06, /* The stored values are valid */
    WAVE_CHUNK_INVALID    = 0x07, /* The stored values are NOT valid */

    WAVE_CHUNK_DEFAULT    = WAVE_CHUNK_NULL
} wave_chunk_status_t;

struct wave_file_s
{
    FILE *file;
    size_t file_size;
    wave_file_status_t file_status;

    uint8_t *raw_data;
    wave_data_status_t data_status;

    wFormatTag_t format;

    _riff_t riff_chunk;
    _fmt_chunk_t fmt_chunk;
    _data_t data_chunk;
    wave_chunk_status_t chunk_status;

    /*
        For the next 3 variables:
        "bit 0" indicates value NOT valid
        "bit 1" indicates value valid

        MSB == 1: All values OK
        The n values in the struct are situated from LSB to MSB

        _riff_values_status example: 
            [uint8_T] xxxxxxxx
                      |    |||
                      V    |||
          All values OK    V||
                    ckID Ok V|
                   cksize Ok V
                     WAVEID Ok
    */
    uint8_t  _riff_values_status; /*  */
    uint16_t _fmt_values_status;  /*  */
    uint8_t  _data_values_status; /*  */
};
typedef struct wave_file_s wave_file_t;



wave_file_t *wave_open(const char *filename);
func_err_t parse_wave(wave_file_t *wave);
void wave_close(wave_file_t *wave_file);
void wave_end(wave_file_t *wave_file);

#endif /* End of _WAVE_H_ */