#include "agbplay.h"
#include "ConfigManager.h"
#include "PlayerInterface.h"

class PlayerHandle {
public:
    explicit PlayerHandle(std::filesystem::path const & filepath);
    PlayerHandle(uint8_t *rom, size_t size);
    std::shared_ptr<Rom> rom;
    std::shared_ptr<ConfigManager> config;
    std::shared_ptr<PlayerInterface> playerInterface;
    std::unique_ptr<SongTable> songTable;
};

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
    player->handle = static_cast<void*>(new PlayerHandle(data, size));
    return player;
}

Player *PlayerCreateFromPath(const char *path) {
    auto *player = new Player;
    player->handle = static_cast<void*>(new PlayerHandle(path));
    return player;
}

void PlayerDelete(Player* player) {
    delete (PlayerHandle*)player->handle;
    delete player;
}

void PlayerPlay(Player* player) {
    ((PlayerHandle*)player->handle)->playerInterface->Play();
}

void PlayerPause(Player* player) {
    ((PlayerHandle*)player->handle)->playerInterface->Pause();
}

void PlayerStop(Player* player) {
    ((PlayerHandle*)player->handle)->playerInterface->Stop();
    while (((PlayerHandle*)player->handle)->playerInterface->GetPlayerState() != PlayerState::TERMINATED &&
           ((PlayerHandle*)player->handle)->playerInterface->GetPlayerState() != PlayerState::THREAD_DELETED)
        ((PlayerHandle*)player->handle)->playerInterface->Stop();
}

bool PlayerIsPlaying(Player* player) {
    return ((PlayerHandle*)player->handle)->playerInterface->IsPlaying();
}

void PlayerSetSong(Player* player, uint16_t uid) {
    ((PlayerHandle*)player->handle)->playerInterface->LoadSong(((PlayerHandle*)player->handle)->songTable->GetPosOfSong(uid));
}

size_t PlayerGetSongNumber(Player *player) {
    return ((PlayerHandle*)player->handle)->songTable->GetNumSongs();
}

void PlayerTakeBuffer(Player* player, void* buffer, size_t size) {
    ((PlayerHandle*)player->handle)->playerInterface->GetBuffer().Take(static_cast<sample*>(buffer), size);
}
