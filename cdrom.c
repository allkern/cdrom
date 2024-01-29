#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "cdrom.h"

#define BTOI(b) btoi_table[b]

static const uint8_t btoi_table[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
    0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
    0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,
    0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41,
    0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43,
    0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
    0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d,
    0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61,
    0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b,
    0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73,
    0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
    0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d,
    0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91,
    0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93,
    0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b,
    0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d,
    0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5,
};

void cdrom_cmd_getstat(psx_cdrom_t* cdrom);
void cdrom_cmd_setloc(psx_cdrom_t* cdrom);
void cdrom_cmd_play(psx_cdrom_t* cdrom);
void cdrom_cmd_forward(psx_cdrom_t* cdrom);
void cdrom_cmd_backward(psx_cdrom_t* cdrom);
void cdrom_cmd_readn(psx_cdrom_t* cdrom);
void cdrom_cmd_motoron(psx_cdrom_t* cdrom);
void cdrom_cmd_stop(psx_cdrom_t* cdrom);
void cdrom_cmd_pause(psx_cdrom_t* cdrom);
void cdrom_cmd_init(psx_cdrom_t* cdrom);
void cdrom_cmd_mute(psx_cdrom_t* cdrom);
void cdrom_cmd_demute(psx_cdrom_t* cdrom);
void cdrom_cmd_setfilter(psx_cdrom_t* cdrom);
void cdrom_cmd_setmode(psx_cdrom_t* cdrom);
void cdrom_cmd_getparam(psx_cdrom_t* cdrom);
void cdrom_cmd_getlocl(psx_cdrom_t* cdrom);
void cdrom_cmd_getlocp(psx_cdrom_t* cdrom);
void cdrom_cmd_setsession(psx_cdrom_t* cdrom);
void cdrom_cmd_gettn(psx_cdrom_t* cdrom);
void cdrom_cmd_gettd(psx_cdrom_t* cdrom);
void cdrom_cmd_seekl(psx_cdrom_t* cdrom);
void cdrom_cmd_seekp(psx_cdrom_t* cdrom);
void cdrom_cmd_test(psx_cdrom_t* cdrom);
void cdrom_cmd_getid(psx_cdrom_t* cdrom);
void cdrom_cmd_reads(psx_cdrom_t* cdrom);
void cdrom_cmd_reset(psx_cdrom_t* cdrom);
void cdrom_cmd_getq(psx_cdrom_t* cdrom);
void cdrom_cmd_readtoc(psx_cdrom_t* cdrom);
void cdrom_cmd_videocd(psx_cdrom_t* cdrom);

