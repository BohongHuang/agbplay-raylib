//
// Created by coco24 on 22-8-23.
//

#include "CInterface.h"

PlayerHandle::PlayerHandle(std::filesystem::path const & filepath): rom(std::make_shared<Rom>(filepath)),
                                                                    config(std::make_shared<ConfigManager>(rom->GetROMCode())),
                                                                    playerInterface(std::make_shared<PlayerInterface>(rom, 0, config)),
                                                                    songTable(std::make_unique<SongTable>(rom)) {}

PlayerHandle::PlayerHandle(uint8_t *rom, size_t size):  rom(std::make_shared<Rom>(rom, size)),
                                                        config(std::make_shared<ConfigManager>(this->rom->GetROMCode())),
                                                        playerInterface(std::make_shared<PlayerInterface>(this->rom, 0, config)),
                                                        songTable(std::make_unique<SongTable>(this->rom)) {}

Player *PlayerCreateFromRomData(uint8_t *data, size_t size) {
    auto *player = new Player;
    player->handle = new PlayerHandle(data, size);
    return player;
}

Player *PlayerCreateFromPath(const char *path) {
    auto *player = new Player;
    player->handle = new PlayerHandle(path);
    return player;
}

void PlayerDelete(Player* player) {
    delete player->handle;
    delete player;
}

void PlayerPlay(Player* player) {
    player->handle->playerInterface->Play();
}

void PlayerPause(Player* player) {
    player->handle->playerInterface->Pause();
}

void PlayerStop(Player* player) {
    player->handle->playerInterface->Stop();
    while (player->handle->playerInterface->GetPlayerState() != PlayerState::TERMINATED &&
           player->handle->playerInterface->GetPlayerState() != PlayerState::THREAD_DELETED)
        player->handle->playerInterface->Stop();
}

bool PlayerIsPlaying(Player* player) {
    player->handle->playerInterface->IsPlaying();
}

void PlayerSetSong(Player* player, uint16_t uid) {
    player->handle->playerInterface->LoadSong(player->handle->songTable->GetPosOfSong(uid));
}

size_t PlayerGetSongNumber(Player *player) {
    return player->handle->songTable->GetNumSongs();
}

void PlayerTakeBuffer(Player* player, void* buffer, size_t size) {
    player->handle->playerInterface->GetBuffer().Take(static_cast<sample*>(buffer), size);
}

