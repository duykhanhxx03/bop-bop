#pragma once
#include "LTexture.h"
#include "CommonFunction.h"
#include "LButton.h"
#include "LSound.h"
#include <fstream>
#include <iostream>
using namespace std;

class gOptionsMenu {
private:
    //Start menu texture
    LTexture OptionsMenuBox;
    LTexture BGMTexture;
    LTexture SFXTexture;
    LTexture BackTexture;

    //Button
    LButton Back;
    LButton SFX;
    LButton BGM;

    //status
    ControllerStatus SFXstatus;
    ControllerStatus BGMstatus;

    //Clips
    SDL_Rect BackSpriteClips[BUTTON_SPRITE_TOTAL];
public:
    gOptionsMenu() {};
    void handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre);
    void show(SDL_Renderer* gRenderer);
    void setPositionX_BGM(const int& x);
    void setPositionX_SFX(const int& x);
    void loadMedia(bool& success, SDL_Renderer* gRenderer, LSound& gSound);
};
static gOptionsMenu OPTIONS_MENU;
