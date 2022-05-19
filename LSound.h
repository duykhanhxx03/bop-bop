#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <fstream>
#include <iostream>
using namespace std;
class LSound {
private:
    Mix_Music* gBgm;
    Mix_Chunk* gLoseSound;
    Mix_Chunk* gJumpSound;
    Mix_Chunk* gPassSound;
    Mix_Chunk* gGainSound;
    //MIX_MAX_VOLUME=128
    int volumeMusic = 128;
    int volumeChunk = 128;

    int savedVolumeMusic = volumeMusic;
    int savedVolumeChunk = volumeChunk;
public:
    bool isPlayingMusic();
    bool isPausedMusic();
    void StopMusic();
    void PlayMusic();
    void PlayPassSound();
    void PauseMusic();
    void ResumeMusic();
    void PlayJumpSound();
    void PlayGainSound();
    void PlayLoseSound();
    int getVolumeMusic() const;
    int getVolumeChunk() const;
    void setVolumeMusic(const int& v);
    void setVolumeChunk(const int& v);
    void turnOffSound();
    void turnOnSound();
    void loadMedia(bool& success);
    void free();
    ~LSound();
};
static LSound gSound;