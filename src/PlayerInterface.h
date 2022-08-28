#pragma once

#include <cstdint>
#include <vector>
#include <thread>
#include <memory>

#include "Constants.h"
#include "GameConfig.h"
#include "Ringbuffer.h"
#include "LoudnessCalculator.h"
#include "PlayerContext.h"
#include "Rom.h"
#include "ConfigManager.h"

typedef volatile enum class State : int {
    RESTART, PLAYING, PAUSED, TERMINATED, SHUTDOWN, THREAD_DELETED
} PlayerState;

class PlayerInterface 
{
public:
    PlayerInterface(std::shared_ptr<Rom> rom, size_t initSongPos, const std::shared_ptr<ConfigManager>& config);
    PlayerInterface(const PlayerInterface&) = delete;
    PlayerInterface& operator=(const PlayerInterface&) = delete;
    ~PlayerInterface();

    void LoadSong(size_t songPos);
    void Play();
    void Pause();
    void Stop();
    void SpeedDouble();
    void SpeedHalve();
    bool IsPlaying();
    void UpdateView();
    void ToggleMute(size_t index);
    void Mute(size_t index, bool mute);
    size_t GetMaxTracks() { return mutedTracks.size(); }
    void GetMasterVolLevels(float& left, float& right);
    SongInfo GetSongInfo() const;
    const std::shared_ptr<Rom> &GetRom() const;
    const std::shared_ptr<ConfigManager> &GetConfig() const;
    Ringbuffer& GetBuffer();
    PlayerState GetPlayerState();

private:
    void initContext();
    void threadWorker();

    void setupLoudnessCalcs();

    std::shared_ptr<Rom> rom;
    std::shared_ptr<ConfigManager> config;
    uint32_t speedFactor = 64;
    PlayerState playerState = State::THREAD_DELETED;
    std::unique_ptr<PlayerContext> ctx;


    LoudnessCalculator masterLoudness{10.0f};
    std::vector<LoudnessCalculator> trackLoudness;
    std::vector<bool> mutedTracks;

    Ringbuffer rBuf;

    std::unique_ptr<std::thread> playerThread;
};
