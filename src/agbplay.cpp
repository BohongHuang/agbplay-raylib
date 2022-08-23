#include "CInterface.h"
#include "raylib.h"

Player *player;

void AudioInputCallback(void *buffer, unsigned int frames)
{
    PlayerTakeBuffer(player, buffer, frames);
}

int main(int argc, char *argv[])
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [audio] example - music playing (streaming)");

    player = PlayerCreateFromPath(argv[1]);

    int songId = argc > 2 ? atoi(argv[2]) : 0;
    int songCount = PlayerGetSongNumber(player);

    PlayerSetSong(player, songId);

    InitAudioDevice();

    PlayerPlay(player);

    AudioStream stream = LoadAudioStream(48000, 32, 2);

    SetAudioStreamCallback(stream, AudioInputCallback);

    PlayAudioStream(stream);

    char sprintfBuffer[32];

    SetTargetFPS(30);

    while (!WindowShouldClose())
    {
        ClearBackground(RAYWHITE);

        if (IsKeyPressed(KEY_UP)) {
            songId--;
            if (songId < 0) {
                songId = 0;
            }
            PlayerSetSong(player, songId);
        }

        if (IsKeyPressed(KEY_LEFT)) {
            songId -= 10;
            if (songId < 0) {
                songId = 0;
            }
            PlayerSetSong(player, songId);
        }

        if (IsKeyPressed(KEY_DOWN)) {
            songId++;
            if (songId >= songCount) {
                songId = songCount - 1;
            }
            PlayerSetSong(player, songId);
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            songId += 10;
            if (songId >= songCount) {
                songId = songCount - 1;
            }
            PlayerSetSong(player, songId);
        }

        if (IsKeyPressed(KEY_SPACE)) {
            PlayerPause(player);
        }

        BeginDrawing();

        sprintf(sprintfBuffer, "Current Song: %d", songId);
        DrawText(sprintfBuffer, 50, 50, 32, BLACK);

        DrawFPS(0, 0);

        EndDrawing();
    }

    PlayerStop(player);

    CloseAudioDevice();

    CloseWindow();

    PlayerDelete(player);

    return 0;
}
