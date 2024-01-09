#include <stdio.h>

#include "cue.h"

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

int main(int argc, const char* argv[]) {
    cue_t* cue = cue_create();
    cue_init(cue, FM_FILE);
    cue_parse(cue, argv[1]);

    printf("Parsed CUE file \'%s\'. Track count: %llu\n",
        argv[1],
        cue->tracks->size,
        cue->files->size
    );

    printf("Tracks:\n");

    // node_t* track = list_front(cue->tracks);

    // while (track) {
    //     cue_track_t* ct = track->data;

    //     printf("    track %u: mode=%s [0]=%u [1]=%u in \'%s\'\n",
    //         ct->number,
    //         m_cue_keywords[ct->mode],
    //         ct->index[0],
    //         ct->index[1],
    //         ct->file->name
    //     );

    //     track = track->next;
    // }

    cue_load(cue);

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

    cue_destroy(cue);

    return 0;
}