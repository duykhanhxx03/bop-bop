#pragma once
#include "LTexture.h"
#include "LButton.h"
class gStartMenu {
private:
    //Start menu texture
    LTexture StartMenuBox;
    LTexture ButtonTexture;
    LTexture IntroduceButtonTexture;

    SDL_Color textColor = { 255,255,255,255 };
    LTexture GameTitleText;

    //Button
    LButton Start;
    LButton Options;
    LButton Exit;
    LButton Introduce;

    //Clips
    SDL_Rect StartSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect OptionsSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect ExitSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect IntroduceSpriteClips[BUTTON_SPRITE_TOTAL];
public:
    gStartMenu() {};
    void handleEvent(SDL_Event& e,MENU_STATUS& menuStatus, MENU_STATUS& menuPre);
    void show(SDL_Renderer* gRenderer, TTF_Font* Font);
    void loadMedia(bool& success, SDL_Renderer* gRenderer, TTF_Font* Font);
};
static gStartMenu START_MENU;