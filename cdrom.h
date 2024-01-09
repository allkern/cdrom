#ifndef CDROM_H
#define CDROM_H

#include <stdint.h>

#include "queue.h"

#define CD_SECTOR_SIZE 2352

typedef struct {
    uint8_t status;
    uint8_t ier;
    uint8_t ifr;
    uint8_t vol[4];
    queue_t* data;
    queue_t* response;
    queue_t* parameters;
} psx_cdrom_t;

void psx_cdrom_init(psx_cdrom_t* cdrom);

#endif