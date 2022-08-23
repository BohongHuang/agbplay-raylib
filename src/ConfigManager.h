#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include "GameConfig.h"
#include "Constants.h"

class ConfigManager
{
public:
    ConfigManager() = default;
    ConfigManager(std::string const &);
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    GameConfig& GetCfg();
    const GameConfig& GetCfg() const;
    void SetGameCode(const std::string& gameCode);
    const std::filesystem::path& GetWavOutputDir();
    CGBPolyphony GetCgbPolyphony() const;
    int8_t GetMaxLoopsPlaylist() const;
    void SetMaxLoopsPlaylist(int8_t maxLoops);
    int8_t GetMaxLoopsExport() const;
    double GetPadSecondsStart() const;
    double GetPadSecondsEnd() const;
private:
    std::vector<GameConfig> configs;
    std::filesystem::path confWavOutputDir;
    CGBPolyphony confCgbPolyphony;
    int8_t maxLoopsPlaylist = -1;
    int8_t maxLoopsExport = 2;
    size_t streamBufferSize = STREAM_BUF_SIZE;

public:
    size_t GetStreamBufferSize() const;
    void SetStreamBufferSize(size_t streamBufferSize);

private:
    std::filesystem::path configPath;
    GameConfig *curCfg = nullptr;
    double padSecondsStart;
    double padSecondsEnd;
};
