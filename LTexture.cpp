#include "LTexture.h"
//Texture
LTexture::LTexture() {
    mTexture = NULL;
    mHeight = 0;
    mWidth = 0;
}
LTexture::~LTexture()
{
    free();
}
bool LTexture::loadFromFile(string path, SDL_Renderer* gRenderer) {
    free();

    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        cout << "Failed to load image: " << path << " " << IMG_GetError();
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            cout << "Failed to create texture from surface: " << path << " " << SDL_GetError();
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture != NULL;
}
void LTexture::free() {
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}
void LTexture::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}
void LTexture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(mTexture, alpha);
}
void LTexture::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip) {
    SDL_Rect renderQuad = { x,y,mWidth, mHeight };
    if (clip != NULL) {
        renderQuad.w = clip->w ;
        renderQuad.h = clip->h ;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}
int LTexture::getHeight() const {
    return mHeight;
}
int LTexture::getWidth() const {
    return mWidth;
}
#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(SDL_Renderer* gRenderer, std::string textureText, SDL_Color textColor, TTF_Font* gFont)
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface != NULL)
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL)
        {
            cout << "Unable to create texture from rendered text! SDL Error: %s\n" << SDL_GetError();
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    else
    {
        cout << "Unable to render text surface! SDL_ttf Error: %s\n" << TTF_GetError();
    }


    //Return success
    return mTexture != NULL;
}
#endif