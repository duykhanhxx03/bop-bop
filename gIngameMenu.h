#pragma once
#include "LTexture.h"
#include "LButton.h"
#include <iostream>
using namespace std;

class gIngameMenu {
private:
    //Start menu texture
    LTexture PauseTexture;
    LTexture OptionsTexture;
    LTexture SoundTexture;

    //Button
    LButton Pause;
    LButton Home;
    LButton Sound;

    //Clips
    SDL_Rect PauseSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect OptionsSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect SoundSpriteClips[BUTTON_SPRITE_TOTAL];
public:
    //status
    bool isPauseOn;
    bool isSoundOn;
    bool isOptionsOn;

    gIngameMenu();
    void handleEvent(SDL_Event& e, GAME_STATUS& gameStatus);
    void show(SDL_Renderer* gRenderer);
    void loadMedia(bool& success, SDL_Renderer* gRenderer);
};
static gIngameMenu INGAME_MENU;