void cdrom_write_stat(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_cmd(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_null(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_parm(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_ier(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_ifr(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_req(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_vol0(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_vol1(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_vol2(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_vol3(psx_cdrom_t* cdrom, uint8_t data);
void cdrom_write_vapp(psx_cdrom_t* cdrom, uint8_t data);

cdrom_write_func cdrom_write_table[] = {
    cdrom_write_stat, cdrom_write_cmd , cdrom_write_parm, cdrom_write_req ,
    cdrom_write_stat, cdrom_write_null, cdrom_write_ier , cdrom_write_ifr ,
    cdrom_write_stat, cdrom_write_null, cdrom_write_vol0, cdrom_write_vol1,
    cdrom_write_stat, cdrom_write_vol2, cdrom_write_vol3, cdrom_write_vapp
};

psx_cdrom_t* psx_cdrom_create(void) {
    return malloc(sizeof(psx_cdrom_t));
}

void psx_cdrom_init(psx_cdrom_t* cdrom) {
    memset(cdrom, 0, sizeof(psx_cdrom_t));

    cdrom->data = queue_create();
    cdrom->response = queue_create();
    cdrom->parameters = queue_create();

    queue_init(cdrom->data, CD_SECTOR_SIZE);
    queue_init(cdrom->response, 32);
    queue_init(cdrom->parameters, 32);
}

void cdrom_set_int(psx_cdrom_t* cdrom, int n) {
    cdrom->ifr = n;
}

void cdrom_error(psx_cdrom_t* cdrom, uint8_t stat, uint8_t err) {
    cdrom->ifr = 5;

    queue_push(cdrom->response, CD_STAT_ERROR | stat);
    queue_push(cdrom->response, err);
}

void cdrom_handle_first_response(psx_cdrom_t* cdrom) {
    // Check for no disc
    // i.e. INT5(11h, 80h)
    if (!cdrom->disc) {
        cdrom_error(cdrom, CD_STAT_SHELLOPEN, CD_ERR_NO_DISC);

        return;
    }

    // Check for wrong number of parameters and invalid commands
    // i.e. INT5(03h, 20h), INT5(03h, 40h)
    switch (cdrom->pending_command) {
        case CDL_GETSTAT:
        case CDL_FORWARD:
        case CDL_BACKWARD:
        case CDL_READN:
        case CDL_MOTORON:
        case CDL_STOP:
        case CDL_PAUSE:
        case CDL_INIT:
        case CDL_MUTE:
        case CDL_DEMUTE:
        case CDL_GETPARAM:
        case CDL_GETLOCL:
        case CDL_GETLOCP:
        case CDL_GETTN:
        case CDL_SEEKL:
        case CDL_SEEKP:
        case CDL_GETID:
        case CDL_READS:
        case CDL_RESET:
        case CDL_READTOC: {
            // These commands take no parameters
            if (queue_size(cdrom->parameters)) {
                cdrom_error(cdrom,
                    CD_STAT_SPINDLE,
                    CD_ERR_WRONG_PARAMETER_COUNT
                );

                return;
            }
        } break;

        case CDL_SETLOC: {
            // Setloc takes exactly 3 parameters
            if (queue_size(cdrom->parameters) != 3) {
                cdrom_error(cdrom,
                    CD_STAT_SPINDLE,
                    CD_ERR_WRONG_PARAMETER_COUNT
                );

                return;
            }
        } break;

        case CDL_PLAY: {
            // Play may take either 0 or 1 parameter
            if (queue_size(cdrom->parameters) > 1) {
                cdrom_error(cdrom,
                    CD_STAT_SPINDLE,
                    CD_ERR_WRONG_PARAMETER_COUNT
                );

                return;
            }
        } break;

        case CDL_SETFILTER:
        case CDL_GETQ: {
            // Setfilter and GetQ both take exactly 2 parameters
            if (queue_size(cdrom->parameters) != 2) {
                cdrom_error(cdrom,
                    CD_STAT_SPINDLE,
                    CD_ERR_WRONG_PARAMETER_COUNT
                );

                return;
            }
        } break;

        case CDL_SETMODE:
        case CDL_GETTD:
        case CDL_TEST: {
            // Setmode and GetTD both take exactly 1 parameter

            // Test may actually take additional parameters depending
            // on the subfunction, but we only emulate subfunction 20h
            // for now, which takes no extra parameters
            if (queue_size(cdrom->parameters) != 1) {
                cdrom_error(cdrom,
                    CD_STAT_SPINDLE,
                    CD_ERR_WRONG_PARAMETER_COUNT
                );

                return;
            }
        } break;

        case CDL_VIDEOCD: {
            // To-do: Check for model
            // VideoCD is only supported on the SCPH-5903
            // Should return invalid command normally

            // VideoCD takes exactly 6 parameters
            if (queue_size(cdrom->parameters) != 6) {
                cdrom_error(cdrom,
                    CD_STAT_SPINDLE,
                    CD_ERR_WRONG_PARAMETER_COUNT
                );

                return;
            }
        } break;

        default: {
            // Invalid command
            cdrom_error(cdrom,
                CD_STAT_SPINDLE,
                CD_ERR_INVALID_COMMAND
            );

            return;
        } break;
    }

    // If everything is alright (i.e. disc present, valid command,
    // correct number of parameters) then send INT3(stat)
    cdrom_set_int(cdrom, 3);

    queue_push(cdrom->response, CD_STAT_SPINDLE);

    // Check for commands that send second responses
    switch (cdrom->pending_command) {
        case CDL_STOP:
        case CDL_MOTORON:
        case CDL_PAUSE:
    }

    cdrom->state = CD_STATE_IDLE;
}

void psx_cdrom_update(psx_cdrom_t* cdrom, int cycles) {
    if (cdrom->delay > 0) {
        cdrom->delay -= cycles;

        return;
    }

    if (cdrom->state == CD_STATE_IDLE)
        return;

    if (cdrom->state == CD_STATE_PROCESSING_COMMAND) {
        cdrom_handle_first_response(cdrom);

        return;
    }
}

uint8_t cdrom_read_status(psx_cdrom_t* cdrom) {
    return (cdrom->index                        << 0) |
           (cdrom->xa_playing                   << 2) |
           (queue_is_empty(cdrom->parameters)   << 3) |
           ((!queue_is_full(cdrom->parameters)) << 4) |
           ((!queue_is_empty(cdrom->response))  << 5) |
           ((!queue_is_empty(cdrom->data))      << 6) |
           (cdrom->busy                         << 7);
}

uint8_t cdrom_read_data(psx_cdrom_t* cdrom) {
    if (!cdrom->data_req)
        return 0;

    return queue_pop(cdrom->data);
}

uint32_t psx_cdrom_read8(psx_cdrom_t* cdrom, uint32_t addr) {
    switch (cdrom->index) {
        case 0: return cdrom_read_status(cdrom);
        case 1: return queue_pop(cdrom->response);
        case 2: return cdrom_read_data(cdrom);
        case 3: return (addr & 1) ? (cdrom->ifr | 0xe0) : cdrom->ier;
    }
}
void psx_cdrom_write8(psx_cdrom_t* cdrom, uint32_t addr, uint32_t value) {
    cdrom_write_table[(cdrom->index << 2) | addr](cdrom, value);
}

void psx_cdrom_destroy(psx_cdrom_t* cdrom);

void cdrom_write_stat(psx_cdrom_t* cdrom, uint8_t data) {
    cdrom->index = data & 3;
}

void cdrom_write_cmd(psx_cdrom_t* cdrom, uint8_t data) {
    if (cdrom->state == CD_STATE_IDLE) {
        cdrom->pending_command = data;
        cdrom->state = CD_STATE_PROCESSING_COMMAND;

        // Init takes a little bit longer to process
        // i.e. INT3 arrives 1ms late
        if (data == CDL_INIT) {
            cdrom->delay = DELAY_1MS * 2;
        } else {
            cdrom->delay = DELAY_1MS;
        }

        cdrom->busy = 1;

        return;
    }

    if (cdrom->state == CD_STATE_PROCESSING_COMMAND) {

    }
}

void cdrom_write_null(psx_cdrom_t* cdrom, uint8_t data) {
    /* Ignore writes */
}

void cdrom_write_parm(psx_cdrom_t* cdrom, uint8_t data) {
    queue_push(cdrom->parameters, data);
}

void cdrom_write_ier(psx_cdrom_t* cdrom, uint8_t data) {
    cdrom->ier = data;
}

void cdrom_write_ifr(psx_cdrom_t* cdrom, uint8_t data) {
    if (data & 0x40)
        queue_clear(cdrom->parameters);

    cdrom->ifr &= ~(data & 0x1f);

    // If an INT is acknowledged, then the response
    // FIFO is cleared
    if ((cdrom->ifr & 0x1f) == 0)
        queue_clear(cdrom->response);
}

void cdrom_write_req(psx_cdrom_t* cdrom, uint8_t data) {
    cdrom->data_req = data & 0x80;
}

void cdrom_write_vol0(psx_cdrom_t* cdrom, uint8_t data) {
    cdrom->vol[0] = data;
}

void cdrom_write_vol1(psx_cdrom_t* cdrom, uint8_t data) {
    cdrom->vol[1] = data;
}

void cdrom_write_vol2(psx_cdrom_t* cdrom, uint8_t data) {
    cdrom->vol[2] = data;
}

void cdrom_write_vol3(psx_cdrom_t* cdrom, uint8_t data) {
    cdrom->vol[3] = data;
}

void cdrom_write_vapp(psx_cdrom_t* cdrom, uint8_t data) {
    /* To-do: Apply volume changes to CDDA/CDXA audio? */
}

uint32_t psx_cdrom_read32(psx_cdrom_t* cdrom, uint32_t addr) {
    assert(("32-bit CDROM reads are not supported", 0));
}

uint32_t psx_cdrom_read16(psx_cdrom_t* cdrom, uint32_t addr) {
    assert(("16-bit CDROM reads are not supported", 0));

    // The CDROM controller is connected to the SUB-BUS which is a 16-bit
    // bus, but the output from the controller itself is 8-bit. I think
    // 16-bit accesses are handled as a pair of 8-bit accesses
    return psx_cdrom_read8(cdrom, addr) << 8 | psx_cdrom_read8(cdrom, addr+1);
}

void psx_cdrom_write32(psx_cdrom_t* cdrom, uint32_t addr, uint32_t value) {
    assert(("32-bit CDROM writes are not supported", 0));
}

void psx_cdrom_write16(psx_cdrom_t* cdrom, uint32_t addr, uint32_t value) {
    assert(("16-bit CDROM writes are not supported", 0));
}

#define VALID_BCD(bcd) \
    (((bcd & 0xf0) <= 0x90) && ((bcd & 0xf) <= 9))

#define VALID_MSF(m, s, f) \
    (VALID_BCD(m) && VALID_BCD(s) && VALID_BCD(f))

// +----------------------+
// | Commands             |
// +----------------------+

void cdrom_cmd_getstat(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_setloc(psx_cdrom_t* cdrom) {
    int m = queue_pop(cdrom->parameters);
    int s = queue_pop(cdrom->parameters);
    int f = queue_pop(cdrom->parameters);

    if (!VALID_MSF(m, s, f)) {
        // Error: Invalid MSF

        return;
    }

    cdrom->pending_setloc = (BTOI(m) * 4500) + (BTOI(s) * 75) + BTOI(f);
}

void cdrom_cmd_play(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_forward(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_backward(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_readn(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_motoron(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_stop(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_pause(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_init(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_mute(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_demute(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_setfilter(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_setmode(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_getparam(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_getlocl(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_getlocp(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_setsession(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_gettn(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_gettd(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_seekl(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_seekp(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_test(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_getid(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_reads(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_reset(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_getq(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_readtoc(psx_cdrom_t* cdrom) {

}

void cdrom_cmd_videocd(psx_cdrom_t* cdrom) {

}