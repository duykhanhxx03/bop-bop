#pragma once
#include <SDL_ttf.h>
#include <iostream>
using namespace std;
//Font Declare
static TTF_Font* Font = NULL;
static TTF_Font* FontBigSize = NULL;
static TTF_Font* FontMedSize = NULL;
void loadFont(bool& success, TTF_Font*& Font, TTF_Font*& FontMedSize, TTF_Font*& FontBigSize);

