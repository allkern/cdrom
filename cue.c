#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "cue.h"

static const char* cue_keywords[] = {
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

int cue_parse_keyword(cue_t* cue) {
    char buf[256];
    char* ptr = buf;

    while (isalpha(cue->c) || isdigit(cue->c) || cue->c == '/') {
        *ptr++ = cue->c;

        cue->c = fgetc(cue->file);
    }

    *ptr = '\0';

    int i = 0;

    const char* keyword = cue_keywords[i];

    while (keyword) {
        if (!strcmp(keyword, buf)) {
            return i;
        } else {
            keyword = cue_keywords[++i];
        }
    }

    return -1;
}

int cue_parse_number(cue_t* cue) {
    if (!isdigit(cue->c))
        return 0;

    char buf[4];

    char* ptr = buf;

    while (isdigit(cue->c)) {
        *ptr++ = cue->c;

        cue->c = fgetc(cue->file);
    }

    *ptr = '\0';

    return atoi(buf);
}

uint32_t cue_parse_msf(cue_t* cue) {
    int m = 0;
    int s = 0;
    int f = 0;

    if (!isdigit(cue->c))
        return 0;

    m = cue_parse_number(cue);

    if (cue->c != ':')
        return 0;

    cue->c = fgetc(cue->file);

    s = cue_parse_number(cue);

    if (cue->c != ':')
        return 0;

    cue->c = fgetc(cue->file);

    f = cue_parse_number(cue);

    // 1 second = 75 frames (sectors)
    // 1 minute = 60 seconds = 4500 frames
    return f + (s * 75) + (m * 4500);
}

void cue_parse_index(cue_t* cue) {
    cue_track_t* track = list_back(cue->tracks)->data;

    while (isspace(cue->c))
        cue->c = fgetc(cue->file);

    if (!isdigit(cue->c))
        return;

    int i = cue_parse_number(cue);

    while (isspace(cue->c))
        cue->c = fgetc(cue->file);

    if (i > 1)
        return;

    track->index[i] = cue_parse_msf(cue);
}

cue_track_t* cue_parse_track(cue_t* cue) {
    while (isspace(cue->c))
        cue->c = fgetc(cue->file);

    if (!isdigit(cue->c))
        return NULL;

    cue_track_t* track = malloc(sizeof(cue_track_t));

    track->end = 0;
    track->start = 0;
    track->pregap = 0;
    track->index[0] = 0;
    track->index[1] = 0;
    track->file = list_back(cue->files)->data;
    track->number = cue_parse_number(cue);

    while (isspace(cue->c))
        cue->c = fgetc(cue->file);

    track->mode = cue_parse_keyword(cue);

    return track;
}

cue_file_t* cue_parse_file(cue_t* cue) {
    while (isspace(cue->c))
        cue->c = fgetc(cue->file);

    if (cue->c != '\"')
        return NULL;

    cue_file_t* file = malloc(sizeof(cue_file_t));

    file->tracks = list_create();
    file->name = malloc(512);

    char* ptr = file->name;

    cue->c = fgetc(cue->file);

    while (cue->c != '\"') {
        *ptr++ = cue->c;

        cue->c = fgetc(cue->file);
    }

    *ptr = '\0';

    cue->c = fgetc(cue->file);

    // Ignore file type
    while (isspace(cue->c))
        cue->c = fgetc(cue->file);

    while (isalpha(cue->c))
        cue->c = fgetc(cue->file);

    return file;
}

cue_t* cue_create(void) {
    return malloc(sizeof(cue_t));
}

void cue_init(cue_t* cue) {
    cue->files = list_create();
    cue->tracks = list_create();
}

int cue_parse(cue_t* cue, const char* path) {
    cue->file = fopen(path, "rb");

    if (!cue->file)
        return 1;

    cue->c = fgetc(cue->file);

    while (isspace(cue->c))
        cue->c = fgetc(cue->file);

    while (!feof(cue->file)) {
        int kw = cue_parse_keyword(cue);

        switch (kw) {
            case CUE_FILE: {
                list_push_back(cue->files, cue_parse_file(cue));
            } break;

            case CUE_TRACK: {
                cue_track_t* track = cue_parse_track(cue);
                cue_file_t* file = list_back(cue->files)->data;

                list_push_back(cue->tracks, track);
                list_push_back(file->tracks, track);
            } break;

            case CUE_INDEX: {
                cue_parse_index(cue);
            } break;

            default: {
                printf("Unknown keyword: %s (%u)\n", cue_keywords[kw], kw);

                return 1;
            } break;
        }

        while (isspace(cue->c))
            cue->c = fgetc(cue->file);
    }

    return 0;
}

size_t get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);

    size_t size = ftell(file);

    fseek(file, 0, SEEK_SET);

    return size;
}

