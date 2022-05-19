#pragma once
#include "LTexture.h"
#include "CommonFunction.h"
#include "LButton.h"

class gIntroduceMenu {
private:
    //Start menu texture
    LTexture IntroduceMenuBox;
    LTexture BackTexture;

    //Button
    LButton Back;

    //Clips
    SDL_Rect BackSpriteClips[BUTTON_SPRITE_TOTAL];
public:
    gIntroduceMenu() {};
    void handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre);
    void show(SDL_Renderer* gRenderer);
    void loadMedia(bool& success, SDL_Renderer* gRenderer);
};
static gIntroduceMenu INTRODUCE_MENU;