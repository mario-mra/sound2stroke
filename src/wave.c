#include "wave.h"
#include "mrrb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>


// Read n bytes from the WAV file, and, if(compare_en), compares the obtained array with a given one
bool read_n_bytes(wave_file_t *wave, uint8_t *out_buff, size_t n_bytes, bool compare_en, uint8_t *compare_array)
{
    for(size_t i=0; i<n_bytes; i++)
        *(out_buff + i) = fgetc((*wave).file);

    if(compare_en)
        if(!compare_byte_arrays(out_buff, compare_array, n_bytes))
            return false;

    return true;
}


func_err_t _get_RIFF_chunk(wave_file_t *wave)
{
    _riff_t *RIFF_chunk = &(*wave).riff_chunk;

    // ckID
    uint8_t ckID[4];
    if(!read_n_bytes(wave, ckID, 4, true, (uint8_t *)"RIFF"))
        return FUNC_INPUT_ERR;
    
    (*RIFF_chunk).ckID = (uint32_t)merge_array(ckID, 4, BIG_ENDIAN);

    // cksize
    uint8_t cksize[4];
    read_n_bytes(wave, cksize, 4, false, NULL);

    (*RIFF_chunk).cksize = (uint32_t)merge_array(cksize, 4, LITTLE_ENDIAN);

    // WAVEID
    uint8_t WAVEID[4];
    if(read_n_bytes(wave, WAVEID, 4, true, (uint8_t *)"WAVE") == FUNC_FALSE)
        return FUNC_INPUT_ERR;
    
    (*RIFF_chunk).WAVEID = (uint32_t)merge_array(WAVEID, 4, BIG_ENDIAN);

    LOG_D("RIFF -> ckID: 0x%x\n", (*RIFF_chunk).ckID);
    LOG_D("RIFF -> cksize: 0x%x (%" PRIu32 ")\n", (*RIFF_chunk).cksize, (*RIFF_chunk).cksize);
    LOG_D("RIFF -> WAVEID: 0x%x\n", (*RIFF_chunk).WAVEID);

    return FUNC_OK;
}

