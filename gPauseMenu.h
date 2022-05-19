#pragma once
#include "LTexture.h"
#include "LButton.h"
class gPauseMenu {
private:
    //Start menu texture
    LTexture PauseMenuBox;
    LTexture ButtonTexture;

    //Button
    LButton ReStart;
    LButton Resume;
    LButton Options;
    LButton Exit;

    //Clips
    SDL_Rect ReStartSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect ResumeSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect OptionsSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect ExitSpriteClips[BUTTON_SPRITE_TOTAL];
public:
    gPauseMenu() {};
    void handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre);
    void show(SDL_Renderer* gRenderer);
    void loadMedia(bool& success, SDL_Renderer* gRenderer);
};
static gPauseMenu PAUSE_MENU;

