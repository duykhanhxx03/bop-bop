#pragma once
#include "LTexture.h"
#include "LButton.h"
class gExitMenu {
private:
    //Start menu texture
    LTexture ExitMenuBox;
    LTexture ButtonTexture;

    //Button
    LButton Yes;
    LButton No;

    //Clips
    SDL_Rect YesSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect NoSpriteClips[BUTTON_SPRITE_TOTAL];
public:
    gExitMenu() {};
    void handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre);
    void show(SDL_Renderer* gRenderer);
    void loadMedia(bool& success, SDL_Renderer* gRenderer);
};
static gExitMenu EXIT_MENU;

