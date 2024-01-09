#ifndef DISC_H
#define DISC_H

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

typedef struct {
    
} psx_disc_t;

#endif