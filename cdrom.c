#include "cdrom.h"

#include <string.h>

void psx_cdrom_init(psx_cdrom_t* cdrom) {
    memset(cdrom, 0, sizeof(psx_cdrom_t));

    cdrom->data = queue_create();
    cdrom->response = queue_create();
    cdrom->parameters = queue_create();

    queue_init(cdrom->data, CD_SECTOR_SIZE);
    queue_init(cdrom->response, 32);
    queue_init(cdrom->parameters, 32);
}