func_err_t _get_fmt_chunk(wave_file_t *wave)
{
    _fmt_chunk_t *fmt_chunk = &(*wave).fmt_chunk;

    // ckID
    uint8_t ckID[4];
    if(read_n_bytes(wave, ckID, 4, true, (uint8_t *)"fmt ") == FUNC_FALSE)
        return FUNC_INPUT_ERR;
    
    (*fmt_chunk).ckID = (uint32_t)merge_array(ckID, 4, BIG_ENDIAN);

    // cksize
    uint8_t cksize[4];
    read_n_bytes(wave, cksize, 4, false, NULL);
    
    (*fmt_chunk).cksize = (uint32_t)merge_array(cksize, 4, LITTLE_ENDIAN);

    // wFormatTag
    uint8_t wFormatTag[2];
    read_n_bytes(wave, wFormatTag, 2, false, NULL);
    
    (*fmt_chunk).wFormatTag = (uint16_t)merge_array(wFormatTag, 2, LITTLE_ENDIAN);

    // nChannels
    uint8_t nChannels[2];
    read_n_bytes(wave, nChannels, 2, false, NULL);
    
    (*fmt_chunk).nChannels = (uint16_t)merge_array(nChannels, 2, LITTLE_ENDIAN);

    // nSamplesPerSec
    uint8_t nSamplesPerSec[4];
    read_n_bytes(wave, nSamplesPerSec, 4, false, NULL);
    
    (*fmt_chunk).nSamplesPerSec = (uint32_t)merge_array(nSamplesPerSec, 4, LITTLE_ENDIAN);

    // nAvgBytesPerSec
    uint8_t nAvgBytesPerSec[4];
    read_n_bytes(wave, nAvgBytesPerSec, 4, false, NULL);

    (*fmt_chunk).nAvgBytesPerSec = (uint32_t)merge_array(nAvgBytesPerSec, 4, LITTLE_ENDIAN);

    // nBlockAlign
    uint8_t nBlockAlign[2];
    read_n_bytes(wave, nBlockAlign, 2, false, NULL);
    
    (*fmt_chunk).nBlockAlign = (uint16_t)merge_array(nBlockAlign, 2, LITTLE_ENDIAN);

    // wBitsPerSample
    uint8_t wBitsPerSample[2];
    read_n_bytes(wave, wBitsPerSample, 2, false, NULL);
    
    (*fmt_chunk).wBitsPerSample = (uint16_t)merge_array(wBitsPerSample, 2, BIG_ENDIAN);

    if((*fmt_chunk).cksize > 16)
    {
        // cbSize
        uint8_t cbSize[2];
        read_n_bytes(wave, cbSize, 2, false, NULL);
        
        (*fmt_chunk).cbSize = (uint16_t)merge_array(cbSize, 2, LITTLE_ENDIAN);
    }

    if((*fmt_chunk).cksize > 18)
    {
        // wValidBitsPerSample
        uint8_t wValidBitsPerSample[2];
        read_n_bytes(wave, wValidBitsPerSample, 2, false, NULL);
        
        (*fmt_chunk).wValidBitsPerSample = (uint16_t)merge_array(wValidBitsPerSample, 2, BIG_ENDIAN);

        // dwChannelMask
        uint8_t dwChannelMask[4];
        read_n_bytes(wave, dwChannelMask, 4, false, NULL);
        
        (*fmt_chunk).dwChannelMask = (uint32_t)merge_array(dwChannelMask, 4, LITTLE_ENDIAN);

        // SubFormat
        uint8_t SubFormat_LOW[8];
        uint8_t SubFormat_HIGH[8];
        read_n_bytes(wave, SubFormat_LOW, 8, false, NULL);
        read_n_bytes(wave, SubFormat_HIGH, 8, false, NULL);
        
        (*fmt_chunk).SubFormat_LOW = (uint64_t)merge_array(SubFormat_LOW, 8, LITTLE_ENDIAN);
        (*fmt_chunk).SubFormat_HIGH = (uint64_t)merge_array(SubFormat_HIGH, 8, LITTLE_ENDIAN);
    }

    LOG_D("fmt  -> ckID: 0x%x\n", (*fmt_chunk).ckID);
    LOG_D("fmt  -> cksize: 0x%x (%" PRIu32 ")\n", (*fmt_chunk).cksize, (*fmt_chunk).cksize);
    LOG_D("fmt  -> wFormatTag: 0x%x\n", (*fmt_chunk).wFormatTag);
    LOG_D("fmt  -> nChannels: 0x%x\n", (*fmt_chunk).nChannels);
    LOG_D("fmt  -> nSamplesPerSec: 0x%x\n", (*fmt_chunk).nSamplesPerSec);
    LOG_D("fmt  -> nAvgBytesPerSec: 0x%x\n", (*fmt_chunk).nAvgBytesPerSec);
    LOG_D("fmt  -> nBlockAlign: 0x%x\n", (*fmt_chunk).nBlockAlign);
    LOG_D("fmt  -> wBitsPerSample: 0x%x\n", (*fmt_chunk).wBitsPerSample);
    if((*fmt_chunk).cksize > 16)
        LOG_D("fmt  -> cbSize: 0x%x\n", (*fmt_chunk).cbSize);
    if((*fmt_chunk).cksize > 18)
    {
        LOG_D("fmt  -> wValidBitsPerSample: 0x%x\n", (*fmt_chunk).wValidBitsPerSample);
        LOG_D("fmt  -> dwChannelMask: 0x%x\n", (*fmt_chunk).dwChannelMask);
        LOG_D("fmt  -> SubFormat_LOW: 0x%lx\n", (*fmt_chunk).SubFormat_LOW);
        LOG_D("fmt  -> SubFormat_HIGH: 0x%lx\n", (*fmt_chunk).SubFormat_HIGH);
    }

    return FUNC_OK;
}

