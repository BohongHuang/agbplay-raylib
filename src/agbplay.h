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
} Player;

Player *PlayerCreateFromRomData(uint8_t *, size_t);
Player *PlayerCreateFromPath(const char *);
void PlayerDelete(Player*);
void PlayerPlay(Player*);
void PlayerPause(Player*);
void PlayerStop(Player*);
bool PlayerIsPlaying(Player*);
void PlayerSetSong(Player*, uint16_t);
size_t PlayerGetSongNumber(Player*);
void PlayerTakeBuffer(Player*, void*, size_t);

#ifdef __cplusplus
};
#endif

#endif    // AGBPLAY_H
