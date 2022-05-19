#include "LSound.h"
//Game sound
void LSound::free() {
    //Free the sound effects
    Mix_FreeChunk(gLoseSound);
    Mix_FreeChunk(gGainSound);
    Mix_FreeChunk(gPassSound);
    Mix_FreeChunk(gJumpSound);
    gLoseSound = NULL;
    gGainSound = NULL;
    gPassSound = NULL;
    gJumpSound = NULL;

    //Free the music
    Mix_FreeMusic(gBgm);
    gBgm = NULL;
}
LSound::~LSound() {
    free();
}
bool LSound::isPlayingMusic() {
    return Mix_PlayingMusic();
}
bool LSound::isPausedMusic() {
    return Mix_PausedMusic();
}
void LSound::StopMusic() {
    Mix_HaltMusic();
}
void LSound::PlayMusic() {
    if (!isPlayingMusic())
        Mix_PlayMusic(gBgm, -1);
    Mix_VolumeMusic(volumeMusic);
}
void LSound::PauseMusic() {
    if (isPlayingMusic())
        Mix_PauseMusic();
}
void LSound::ResumeMusic() {
    if (isPausedMusic())
        Mix_ResumeMusic();
}
void LSound::PlayJumpSound() {
    Mix_PlayChannel(-1, gJumpSound, 0);
    Mix_VolumeChunk(gJumpSound, volumeChunk);
}
void LSound::PlayLoseSound() {
    Mix_PlayChannel(-1, gLoseSound, 0);
    Mix_VolumeChunk(gLoseSound, volumeChunk);
}
void LSound::PlayGainSound() {
    Mix_PlayChannel(-1, gGainSound, 0);
    Mix_VolumeChunk(gGainSound, volumeChunk);
}
void LSound::PlayPassSound() {
    Mix_PlayChannel(-1, gPassSound, 0);
    Mix_VolumeChunk(gPassSound, volumeChunk);
}
void LSound::setVolumeMusic(const int& v) {
    volumeMusic = v;
    Mix_VolumeMusic(volumeMusic);
}
void LSound::setVolumeChunk(const int& v) {
    volumeChunk = v;
    Mix_VolumeChunk(gJumpSound, volumeChunk);
    Mix_VolumeChunk(gLoseSound, volumeChunk);
    Mix_VolumeChunk(gPassSound, volumeChunk);
    Mix_VolumeChunk(gGainSound, volumeChunk);
}
void LSound::loadMedia(bool& success) {
    gBgm = Mix_LoadMUS("sound/bgm.wav");
    if (!gBgm) {
        cout << "Failed to load bgm" << Mix_GetError() << endl;
        success = false;
    }
    gJumpSound = Mix_LoadWAV("sound/jumpsound.wav");
    if (!gJumpSound) {
        cout << "Failed to load jumpsound1" << Mix_GetError() << endl;
        success = false;
    }
    gPassSound = Mix_LoadWAV("sound/passsound.wav");
    if (!gPassSound) {
        cout << "Failed to load gPassSound" << Mix_GetError() << endl;
        success = false;
    }
    gLoseSound = Mix_LoadWAV("sound/losesound.wav");
    if (!gLoseSound) {
        cout << "Failed to load losesound" << Mix_GetError() << endl;
        success = false;
    }
    gGainSound = Mix_LoadWAV("sound/gainsound.wav");
    if (!gGainSound) {
        cout << "Failed to load gGainSound" << Mix_GetError() << endl;
        success = false;
    }
}
void LSound::turnOffSound() {
    savedVolumeMusic = volumeMusic;
    savedVolumeChunk = volumeChunk;
    volumeChunk = volumeMusic = 0;
    setVolumeChunk(volumeChunk);
    setVolumeMusic(volumeMusic);
}
void LSound::turnOnSound() {
    volumeMusic = savedVolumeMusic;
    volumeChunk = savedVolumeChunk;
    setVolumeChunk(volumeChunk);
    setVolumeMusic(volumeMusic);
}
int LSound::getVolumeMusic() const {
    return volumeMusic;
}
int LSound::getVolumeChunk() const {
    return volumeChunk;
}