func_err_t _get_data_chunk(wave_file_t *wave)
{
    _data_t *data_chunk = &(*wave).data_chunk;

    // ckID
    uint8_t ckID[4];
    if(read_n_bytes(wave, ckID, 4, true, (uint8_t *)"data") == FUNC_FALSE)
        return FUNC_INPUT_ERR;
    
    (*data_chunk).ckID = (uint32_t)merge_array(ckID, 4, BIG_ENDIAN);

    // cksize
    uint8_t cksize[4];
    read_n_bytes(wave, cksize, 4, false, NULL);
    
    (*data_chunk).cksize = (uint32_t)merge_array(cksize, 4, LITTLE_ENDIAN);

    LOG_D("Data -> ckID: 0x%x\n", (*data_chunk).ckID);
    LOG_D("Data -> cksize: 0x%x (%" PRIu32 ")\n", (*data_chunk).cksize, (*data_chunk).cksize);

    return FUNC_OK;
}

func_err_t _get_data(wave_file_t *wave)
{
    uint32_t samples = (*wave).data_chunk.cksize;
    (*wave).raw_data = calloc(samples, sizeof(uint8_t));

    if((*wave).raw_data == NULL)
        return FUNC_INPUT_ERR;

    read_n_bytes(wave, (*wave).raw_data, samples, false, NULL);
    (*wave).data_status = WAVE_DATA_RAW_OK;

    return FUNC_OK;
}

func_err_t _get_data_pad(wave_file_t *wave)
{
    _data_t *data_chunk = &(*wave).data_chunk;

    if((*wave).data_chunk.cksize%2 != 0)
    {
        uint8_t pad_byte[1];
        read_n_bytes(wave, pad_byte, 1, false, NULL);

        (*data_chunk).pad_byte = pad_byte[0];
        (*data_chunk).pad_status = true;

        LOG_D("Data -> pad_byte: 0x%x\n", (*data_chunk).pad_byte);

        return FUNC_OK;
    }
    else
    {
        (*data_chunk).pad_status = false;
        return FUNC_OK;

    }

    return FUNC_ERR;
}

func_err_t parse_wave(wave_file_t *wave)
{
    if((*wave).chunk_status == WAVE_CHUNK_PARSE_OK)
        return FUNC_INPUT_ERR;

    rewind((*wave).file);
    (*wave).chunk_status = WAVE_CHUNK_PARSE_ERR;

    if(_get_RIFF_chunk(wave) != FUNC_OK)
        return FUNC_ERR;
    if(_get_fmt_chunk(wave)  != FUNC_OK)
        return FUNC_ERR;
    if(_get_data_chunk(wave) != FUNC_OK)
        return FUNC_ERR;
    if(_get_data(wave) != FUNC_OK)
        return FUNC_ERR;
    if(_get_data_pad(wave) != FUNC_OK)
        return FUNC_ERR;

    rewind((*wave).file);
    (*wave).chunk_status = WAVE_CHUNK_PARSE_OK;
    return FUNC_OK;
}

wave_file_t *wave_open(const char *filename)
{
    FILE *audio_file = fopen(filename, "r");

    if(audio_file == NULL) return (wave_file_t *)NULL;
    else
    {
        wave_file_t *wave = (wave_file_t *) malloc(1 * sizeof(wave_file_t));

        if(wave == NULL)
            return (wave_file_t *)NULL;

        (*wave).file_status = WAVE_FILE_OPEN;

        fseek(audio_file, 0L, SEEK_END);
        (*wave).file_size = ftell(audio_file);
        rewind(audio_file);

        (*wave).chunk_status = WAVE_CHUNK_NOT_PARSED;

        (*wave).file = audio_file;

        return wave;
    }

    return (wave_file_t *)NULL;
}

void wave_close(wave_file_t *wave)
{
    if((*wave).file_status == WAVE_FILE_OPEN)
    {
        fclose((*wave).file);
        (*wave).file_status = WAVE_FILE_CLOSED;
    }
}

void wave_end(wave_file_t *wave)
{
    wave_close(wave);
    if((*wave).raw_data != NULL)
        free((*wave).raw_data);
    free(wave);
}