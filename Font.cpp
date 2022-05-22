#include "Font.h"
void loadFont(bool& success, TTF_Font*& Font, TTF_Font*& FontSmallSize, TTF_Font*& FontMedSize, TTF_Font*& FontBigSize) {
    Font = TTF_OpenFont("font/Planes_ValMore.ttf", 50);
    if (Font == NULL)
    {
        cout << "Failed to load Planes_ValMore font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
    FontBigSize = TTF_OpenFont("font/Planes_ValMore.ttf", 140);
    if (FontBigSize == NULL)
    {
        cout << "Failed to load Planes_ValMore font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
    FontMedSize = TTF_OpenFont("font/Planes_ValMore.ttf", 80);
    if (FontMedSize == NULL)
    {
        cout << "Failed to load Planes_ValMore font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
    FontSmallSize = TTF_OpenFont("font/Planes_ValMore.ttf", 30);
    if (FontSmallSize == NULL)
    {
        cout << "Failed to load Planes_ValMore font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
}