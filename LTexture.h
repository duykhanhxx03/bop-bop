#pragma once
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
using namespace std;

class LTexture {
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(string path, SDL_Renderer *gRenderer);
    void free();

#if defined(SDL_TTF_MAJOR_VERSION)
    //Creates image from font string
    bool loadFromRenderedText(SDL_Renderer* gRenderer, string textureText, SDL_Color textColor, TTF_Font* gFont);
#endif

    //set color mudulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //set blending
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);

    void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL);

    int getWidth() const;
    int getHeight() const;
private:
    SDL_Texture* mTexture;

    int mWidth;
    int mHeight;
};