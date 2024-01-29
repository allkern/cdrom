#ifndef DISC_H
#define DISC_H

#include <stdint.h>

/*
    PSX disc reader API version 2 specification:

    Mandatory formats:
    - BIN/CUE (Multi track)
    - BIN (Single track)

    Optional (but encouraged) formats:
    - ISO (Raw ISO9660 images)
    - CHD (Compressed MAME "Hunks of Data")

    Optional formats:
    - MDS/MDF (Alcohol 120% images)
*/

enum {
    TS_FAR = 0,
    TS_DATA,
    TS_AUDIO,
    TS_PREGAP
};

enum {
    CD_EXT_UNSUPPORTED = 0,
    CD_EXT_CUE,
    CD_EXT_BIN,
};

enum {
    CDT_ERROR = 0,
    CDT_LICENSED,
    CDT_AUDIO,
    CDT_UNKNOWN
};

#define CD_SECTOR_SIZE 2352

typedef int (*read_func)(void*, uint32_t, void*);

typedef struct {
    read_func read;
    void* udata;
} psx_disc_t;

psx_disc_t* psx_disc_create(void);
int psx_disc_open(psx_disc_t* disc, const char* path);
int psx_disc_read(psx_disc_t* disc, uint32_t lba, void* buf);
void psx_disc_close(psx_disc_t* disc);
void psx_disc_destroy(psx_disc_t* disc);

#endif