uint32_t init_tracks(cue_file_t* file, uint32_t lba) {
    node_t* node = list_front(file->tracks);

    // if (file->tracks->size == 1) {
    //     cue_track_t* data = node->data;

    //     data->pregap = data->index[1] - data->index[0];
    //     data->start = 
    //     data->end = lba + (file->size / 0x930);
    // }

    uint32_t pregap = 0;

    while (node) {
        cue_track_t* data = node->data;

        if ((file->tracks->size == 1) && !data->pregap)
            data->pregap = data->index[1] - data->index[0];

        data->start = lba + data->index[1];

        if (node->next) {
            cue_track_t* next = node->next->data;

            data->end = lba + next->index[1];
        } else {
            if (file->tracks->size == 1) {
                data->end = lba + data->index[1] + (file->size / 0x930);
            } else {
                data->end = lba + (file->size / 0x930);
            }
        }

        pregap += data->pregap;

        node = node->next;
    }

    return pregap;
}

void cue_load(cue_t* cue, int mode) {
    node_t* node = list_front(cue->files);

    // 00:02:00
    uint32_t lba = 2 * 75;

    while (node) {
        cue_file_t* data = node->data;

        FILE* file = fopen(data->name, "rb");

        if (!file) {
            perror("Could not open file");

            exit(1);
        }

        data->buf_mode = mode;
        data->size = get_file_size(file);

        printf("Loaded \'%s\': size=%llx, sectors=%llu\n",
            data->name,
            data->size,
            data->size / 0x930
        );

        if (data->buf_mode == LD_BUFFERED) {
            data->buf = malloc(data->size);

            fseek(file, 0, SEEK_SET);
            fread(data->buf, 1, data->size, file);

            fclose(file);
        } else {
            data->buf = file;
        }

        data->start = lba + init_tracks(data, lba);

        lba += data->size / 0x930;

        node = node->next;
    }
}

void cue_destroy(cue_t* cue) {
    node_t* node = list_front(cue->files);

    while (node) {
        cue_file_t* file = node->data;

        if (file->buf_mode == LD_BUFFERED) {
            free(file->buf);
        } else {
            fclose((FILE*)file->buf);
        }

        list_destroy(file->tracks);

        free(file->name);
        free(file);

        node = node->next;
    }

    list_destroy(cue->files);

    node = list_front(cue->tracks);

    while (node) {
        free(node->data);

        node = node->next;
    }

    list_destroy(cue->tracks);

    free(cue);
}

cue_track_t* get_sector_track(cue_t* cue, uint32_t lba) {
    node_t* node = list_front(cue->tracks);

    while (node) {
        cue_track_t* track = node->data;

        if ((lba >= track->start) && (lba < track->end))
            return track;

        node = node->next;
    }

    return NULL;
}

int cue_read(cue_t* cue, uint32_t lba, void* buf) {
    if (lba >= ((cue_track_t*)list_back(cue->tracks)->data)->end)
        return TS_FAR;

    cue_track_t* track = get_sector_track(cue, lba);

    // If the LBA isn't too far but the track wasn't found
    // then we are being requested a pregap sector. Clear buffer
    // and initialize sync data (not actually needed)
    if (!track) {
        memset(buf, 0, 2352);
        memset(buf + 1, 255, 10);

        return TS_PREGAP;
    }

    cue_file_t* file = track->file;

    printf("Reading sector %u at track %u, file=%s (%u), offset=%u (%08x)\n",
        lba,
        track->number,
        track->file->name,
        file->start,
        lba - file->start,
        (lba - file->start) * 2352
    );

    if (file->buf_mode == LD_BUFFERED) {
        uint8_t* ptr = file->buf + ((lba - file->start) * 2352);

        memcpy(buf, ptr, 2352);
    } else {
        fseek(file->buf, (lba - file->start) * 2352, SEEK_SET);

        fread(buf, 1, 2352, file->buf);
    }

    return (track->mode == CUE_MODE2_2352) ? TS_DATA : TS_AUDIO;
}