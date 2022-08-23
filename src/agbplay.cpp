#include "agbplay.h"
#include "ConfigManager.h"
#include "PlayerInterface.h"

class AgbPlayerHandle
{
public:
    explicit AgbPlayerHandle(std::filesystem::path const & filepath);
    AgbPlayerHandle(uint8_t *rom, size_t size);
    std::shared_ptr<Rom> rom;
    std::shared_ptr<ConfigManager> config;
    std::shared_ptr<PlayerInterface> playerInterface;
    std::unique_ptr<SongTable> songTable;
};

AgbPlayerHandle::AgbPlayerHandle(std::filesystem::path const & filepath): rom(std::make_shared<Rom>(filepath)),
                                                                    config(std::make_shared<ConfigManager>(rom->GetROMCode())),
                                                                    playerInterface(std::make_shared<PlayerInterface>(rom, 0, config)),
                                                                    songTable(std::make_unique<SongTable>(rom)) {}

AgbPlayerHandle::AgbPlayerHandle(uint8_t *rom, size_t size):  rom(std::make_shared<Rom>(rom, size)),
                                                        config(std::make_shared<ConfigManager>(this->rom->GetROMCode())),
                                                        playerInterface(std::make_shared<PlayerInterface>(this->rom, 0, config)),
                                                        songTable(std::make_unique<SongTable>(this->rom)) {}

AgbPlayer *AgbPlayerCreateFromRomData(uint8_t *data, size_t size) {
    auto *player = new AgbPlayer;
    player->handle = static_cast<void*>(new AgbPlayerHandle(data, size));
    return player;
}

AgbPlayer *AgbPlayerCreateFromPath(const char *path) {
    auto *player = new AgbPlayer;
    player->handle = static_cast<void*>(new AgbPlayerHandle(path));
    return player;
}

void AgbPlayerDelete(AgbPlayer * player) {
    delete (AgbPlayerHandle *)player->handle;
    delete player;
}

void AgbPlayerPlay(AgbPlayer * player) {
    ((AgbPlayerHandle *)player->handle)->playerInterface->Play();
}

void AgbPlayerPause(AgbPlayer * player) {
    ((AgbPlayerHandle *)player->handle)->playerInterface->Pause();
}

void AgbPlayerStop(AgbPlayer * player) {
    ((AgbPlayerHandle *)player->handle)->playerInterface->Stop();
}

bool AgbPlayerIsPlaying(AgbPlayer * player) {
    return ((AgbPlayerHandle *)player->handle)->playerInterface->IsPlaying();
}

void AgbPlayerSetSong(AgbPlayer * player, uint16_t uid) {
    ((AgbPlayerHandle *)player->handle)->playerInterface->LoadSong(((AgbPlayerHandle *)player->handle)->songTable->GetPosOfSong(uid));
}

size_t AgbPlayerGetSongNumber(AgbPlayer *player) {
    return ((AgbPlayerHandle *)player->handle)->songTable->GetNumSongs();
}

void AgbPlayerTakeBuffer(AgbPlayer * player, void* buffer, size_t size) {
    ((AgbPlayerHandle *)player->handle)->playerInterface->GetBuffer().Take(static_cast<sample*>(buffer), size);
}
