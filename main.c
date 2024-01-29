#include <stdlib.h>
#include <stdio.h>

#include "cue.h"
#include "queue.h"

static const char* m_cue_keywords[] = {
    "4CH",
    "AIFF",
    "AUDIO",
    "BINARY",
    "CATALOG",
    "CDG",
    "CDI/2336",
    "CDI/2352",
    "CDTEXTFILE",
    "DCP",
    "FILE",
    "FLAGS",
    "INDEX",
    "ISRC",
    "MODE1/2048",
    "MODE1/2352",
    "MODE2/2336",
    "MODE2/2352",
    "MOTOROLA",
    "MP3",
    "PERFORMER",
    "POSTGAP",
    "PRE",
    "PREGAP",
    "REM",
    "SCMS",
    "SONGWRITER",
    "TITLE",
    "TRACK",
    "WAVE",
    0
};

#define MSF_TO_LBA(m, s, f) ((m * 4500) + (s * 75) + f)

int main(int argc, const char* argv[]) {
    cue_t* cue = cue_create();
    cue_init(cue);
    cue_parse(cue, argv[1]);

    printf("Parsed CUE file \'%s\'. Track count: %llu\n",
        argv[1],
        cue->tracks->size
    );

    cue_load(cue, LD_BUFFERED);

    printf("Loaded CUE image\n");

    node_t* track = list_front(cue->tracks);

    while (track) {
        cue_track_t* ct = track->data;

        printf("    track %u: mode=%s start=%u end=%u pregap=%u in \'%s\'\n",
            ct->number,
            m_cue_keywords[ct->mode],
            ct->start,
            ct->end,
            ct->pregap,
            ct->file->name
        );

        track = track->next;
    }

    uint8_t* buf = malloc(2352);

    int lba = MSF_TO_LBA(0, 2, 0);

    cue_read(cue, lba, buf);

    for (int y = 0; y < 0x10; y++) {
        printf("%08x: ", ((lba - 150) * 0x930) + (y << 4));

        for (int x = 0; x < 0x10; x++) {
            printf("%02x ", buf[x + (y * 0x10)]);
        }

        printf("\n");
    }

    free(buf);

    printf("Destroying CUE... ");

    cue_destroy(cue);

    printf("done\n");

    return 0;
}