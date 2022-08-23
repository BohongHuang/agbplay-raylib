#ifndef AGBPLAY_H
#define AGBPLAY_H

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *handle;
} AgbPlayer;

AgbPlayer *AgbPlayerCreateFromRomData(uint8_t *, size_t);
AgbPlayer *AgbPlayerCreateFromPath(const char *);
void AgbPlayerDelete(AgbPlayer *);
void AgbPlayerPlay(AgbPlayer *);
void AgbPlayerPause(AgbPlayer *);
void AgbPlayerStop(AgbPlayer *);
bool AgbPlayerIsPlaying(AgbPlayer *);
void AgbPlayerSetSong(AgbPlayer *, uint16_t);
size_t AgbPlayerGetSongNumber(AgbPlayer *);
void AgbPlayerTakeBuffer(AgbPlayer *, void *, size_t);

#ifdef __cplusplus
};
#endif

#endif    // AGBPLAY_H
