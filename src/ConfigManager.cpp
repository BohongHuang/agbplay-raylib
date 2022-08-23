#include <fstream>
#include <regex>
#include <cstring>
#include <iostream>
#include <cstdlib>

// sorry, for some reason multiple versions of jsoncpp use different paths :/
#if __has_include(<json/json.h>)
#include <json/json.h>
#include <json/writer.h>
#else
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#endif

#include "ConfigManager.h"
#include "Util.h"
#include "Xcept.h"
#include "Debug.h"

ConfigManager::ConfigManager(const std::string& gameCode): ConfigManager()
{
    SetGameCode(gameCode);
}

GameConfig& ConfigManager::GetCfg()
{
    if (curCfg)
        return *curCfg;
    else
        throw Xcept("Trying to get the game config without setting the game code");
}

const GameConfig& ConfigManager::GetCfg() const
{
    if (curCfg)
        return *curCfg;
    else
        throw Xcept("Trying to get the game config without setting the game code");
}

void ConfigManager::SetGameCode(const std::string& gameCode)
{
    for (GameConfig& config : configs)
    {
        const auto &gameCodesToCheck = config.GetGameCodes();
        if (std::find(gameCodesToCheck.begin(), gameCodesToCheck.end(), gameCode) != gameCodesToCheck.end()) {
            curCfg = &config;
            return;
        }
    }
    configs.emplace_back(gameCode);
    curCfg = &configs.back();
}

const std::filesystem::path& ConfigManager::GetWavOutputDir()
{
    return confWavOutputDir;
}

CGBPolyphony ConfigManager::GetCgbPolyphony() const
{
    return confCgbPolyphony;
}

int8_t ConfigManager::GetMaxLoopsPlaylist() const
{
    return maxLoopsPlaylist < -1 ? 0 : maxLoopsPlaylist;
}

void ConfigManager::SetMaxLoopsPlaylist(int8_t maxLoops) {
    this->maxLoopsPlaylist = maxLoops;
}

int8_t ConfigManager::GetMaxLoopsExport() const
{
    return maxLoopsExport < 0 ? 0 : maxLoopsExport;
}

double ConfigManager::GetPadSecondsStart() const
{
    return padSecondsStart;
}

double ConfigManager::GetPadSecondsEnd() const
{
    return padSecondsEnd;
}
