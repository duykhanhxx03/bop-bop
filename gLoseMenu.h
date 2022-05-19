#pragma once
#include "LTexture.h"
#include "CommonFunction.h"
#include "LButton.h"
#include "LScore.h"
#include <sstream>

class gLoseMenu {
private:
    SDL_Color textColor = { 255, 255, 255, 255 };
    LTexture loseMenuBox;
    LTexture reStartTexture;
    LTexture fisrtStar[ONOFF_STAR_TOTAL];
    LTexture secondStar[ONOFF_STAR_TOTAL];
    LTexture thirdStar[ONOFF_STAR_TOTAL];

    LTexture tempTexture;
    LTexture yourScoreTexture;
    stringstream timeText;
    LButton reStart;
    SDL_Rect reStartSpriteClips[BUTTON_SPRITE_TOTAL];

public:
    gLoseMenu() {};
    void handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre);
    void show(SDL_Renderer* gRenderer, TTF_Font* Font, TTF_Font* FontMedSize, LScore& SCORE);
    void loadMedia(bool& success, SDL_Renderer* gRenderer);
};
static gLoseMenu LOSE_MENU;