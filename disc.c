#include "disc.h"
#include "cue.h"

const char* disc_cd_extensions[] = {
    "cue",
    "bin",
    0
};

psx_disc_t* psx_disc_create(void) {
    return malloc(sizeof(psx_disc_t));
}

int disc_get_extension(const char* path) {
    const char* ptr = &path[strlen(path) - 1];
    int i = 0;

    while ((*ptr != '.') && (ptr != path))
        ptr--;
    
    if (ptr == path)
        return CD_EXT_UNSUPPORTED;

    while (disc_cd_extensions[i]) {
        if (!strcmp(ptr + 1, disc_cd_extensions[i]))
            return i;
        
        ++i;
    }

    return CD_EXT_UNSUPPORTED;
}

int disc_get_cd_type(psx_disc_t* disc) {
    char buf[CD_SECTOR_SIZE];

    // If the disc is smaller than 16 sectors
    // then it can't be a PlayStation game.
    // Audio discs should also have ISO volume
    // descriptors, so it's probably something else
    // entirely.
    if (!psx_disc_read(disc, 166, buf))
        return CDT_UNKNOWN;

    // Check for the "PLAYSTATION" string at PVD offset 20h
    // Patch 20 byte so comparison is done correctly
    buf[0x2b] = 0;

    if (strncmp(&buf[0x20], "PLAYSTATION", 12))
        return CDT_AUDIO;

    return CDT_LICENSED;
}

int psx_disc_open(psx_disc_t* disc, const char* path) {
    int ext = disc_get_extension(path);

    switch (ext) {
        case CD_EXT_CUE: {
            cue_t* cue = cue_create();

            cue_init(cue);

            if (cue_parse(cue, path))
                return CDT_ERROR;

            cue_load(cue, LD_FILE);
        } break;
    }

    return disc_get_cd_type(disc);
}

int psx_disc_read(psx_disc_t* disc, uint32_t lba, void* buf) {
    return disc->read(disc->udata, lba, buf);
}

void psx_disc_destroy(psx_disc_t* disc) {
    free(disc);
}