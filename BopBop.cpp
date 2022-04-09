// BopBop.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

class LTexture {
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(string path);
    void free();
    //set color mudulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //set blending
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);

    void render(int x, int y, SDL_Rect* clip = NULL);

    int getWidth();
    int getHeight();
private:
    SDL_Texture* mTexture;

    int mWidth;
    int mHeight;
};

bool init();

bool loadMedia();

void clode();

SDL_Window* gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

LTexture gBackgroundTexture;

LTexture::LTexture() {
    mTexture = NULL;
    mHeight = 0;
    mWidth = 0;
}
LTexture::~LTexture()
{
    free();
}
bool LTexture::loadFromFile(string path) {
    free();

    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        cout << "Khong load duoc anh: " << path << " " << IMG_GetError();
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            cout << "Khong tao duoc texture tu surface: " << path << " " << SDL_GetError();
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
void LTexture::free(){
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

void LTexture::render(int x, int y, SDL_Rect* clip) {
    SDL_Rect renderQuad = { x,y,mWidth, mHeight };
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getHeight() {
    return mHeight;
}
int LTexture::getWidth() {
    return mWidth;
}

bool init() {
    bool succes = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Init error: " << SDL_GetError();
        succes = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            cout<<"Warning: Linear texture filtering not enabled!";
        }

        gWindow = SDL_CreateWindow("BOP!BOP! by duykhanhxx03", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            cout << "Khong tao duoc window! " << SDL_GetError();
            succes = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                cout << "Khong tao duoc renderer! " << SDL_GetError();
                succes = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    cout << "SDL_image khong duoc khoi tao! " << IMG_GetError();
                    succes = false;
                }
            }
        }
    }
    return succes;
}

bool loadMedia() {
    bool succes = true;

    if (!gBackgroundTexture.loadFromFile("background_layer_1.png")) {
        cout << "Load BG Layer1 that bai! "<<endl;
        succes = false;
    }
    else
    {
        gBackgroundTexture.setBlendMode(SDL_BLENDMODE_BLEND);
    }
    return succes;
}

void close() {
    gBackgroundTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}
int main(int argc, char* argv[])
{
    if (!init()) {
        cout << "Failed to initialize! " << endl;
    }
    else
    {
        if (!loadMedia()) {
            cout << "Failed to load media! " << endl;
        }
        else
        {
            bool quit = false;

            SDL_Event e;
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);

                gBackgroundTexture.render(0, 0);
                
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();
    return 0;
}
