#ifndef AGBPLAY_H
#define AGBPLAY_H

#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    size_t bufferSize;
    int8_t maxLoopCount;
} AgbPlayerConfig;

typedef struct {
    void *handle;
} AgbPlayer;

AgbPlayer AgbPlayerCreateFromRomData(uint8_t * rom, size_t size, AgbPlayerConfig config);
AgbPlayer AgbPlayerCreateFromPath(const char * path, AgbPlayerConfig config);
void AgbPlayerDelete(AgbPlayer player);
void AgbPlayerPlay(AgbPlayer player);
void AgbPlayerPause(AgbPlayer player);
void AgbPlayerStop(AgbPlayer player);
bool AgbPlayerIsPlaying(AgbPlayer player);
void AgbPlayerSetSong(AgbPlayer player, uint16_t songId);
size_t AgbPlayerGetSongNumber(AgbPlayer player);
void AgbPlayerTakeBuffer(AgbPlayer player, void *buffer, size_t size);

#ifdef __cplusplus
};
#endif

#endif    // AGBPLAY_H
