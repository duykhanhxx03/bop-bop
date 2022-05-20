#pragma once
#include "CommonFunction.h"
class LCountDown {
private:
    int count;
    bool isEnd;
    stringstream timeText;
    LTexture textCountDown;
    LTimer timer;
    SDL_Color textColor = { 255, 255, 255, 255 };
public:
    void init();
    LCountDown();
    void render(SDL_Renderer* gRenderer);
    void show(SDL_Renderer* gRenderer, TTF_Font* Font);
    bool isEndCountDown();
};
static LCountDown gCountDown;
