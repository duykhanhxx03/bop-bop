#pragma once
#include "LTexture.h"
#include"CommonFunction.h"
class gBackground {
private:
    LTexture Layer1;
    LTexture Layer2;
    LTexture Layer3;
    LTexture Layer4;
    LTexture Ground;
public:
    gBackground() {}
    void render(SDL_Renderer* gRenderer, int speedRender);
    void loadMedia(bool& success, SDL_Renderer* gRenderer);
};
static gBackground BACKGROUND;
