// BopBop.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

class LTexture {
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(string path);
    void free();

#if defined(SDL_TTF_MAJOR_VERSION)
    //Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

    //set color mudulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //set blending
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);

    void render(int x, int y, SDL_Rect* clip = NULL, int mScale=1);

    int getWidth();
    int getHeight();
private:
    SDL_Texture* mTexture;

    int mWidth;
    int mHeight;
};

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

void LTexture::render(int x, int y, SDL_Rect* clip, int mScale) {
    SDL_Rect renderQuad = { x,y,mWidth, mHeight };
    if (clip != NULL) {
        renderQuad.w = clip->w * mScale;
        renderQuad.h = clip->h * mScale;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getHeight() {
    return mHeight;
}
int LTexture::getWidth() {
    return mWidth;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
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
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
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
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }


    //Return success
    return mTexture != NULL;
}
#endif

class LTimer
{
public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};
LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if (mStarted && !mPaused)
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if (mStarted && mPaused)
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if (mStarted)
    {
        //If the timer is paused
        if (mPaused)
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}

bool init();

bool loadMedia();

void clode();


class gBackground {
public:
    LTexture Layer1;
    LTexture Layer2;
    LTexture Layer3;

    gBackground() {
    }
    void render() {
        Layer1.render(0, 0);
        Layer2.render(0, 0);
        Layer3.render(0, 0);
    }
};
gBackground bg;

const int GROUND = SCREEN_HEIGHT - 200;
const int JUMP_MAX = 270;
const double GRAVITY =3;
const int RUN = 0;
const int JUMP = 1;
const int FALL = 2;
const int FASTLANDING = 3;
class gCharacter {
private:
    double mPosX;
    double mPosY;

    double mVelY;

    LTimer timer;
    //jump height max

    int status;
public:
    LTexture run;
    LTexture jump_n_fall;
    gCharacter(){
        mPosX = SCREEN_WIDTH/7.0;
        mPosY = GROUND;
        mVelY = 0;

        status = 0;
    };
    vector<SDL_Rect> spriteClip_run;
    vector<SDL_Rect> spriteClip_jump;
    vector<SDL_Rect> spriteClip_fall;
    void show(SDL_Renderer *gRenderer) {
        static int frame = 0;
        move();
        
        SDL_Rect* currentClip = &spriteClip_run[frame / 25];
        const int RUN_FRAME_SIZE = spriteClip_run.size() - 1;
        ++frame;
        if (frame / 25 > RUN_FRAME_SIZE) {
            frame = 0;
        }
        run.render(mPosX, mPosY, currentClip);

        switch (status)
        {
        case RUN:
            cout << "run" << endl;
            break;
        case JUMP:
            cout << "jump" << endl;
            break;
        case FALL:
            cout << "fall" << endl;
            break;
        }
    }
    bool isOnGround() {
        if (mPosY  == GROUND) {
            return true;
        }
        return false;
    }
    void handleEvent(SDL_Event& e) {
        
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_SPACE) {
                if (isOnGround()) {
                    status = JUMP;
                    return;
                }
            }
            if (e.key.keysym.sym == SDLK_DOWN) {
                mVelY = 8;
                status = FASTLANDING;
            }
        }
    }
    void move()
    {
        //FAST LANDING
        if (status==FASTLANDING){
            mPosY += mVelY;

            //If the dot went too far up or down
            if (mPosY > GROUND)
            {
                //Move back
                mPosY = GROUND;
                status = 0;
            }
        }
        //JUMP
        if (status==JUMP && mPosY >= JUMP_MAX) {
            if (!timer.isStarted ()) timer.start();
            if (timer.isStarted()) { 
                mVelY = GRAVITY;
                mPosY -= mVelY;
            }
        }
        else if (mPosY <= JUMP_MAX) { 
            status = FALL;
            timer.stop();
        }

        if (status==FALL && mPosY<=GROUND) {
            if (!timer.isStarted()) timer.start();
            if (timer.isStarted()) {
                mVelY = GRAVITY;
                mPosY += mVelY;
            }
        }

        if (isOnGround()) { 
            status = RUN;
            timer.stop();
        }
    }
};

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Init error: " << SDL_GetError();
        success = false;
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
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                cout << "Khong tao duoc renderer! " << SDL_GetError();
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    cout << "SDL_image khong duoc khoi tao! " << IMG_GetError();
                    success = false;
                }

                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}
gCharacter CHARpinkmonster;
bool loadMedia() {
    bool success = true;
    //BG layer load

    gFont = TTF_OpenFont("font/lazy.ttf", 28);
    if (gFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    if (!bg.Layer1.loadFromFile("imgs/background/background_layer_1.png")) {
        cout << "Load BG Layer1 that bai! "<<endl;
        success = false;
    }
    else
    {
        bg.Layer1.setBlendMode(SDL_BLENDMODE_BLEND);
    }
    if (!bg.Layer2.loadFromFile("imgs/background/background_layer_2.png")) {
        cout << "Load BG Layer2 that bai! " << endl;
        success = false;
    }
    else
    {
        bg.Layer2.setBlendMode(SDL_BLENDMODE_BLEND);
    }
    if (!bg.Layer3.loadFromFile("imgs/background/background_layer_3.png")) {
        cout << "Load BG Layer3 that bai! " << endl;
        success = false;
    }
    else
    {
        bg.Layer3.setBlendMode(SDL_BLENDMODE_BLEND);
    }

    if (!CHARpinkmonster.run.loadFromFile("imgs/characters/Rabbit/Run.png")) {
        cout << "Load rabbit run that bai!" << endl;
        success = false;
    }
    else
    {
        CHARpinkmonster.spriteClip_run = {
        {2151,19,139,132},
        {1906,13,141,147},
        {1667,14,120,170},
        {1428,31,116,154},
        {1205,28,116,129},
        {961,17,119,125},
        {691,21,152,161},
        {463,25,124,160},
        {246,39,102,145},
        {10,36,109,148} };
    }
    
    if (!CHARpinkmonster.jump_n_fall.loadFromFile("imgs/characters/Rabbit/Jump.png")) {
        cout << "Load rabbit jump that bai!" << endl;
        success = false;
    }
    else
    {
        CHARpinkmonster.spriteClip_jump = {
            {0,0,150,350},
            {214,0,150,350},
            {454,0,150,350},
            {681,0,150,350},
            {898,0,150,350},
            {1121,0,150,350},
            {1336,0,150,350},
            {1565,0,150,350},
            {1794,0,150,350},
            {2026,0,150,350},
            //
        };
        CHARpinkmonster.spriteClip_fall = {
            {2260,0,150,350},
            {2270,0,150,350},
            {2710,0,150,350},
            {2939,0,150,350},
            {3167,0,150,350},
            {3390,0,150,350},
            {3624,0,150,350},
            {3839,0,150,350},
            {4054,0,150,350},
        };
    }
    return success;
}

void close() {
    bg.Layer1.free();
    bg.Layer2.free();
    bg.Layer3.free();

    //Free global font
    TTF_CloseFont(gFont);
    gFont = NULL;

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
                    CHARpinkmonster.handleEvent(e);
                }
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);

                //Background render
                bg.render();

                CHARpinkmonster.show(gRenderer);

                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();
    return 0;
}
