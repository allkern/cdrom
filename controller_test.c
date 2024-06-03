#include <stdio.h>
#include <stdint.h>

#include "cdrom.h"

uint8_t cdrom_read(psx_cdrom_t* cdrom, int index, int reg) {
    psx_cdrom_write8(cdrom, 0, index);

    return psx_cdrom_read8(cdrom, reg);
}

void cdrom_write(psx_cdrom_t* cdrom, int index, int reg, uint8_t data) {
    psx_cdrom_write8(cdrom, 0, index);
    psx_cdrom_write8(cdrom, reg, data);
}

int main(void) {
    psx_cdrom_t* cdrom = psx_cdrom_create();
    psx_cdrom_init(cdrom);

    cdrom->disc = cdrom;

    // Enable all INTs (IER <- 1Fh)
    // Send CdlTest(20h)
    cdrom_write(cdrom, 1, 2, 0x1f);
    cdrom_write(cdrom, 0, 1, 0x1a);

    uint8_t ifr;

    while (1) {
        int cycles = 0;

        while (1) {
            psx_cdrom_update(cdrom, 2);

            cycles += 2;

            ifr = cdrom_read(cdrom, 1, 3);

            if (ifr & 0x1f)
                break;
        }

        printf("Received INT%u after %u cycles\n", ifr & 0x1f, cycles);

        while (cdrom_read(cdrom, 0, 0) & 0x20)
            printf("response: %02x\n", cdrom_read(cdrom, 0, 1));

        // Acknowledge INT
        cdrom_write(cdrom, 1, 3, 0x1f);
    }

    return 0;
}