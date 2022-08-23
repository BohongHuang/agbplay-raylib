//
// Created by coco24 on 22-8-23.
//
#include "ConfigManager.h"
#include "PlayerInterface.h"

#ifndef AGBPLAY_CITERFACE_H
#define AGBPLAY_CITERFACE_H

class PlayerHandle {
public:
    explicit PlayerHandle(std::filesystem::path const & filepath);
    PlayerHandle(uint8_t *rom, size_t size);
    std::shared_ptr<Rom> rom;
    std::shared_ptr<ConfigManager> config;
    std::shared_ptr<PlayerInterface> playerInterface;
    std::unique_ptr<SongTable> songTable;
};

#ifdef __cplusplus
extern "C" {
#endif


struct Player {
    PlayerHandle *handle;
};

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

#endif    // AGBPLAY_CITERFACE_H
