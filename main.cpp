// BopBop.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <math.h>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
int speedRender = 10;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

//Sound
class LSound {
private:
    Mix_Music* gBgm;
    Mix_Chunk* gJumpSound;
    //MIX_MAX_VOLUME=128
    int volumeMusic = MIX_MAX_VOLUME;
    int volumeChunk = MIX_MAX_VOLUME;
public:
    bool setBgm(const string &path) {
        gBgm = Mix_LoadMUS(path.c_str());
        return gBgm != NULL;
    }
    bool setJumpSound(const string& path) {
        gJumpSound = Mix_LoadWAV(path.c_str());
        return gJumpSound != NULL;
    }
    bool isPlayingMusic() {
        return Mix_PlayingMusic();
    }
    bool isPausedMusic() {
        return Mix_PausedMusic();
    }
    void StopMusic() {
        Mix_HaltMusic();
    }
    void PlayMusic() {
        if (!isPlayingMusic())
            Mix_PlayMusic(gBgm, -1);
        Mix_VolumeMusic(volumeMusic);
    }
    void PauseMusic() {
        if (isPlayingMusic())
            Mix_PauseMusic();
    }
    void ResumeMusic() {
        if (isPausedMusic())
            Mix_ResumeMusic();
    }
    void PlayJumpSound() {
        Mix_PlayChannel(-1, gJumpSound, 0);
        Mix_VolumeChunk(gJumpSound, volumeChunk);
    }
    void setVolumeMusic(const int& v) {
        volumeMusic = v;
        Mix_VolumeMusic(volumeMusic);
    }
    void setVolumeChunk(const int& v) {
        volumeChunk = v;
        Mix_VolumeChunk(gJumpSound, volumeChunk);
    }

};
LSound gSound;

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

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

const int BUTTON_WIDTH = 320;
const int BUTTON_HEIGHT = 113;
const int TOTAL_BUTTONS = 3;
//Button

class LButton
{
public:
    //Initializes internal variables
    LButton();

    //Sets top left position
    void setPosition(int x, int y);

    //Handles mouse event
    void handleEvent(SDL_Event* e);

    //Shows button sprite
    void render(LTexture& gButtonSpriteSheetTexture, SDL_Rect SpriteClips[]);

private:
    //Top left position
    SDL_Point mPosition;

    //Currently used global sprite
    LButtonSprite mCurrentSprite;
};

class gStartMenu {
private:
public:
    gStartMenu() {};
    //Start menu texture
    LTexture StartMenuBox;
    LTexture MenuBackground;
    LTexture ButtonTexture;

    //Button
    LButton Start;
    LButton Options;
    LButton Exit;

    //Clips
    SDL_Rect StartSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect OptionsSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect ExitSpriteClips[BUTTON_SPRITE_TOTAL];
    void handleEvent(SDL_Event &e) {
        Start.handleEvent(&e);
        Options.handleEvent(&e);
        Exit.handleEvent(&e);
    }
    void show() {
        static int alpha = 0;
        //Blur
        if (alpha > 255) alpha = 255;
        MenuBackground.setAlpha(alpha);
        alpha += 5;

        //Render menu background
        MenuBackground.render(0, 0);
        
        //Render start menu box
        StartMenuBox.render((SCREEN_WIDTH - StartMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - StartMenuBox.getHeight()) / 2);

        //Render button
        Start.render(ButtonTexture, StartSpriteClips);
        Options.render(ButtonTexture, OptionsSpriteClips);
        Exit.render(ButtonTexture, ExitSpriteClips);
    }

};
gStartMenu START_MENU;
class gPauseMenu {
private:
public:
    gPauseMenu() {};
    //Start menu texture
    LTexture PauseMenuBox;
    LTexture MenuBackground;
    LTexture ButtonTexture;

    //Button
    LButton ReStart;
    LButton Resume;
    LButton Options;
    LButton Exit;

    //Clips
    SDL_Rect ReStartSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect ResumeSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect OptionsSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect ExitSpriteClips[BUTTON_SPRITE_TOTAL];
    void handleEvent(SDL_Event& e) {
        ReStart.handleEvent(&e);
        Resume.handleEvent(&e);
        Options.handleEvent(&e);
        Exit.handleEvent(&e);
    }
    void show() {
        static int alpha = 0;
        //Blur
        if (alpha > 255) alpha = 255;
        MenuBackground.setAlpha(alpha);
        alpha += 5;

        //Render menu background
        MenuBackground.render(0, 0);

        //Render start menu box
        PauseMenuBox.render((SCREEN_WIDTH - PauseMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - PauseMenuBox.getHeight()) / 2);

        //Render button
        ReStart.render(ButtonTexture, ReStartSpriteClips);
        Resume.render(ButtonTexture, ResumeSpriteClips);
        Options.render(ButtonTexture, OptionsSpriteClips);
        Exit.render(ButtonTexture, ExitSpriteClips);
    }

};
gPauseMenu PAUSE_MENU;

//class gPauseMenu {
//private:
//public:
//    gPauseMenu() {};
//    //Start menu texture
//    LTexture PauseMenuBox;
//    LTexture MenuBackground;
//    LTexture ButtonTexture;
//
//    //Button
//    LButton ReStart;
//    LButton Resume;
//    LButton Options;
//    LButton Exit;
//
//    //Clips
//    SDL_Rect ReStartSpriteClips[BUTTON_SPRITE_TOTAL];
//    SDL_Rect ResumeSpriteClips[BUTTON_SPRITE_TOTAL];
//    SDL_Rect OptionsSpriteClips[BUTTON_SPRITE_TOTAL];
//    SDL_Rect ExitSpriteClips[BUTTON_SPRITE_TOTAL];
//    void handleEvent(SDL_Event& e) {
//        ReStart.handleEvent(&e);
//        Resume.handleEvent(&e);
//        Options.handleEvent(&e);
//        Exit.handleEvent(&e);
//    }
//    void show() {
//        static int alpha = 0;
//        //Blur
//        if (alpha > 255) alpha = 255;
//        MenuBackground.setAlpha(alpha);
//        alpha += 5;
//
//        //Render menu background
//        MenuBackground.render(0, 0);
//
//        //Render start menu box
//        PauseMenuBox.render((SCREEN_WIDTH - PauseMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - PauseMenuBox.getHeight()) / 2);
//
//        //Render button
//        ReStart.render(ButtonTexture, ReStartSpriteClips);
//        Resume.render(ButtonTexture, ResumeSpriteClips);
//        Options.render(ButtonTexture, OptionsSpriteClips);
//        Exit.render(ButtonTexture, ExitSpriteClips);
//    }
//
//};

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
    //If mouse event happened
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if (x < mPosition.x)
        {
            inside = false;
        }
        //Mouse is right of the button
        else if (x > mPosition.x + BUTTON_WIDTH)
        {
            inside = false;
        }
        //Mouse above the button
        else if (y < mPosition.y)
        {
            inside = false;
        }
        //Mouse below the button
        else if (y > mPosition.y + BUTTON_HEIGHT)
        {
            inside = false;
        }

        //Mouse is outside button
        if (!inside)
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch (e->type)
            {
            case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

            case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

            case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

void LButton::render(LTexture & gButtonSpriteSheetTexture, SDL_Rect SpriteClips[])
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &SpriteClips[mCurrentSprite]);
}

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

bool checkCollision(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Go through the A boxes
    for (int Abox = 0; Abox < a.size(); Abox++)
    {
        //Calculate the sides of rect A
        leftA = a[Abox].x;
        rightA = a[Abox].x + a[Abox].w;
        topA = a[Abox].y;
        bottomA = a[Abox].y + a[Abox].h;

        //Go through the B boxes
        for (int Bbox = 0; Bbox < b.size(); Bbox++)
        {
            //Calculate the sides of rect B
            leftB = b[Bbox].x;
            rightB = b[Bbox].x + b[Bbox].w;
            topB = b[Bbox].y;
            bottomB = b[Bbox].y + b[Bbox].h;

            //If no sides from A are outside of B
            if (((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB)) == false)
            {
                //A collision is detected
                return true;
            }
        }
    }

    //If neither set of collision boxes touched
    return false;
}
//Random session
#include <ctime>
#include <cstdlib>
int generateRandomNumber(const int min, const int max)
{
    srand(time(0));
    // TODO: Return a random integer number between min and max
    return rand() % (max - min + 1) + min;
}
class gBackground {
public:
    LTexture Layer1;
    LTexture Layer2;
    LTexture Layer3;
    LTexture Layer4;
    LTexture Ground;

    gBackground() {
    }
    void render(int speedRender) {
        //Render layer 1
        static int scrollingOffset_layer1 = 0;
        if (speedRender==0) scrollingOffset_layer1-=0; else
            scrollingOffset_layer1-=1;
        if (scrollingOffset_layer1 < -Layer1.getWidth())
        {
            scrollingOffset_layer1 = 0;
        }
        Layer1.render(scrollingOffset_layer1, 0);
        Layer1.render(scrollingOffset_layer1 + Layer1.getWidth(), 0);

        //Render layer 2
        static int scrollingOffset_layer2 = 0;
        if (speedRender == 0) scrollingOffset_layer2 -= 0; else
            scrollingOffset_layer2 -= 2;
        if (scrollingOffset_layer2 < -Layer2.getWidth())
        {
            scrollingOffset_layer2 = 0;
        }
        Layer2.render(scrollingOffset_layer2, 0);
        Layer2.render(scrollingOffset_layer2 + Layer2.getWidth(), 0);

        //Render layer 3
        static int scrollingOffset_layer3 = 0;
        if (speedRender == 0) scrollingOffset_layer3 -= 0; else
            scrollingOffset_layer3 -= 4;
        if (scrollingOffset_layer3 < -Layer3.getWidth())
        {
            scrollingOffset_layer3 = 0;
        }
        Layer3.render(scrollingOffset_layer3, 0);
        Layer3.render(scrollingOffset_layer3 + Layer3.getWidth(), 0);

        //Render layer 4
        static int scrollingOffset_layer4 = 0;
        scrollingOffset_layer4 -= speedRender;
        if (scrollingOffset_layer4 < -Layer4.getWidth())
        {
            scrollingOffset_layer4 = 0;
        }
        Layer4.render(scrollingOffset_layer4, SCREEN_HEIGHT - 205);
        Layer4.render(scrollingOffset_layer4 + Layer4.getWidth(), SCREEN_HEIGHT - 205);


        //Render Ground
        static double scrollingOffset_ground = 0;
        scrollingOffset_ground -= speedRender;
        if (scrollingOffset_ground < -Ground.getWidth())
        {
            scrollingOffset_ground = 0;
        }
        Ground.render(scrollingOffset_ground, SCREEN_HEIGHT - 110);
        Ground.render(scrollingOffset_ground + Ground.getWidth(), SCREEN_HEIGHT-110);

    }
};
gBackground bg;

static int count_run = 0;
static int count_jump = 0;
static int count_fall = 0;
static int count_fastlanding = 0;

const int GROUND = SCREEN_HEIGHT - 70;
const int JUMP_MAX = 290;
const double GRAVITY =120;

//Obstacles
LTexture mShroom;
LTexture mPlantRed;
LTexture mPlantViolet; 
class Shroom {
private:
    vector <SDL_Rect> mColliders;
    vector <SDL_Rect> mColliders_offset;
public:

    LTexture* character;
    int mPosX;
    int mPosY;
    Shroom() {
        mPosX = 0;
        character = NULL;
        mPosY = 0;
    }
    Shroom(LTexture &texture, const int &n, const vector<SDL_Rect>& Colliders={}) {
        character = &texture;
        mPosX = n;
        mPosY = SCREEN_HEIGHT - 80 - character->getHeight() + 5;
        //set colliders
        mColliders = mColliders_offset = Colliders;
        shiftColliders(mColliders);
    }
    vector<SDL_Rect> getColliders() {
        return mColliders;
    }
    void setColliders(const vector<SDL_Rect>& Colliders) {
        mColliders = mColliders_offset = Colliders;
        shiftColliders(mColliders);
    }
    void setX(const int &n) {
        mPosX = n;
    }
    int getWidth() const{
        return character->getWidth();
    }
    int getHeight() const{
        return character->getHeight();
    }
    int getX() const{
        return mPosX;
    }
    void setCharacter(LTexture &texture){
        character = &texture;
    }
    void Free(){
        character = NULL;
    }
    void move() {
        mPosX -= speedRender;
        shiftColliders(mColliders);
    }
    void render(int speedRender) {
        character->render(mPosX, mPosY);
        move();
    }
    bool isOver(){
        if (mPosX <= -character->getWidth()) {
            //Free();
            //x = SCREEN_WIDTH;
            return true;
        }
        return false;
    }
    void shiftColliders(vector <SDL_Rect> &Colliders)
    {
        //Go through the dot's collision boxes
        for (int set = 0; set < Colliders.size(); ++set)
        {
            //Center the collision box
            Colliders[set].x = mPosX + mColliders_offset[set].x;
            //cout <<"Shroom: "<< mColliders[set].x << endl;
            //Set the collision box at its row offset
            Colliders[set].y = mPosY + mColliders_offset[set].y;

            //Move the row offset down the height of the collision box
        }
    }
};

class LIST_OBSTACLES {
private:
    LTexture* character;
    vector<SDL_Rect> mColliders;
public:
    vector<SDL_Rect> getColliders() {
        return mColliders;
    }
    void setColliders(const vector<SDL_Rect>& Colliders) {
        mColliders = Colliders;
    }
    void setCharacter(LTexture& texture) {
        character = &texture;
    }
    LTexture* getCharacter() {
        return character;
    }
};

vector <LIST_OBSTACLES> listObstacles;
enum INDEX_LIST_OBSTACLES {
    SHROOM = 0,
    PLANTRED = 1,
    PLANTVIOLET=2
};
//timer
LTimer timeJump;
enum Rabbit_Sheet_Height {
    RUN_SHEET_HEIGHT=160,
    JUMP_SHEET_HEIGHT= 160,
    FALL_SHEET_HEIGHT= 160,
    FAST_LANDING_HEIGHT= 160
};
enum STATUS {
    RUN = 0,
    JUMP = 1,
    FALL = 2,
    FASTLANDING = 3
};
const int numberOfrunClips = 3;
const int numberOfjumpClips = 1;
const int numberOffallClips = 1;
const int numberOffastlandingClips = 1;
class gCharacter {
private:

    double mPosX;
    double mPosY;

    int runPresentFrame;
    int jumpPresentFrame;
    int fallPresentFrame;
    int fastlandingPresentFrame;

    double mVelY;
    bool isFall;

    int status;
    //SDL_Rect mCollinder;

    vector<SDL_Rect> mCollidersRun[numberOfrunClips];
    vector<SDL_Rect> mCollidersRun_offset[numberOfrunClips];

    vector<SDL_Rect> mCollidersJump[numberOfjumpClips];
    vector<SDL_Rect> mCollidersJump_offset[numberOfjumpClips];

    vector<SDL_Rect> mCollidersFall[numberOffallClips];
    vector<SDL_Rect> mCollidersFall_offset[numberOffallClips];

    vector<SDL_Rect> mCollidersFastLanding[numberOffastlandingClips];
    vector<SDL_Rect> mCollidersFastLanding_offset[numberOffastlandingClips];
    
    vector <SDL_Rect> mColliders;
    vector <SDL_Rect> mColliders_offset;
public:
    LTexture run;
    LTexture jump;
    LTexture fall;

    gCharacter(){
        runPresentFrame=0;
        jumpPresentFrame=0;
        fallPresentFrame=0;
        fastlandingPresentFrame=0;

        mPosX = 150;
        mPosY = GROUND-RUN_SHEET_HEIGHT;
        mVelY = 0;
        
        status = 0;
        isFall = false;
        mColliders = {
            {112,96,32,4},
            {100,100,50,4},
            {92,104,62,6},
            {84,110,74,4},
            {80,114,80,4},
            {138,118,20,4},
            {76,118,56,4},
            {74,122,58,8},
            {72,130,56,4},
            {66,134,50,4},
            {62,138,52,4},
            {66,142,44,10},
            {88,152,26,8}
        };
        //test

        mCollidersRun[0] = mCollidersRun_offset[0]={
            {60,112,123,24},
            {127,135,23,9},
            {118,144,22,6},
            {112,150,16,4},
            {60,134,47,26}
        };
        mCollidersRun[1] = mCollidersRun_offset[1]={
            {81,152,21,8},
            {66,148,104,4},
            {66,142,104,8 },
            {66,138,104,4},
            {66,134,98,4},
            {68,130,90,4},
            {70,126,82,4},
            {72,122,74,4},
            {76,118,68,4},
            {78,114,60,4},
            {82,110,50,4},
            {88,106,38,4},
            {94,102,18,4}
        };
        mCollidersRun[2] = mCollidersRun_offset[2]={
            {72,148,70,12},
            {148,150,30,4},
            {72,140,110,10},
            {72,136,106,4},
            {72,132,100,4},
            {72,128,90,4},
            {72,124,84,4},
            {72,120,76,4},
            {72,116,66,4},
            {72,112,56,4},
            {72,108,48,4},
            {72,104,38,4},
            {72,100,30,4}
        };

        mCollidersJump[0] = mCollidersFall[0] = mCollidersFastLanding[0] 
                          = mCollidersJump_offset[0] = mCollidersFall_offset[0] = mCollidersFastLanding_offset[0] = {
            {112,96,32,4},
            {100,100,50,4},
            {92,104,62,6},
            {84,110,74,4},
            {80,114,80,4},
            {138,118,20,4},
            {76,118,56,4},
            {74,122,58,8},
            {72,130,56,4},
            {66,134,50,4},
            {62,138,52,4},
            {66,142,44,10},
            {88,152,26,8}
        };
        shiftColliders();
    };
    vector<SDL_Rect> spriteClip_run;
    vector<SDL_Rect> spriteClip_jump;
    vector<SDL_Rect> spriteClip_fall;

    //use only for character
    void shiftColliders() {
        shiftColliders(mCollidersFastLanding, mCollidersFastLanding_offset, numberOffastlandingClips);
        shiftColliders(mCollidersJump, mCollidersJump_offset, numberOfjumpClips);
        shiftColliders(mCollidersFall, mCollidersFall_offset, numberOffallClips);
        shiftColliders(mCollidersRun, mCollidersRun_offset, numberOfrunClips);
    }
    void shiftColliders(vector <SDL_Rect> Colliders[], vector <SDL_Rect> Colliders_offset[], const int &numberOfClips)
    {
        //Go through the dot's collision boxes
        for (int index=0; index< numberOfClips; index++)
        for (int set = 0; set < Colliders[index].size(); ++set)
        {
            //Center the collision box
            Colliders[index][set].x = mPosX + Colliders_offset[index][set].x;
            //Set the collision box at its row offset
            Colliders[index][set].y = mPosY + Colliders_offset[index][set].y;

        }
    }
    void shiftColliders(vector <SDL_Rect> &Colliders)
    {
        //Go through the dot's collision boxes
            for (int set = 0; set < Colliders.size(); ++set)
            {
                //Center the collision box
                Colliders[set].x = mPosX + mColliders_offset[set].x;
                //Set the collision box at its row offset
                Colliders[set].y = mPosY + mColliders_offset[set].y;

            }
    }
    int getStatus() {
        return status;
    }
    void show(SDL_Renderer *gRenderer) {

        if (status == RUN) {
            runPresentFrame = count_run++/ 7;
            SDL_Rect* currentClip = &spriteClip_run[runPresentFrame];
            const int RUN_FRAME_SIZE = spriteClip_run.size() - 1;
            if (count_run/7 > RUN_FRAME_SIZE) {
                count_run = 0;
            }
            run.render(mPosX, mPosY, currentClip);
        }

        if (status == JUMP) {
            jumpPresentFrame = count_jump++/ 20;
            SDL_Rect* currentClip = &spriteClip_jump[jumpPresentFrame];
            const int JUMP_FRAME_SIZE = spriteClip_jump.size() - 1;
            if (count_jump/20> JUMP_FRAME_SIZE) {
                count_jump = 0;
            }
            jump.render(mPosX, mPosY, currentClip);
        }

        if (status == FALL) {
            fallPresentFrame = count_fall++/ 7;
            SDL_Rect* currentClip = &spriteClip_fall[fallPresentFrame];
            const int FALL_FRAME_SIZE = spriteClip_fall.size() - 1;
            if (count_fall/ 7 > FALL_FRAME_SIZE) {
                count_fall = 0;
            }
            fall.render(mPosX, mPosY, currentClip);
        }

        if (status == FASTLANDING) {
            fastlandingPresentFrame = count_fastlanding++/ 8;
            SDL_Rect* currentClip = &spriteClip_fall[fastlandingPresentFrame];
            const int FASTLANDING_FRAME_SIZE = spriteClip_fall.size() - 1;
            if (count_fastlanding/ 8 > FASTLANDING_FRAME_SIZE) {
                count_fastlanding = 0;
            }
            fall.render(mPosX, mPosY, currentClip);
        }
        move();
    }
    void handleEvent(SDL_Event& e) {
        
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_SPACE) {
                if (status == RUN) {
                    status = JUMP;
                    mPosY = GROUND-JUMP_SHEET_HEIGHT;
                    count_jump = 0;
                    timeJump.start();
                    gSound.PlayJumpSound();
                }
            }
            if (e.key.keysym.sym == SDLK_DOWN) {
                if (status != RUN) {
                    mVelY = 18;
                    status = FASTLANDING;
                    count_fastlanding = 0;
               }
            }
        }
    }
    void move()
    {   
        //Show info
        //static int pre_status = -1;
        //if (pre_status != status) {

        //     cout <<"status: " << status << endl;
        //     cout <<"Current character height: "<< getCurrentCharHeight() << endl;
        //     cout << "is on ground? " <<isOnGround() << endl;
        //     cout << "current pos Y " << mPosY << endl;
        //     //if (isOnGround()) status = 0;
        //     cout << "--end--" << endl;
        //     pre_status = status;
        //}

        //Processing

        shiftColliders();
        //Fast landing
        if (status == FASTLANDING) {
            mPosY += 30;
            shiftColliders();
            if (mPosY > GROUND - FALL_SHEET_HEIGHT)
            {
                //Move back
                mPosY = GROUND - RUN_SHEET_HEIGHT;
                shiftColliders();
                status = RUN;
                isFall = false;
            }
        }

        if (status == JUMP && mPosY >= JUMP_MAX) {
            mVelY = 220;
            double deltaTime = timeJump.getTicks() / 1000.f;
            mPosY += - (mVelY * (deltaTime)-0.5 * 40 * (deltaTime * deltaTime));
            shiftColliders();
        }
        else if (mPosY <= JUMP_MAX && isFall == false) {
            count_fall = 0;
            status = FALL;
            //timeJump.stop();
            timeJump.start();
            isFall = true;
            //cout << "fall_1" << endl;
        }
        if (status == FALL) {
            if (mPosY  <= GROUND-FALL_SHEET_HEIGHT) {
                mVelY = 80;
                double deltaTime = timeJump.getTicks() / 1000.f;
                mPosY +=mVelY * (deltaTime)+ 0.5*40 * (deltaTime * deltaTime);
                shiftColliders();
                //cout << "fall_2" << endl;
            } else { 
                mPosY = GROUND - RUN_SHEET_HEIGHT;
                shiftColliders();
                status = RUN;
                timeJump.stop();
                isFall = false;
            }
        }
    }
    vector <SDL_Rect> &getColliders()
    {
        switch (status)
        {
        case JUMP:
            return mCollidersJump[jumpPresentFrame];
            break;
        case FALL:
            return mCollidersFall[fallPresentFrame];
            break;
        case FASTLANDING:
            return mCollidersFastLanding[fastlandingPresentFrame];
            break;
        default:
            return mCollidersRun[runPresentFrame];
            break;
        }
    }
};

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO) < 0) {
        cout << "Init error: " << SDL_GetError();
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            cout<<"Warning: Linear texture filtering not enabled!";
        }

        gWindow = SDL_CreateWindow("Bop!Bop! by duykhanhxx03", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            cout << "Khong tao duoc window! " << SDL_GetError();
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
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
                    cout<<"SDL_ttf could not initialize! SDL_ttf Error: "<< TTF_GetError();
                    success = false;
                }
                //Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
                {
                    cout<<"SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError();
                    success = false;
                }
            }
        }
    }
    return success;
}
gCharacter Rabbit;
bool loadMedia() {
    bool success = true;
    
    //Font
    gFont = TTF_OpenFont("font/lazy.ttf", 28);
    if (gFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    //Sound
    if (!gSound.setBgm("sound/bgm.wav")) {
        cout << "Failed to load bgm" << Mix_GetError() << endl;
        success = false;
    }
    if (!gSound.setJumpSound("sound/jumpsound.wav")) {
        cout << "Failed to load jumpsound" << Mix_GetError() << endl;
        success = false;
    }

    //START MENU
    if (!START_MENU.MenuBackground.loadFromFile("imgs/menu/menu_bg.jpg")) {
        cout << "Load menu_bg that bai! " << endl;
        success = false;
    }
    else
    {
        START_MENU.MenuBackground.setBlendMode(SDL_BLENDMODE_BLEND);
    }

    if (!START_MENU.ButtonTexture.loadFromFile("imgs/menu/start/start_menu_button_sheet.png")) {
        cout << "Load gStartMenu that bai! " << endl;
        success = false;
    }
    else
    {
        START_MENU.StartSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,320,113 };
        START_MENU.StartSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,0,320,113 };
        START_MENU.StartSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,0,320,113 };
        START_MENU.StartSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,0,320,113 };
        START_MENU.Start.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 210);

        START_MENU.OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,122,320,113 };
        START_MENU.OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,122,320,113 };
        START_MENU.OptionsSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,122,320,113 };
        START_MENU.OptionsSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,122,320,113 };
        START_MENU.Options.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 330);

        START_MENU.ExitSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,244,320,113 };
        START_MENU.ExitSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,244,320,113 };
        START_MENU.ExitSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,244,320,113 };
        START_MENU.ExitSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,244,320,113 };
        START_MENU.Exit.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 450);

    }
    if (!START_MENU.StartMenuBox.loadFromFile("imgs/menu/start/startmenubox.png")) {
        cout << "Load gStartMenu background that bai! " << endl;
        success = false;
    }
    //PAUSE MENU
    if (!PAUSE_MENU.MenuBackground.loadFromFile("imgs/menu/menu_bg.jpg")) {
        cout << "Load menu_bg that bai! " << endl;
        success = false;
    }
    else
    {
        PAUSE_MENU.MenuBackground.setBlendMode(SDL_BLENDMODE_BLEND);
    }
    if (!PAUSE_MENU.ButtonTexture.loadFromFile("imgs/menu/pause/pause_menu_button_sheet.png")) {
        cout << "Load gPauseMenu that bai! " << endl;
        success = false;
    }
    else
    {
        PAUSE_MENU.ResumeSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,320,113 };
        PAUSE_MENU.ResumeSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,0,320,113 };
        PAUSE_MENU.ResumeSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,0,320,113 };
        PAUSE_MENU.ResumeSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,0,320,113 };
        PAUSE_MENU.Resume.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 160);

        PAUSE_MENU.ReStartSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,122,320,113 };
        PAUSE_MENU.ReStartSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,122,320,113 };
        PAUSE_MENU.ReStartSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,122,320,113 };
        PAUSE_MENU.ReStartSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,122,320,113 };
        PAUSE_MENU.ReStart.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 280);

        PAUSE_MENU.OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,241,320,113 };
        PAUSE_MENU.OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,241,320,113 };
        PAUSE_MENU.OptionsSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,241,320,113 };
        PAUSE_MENU.OptionsSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,241,320,113 };
        PAUSE_MENU.Options.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 400);

        PAUSE_MENU.ExitSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,361,320,113 };
        PAUSE_MENU.ExitSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,361,320,113 };
        PAUSE_MENU.ExitSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,361,320,113 };
        PAUSE_MENU.ExitSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,361,320,113 };
        PAUSE_MENU.Exit.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 515);
    }
    if (!PAUSE_MENU.PauseMenuBox.loadFromFile("imgs/menu/pause/pausemenubox.png")) {
        cout << "Load gPauseMenu background that bai! " << endl;
        success = false;
    }

    if (!bg.Layer1.loadFromFile("imgs/background/background_layer_1.png")) {
        cout << "Load BG Layer1 that bai! "<<endl;
        success = false;
    }
    if (!bg.Layer2.loadFromFile("imgs/background/background_layer_2.png")) {
        cout << "Load BG Layer2 that bai! " << endl;
        success = false;
    }
    if (!bg.Layer3.loadFromFile("imgs/background/background_layer_3.png")) {
        cout << "Load BG Layer3 that bai! " << endl;
        success = false;
    }
    if (!bg.Layer4.loadFromFile("imgs/background/background_layer_4.png")) {
        cout << "Load BG Layer4 that bai! " << endl;
        success = false;
    }
    if (!bg.Ground.loadFromFile("imgs/background/ground.png")) {
        cout << "Load Ground that bai! " << endl;
        success = false;
    }

    //Shroom enemy mShroom
    if (!mShroom.loadFromFile("imgs/obstacle/shroom.png")) {
        cout << "Load shroom enemy that bai!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> Colliders = {
                {7,3,39,3},
                {4,6,45,3},
                {1,9,51,3},
                {1,12,51,2},
                {0,14,53,10},
                {1,24,51,3},
                {3,27,47,3},
                {6,30,41,3},
                {9,33,35,26}
        };
        LIST_OBSTACLES buffer;
        buffer.setCharacter(mShroom);
        buffer.setColliders(Colliders);
        listObstacles.push_back(buffer);
    }
    if (!mPlantRed.loadFromFile("imgs/obstacle/plantred.png")) {
        cout << "Load plantred enemy that bai!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> Colliders = {
            {18,0,11,2},
            {17,2,15,2},
            {17,4,17,2},
            {16,6,19,2},
            {6,8,40,2},
            {5,10,42,2},
            {5,12,44,6},
            {3,18,47,2},
            {2,20,48,2},
            {1,22,48,2},
            {0,25,52,3},
            {0,28,53,6},
            {2,34,50,6},
            {3,40,36,2},
            {5,42,34,3},
            {13,46,35,14}
        };
        LIST_OBSTACLES buffer;
        buffer.setCharacter(mPlantRed);
        buffer.setColliders(Colliders);
        listObstacles.push_back(buffer);
    }
    if (!mPlantViolet.loadFromFile("imgs/obstacle/plantviolet.png")) {
        cout << "Load plantviolet enemy that bai!" << endl;
        success = false;
    }
    else
    {
        vector <SDL_Rect> Colliders = {
            {0,0,53,38},
            {8,37,37,9},
            {6,46,40,6},
            {4,52,45,8}
        };
        LIST_OBSTACLES buffer;
        buffer.setCharacter(mPlantViolet);
        buffer.setColliders(Colliders);
        listObstacles.push_back(buffer);
    }
    //rabbit
    if (!Rabbit.run.loadFromFile("imgs/characters/Rabbit/Run.png")) {
        cout << "Load rabbit run that bai!" << endl;
        success = false;
    }
    else
    {
        Rabbit.spriteClip_run = {
            //Otter
            {0,0,200,160},
            {200,0,200,160},
            {400,0,200,160}
        };
    }
    if (!Rabbit.jump.loadFromFile("imgs/characters/Rabbit/Jump.png")) {
        cout << "Load rabbit jump that bai!" << endl;
        success = false;
    }
    else
    {
        Rabbit.spriteClip_jump = {
            //Otter
            {0,0,200,160}
            //{200,0,200,160},
            //{400,0,200,160}
        };
    }
    if (!Rabbit.fall.loadFromFile("imgs/characters/Rabbit/land.png")) {
        cout << "Load rabbit land that bai!" << endl;
        success = false;
    }
    else
    {
        Rabbit.spriteClip_fall = {
            //Otter
            {0,0,200,160}
            //{200,0,200,160},
            //{400,0,200,160}
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

            //gSound.PlayMusic();
            //Random
            vector <int> randomDistance = { 100, 300, 500, 200};
            //Collider of Shroom

            Shroom one(mPlantViolet, SCREEN_WIDTH), two(mPlantRed, 0);
            one.setColliders(listObstacles[PLANTVIOLET].getColliders());
            two.setColliders(listObstacles[PLANTRED].getColliders());
            vector <Shroom> test;
            test.push_back(one);
            test.push_back(two);
            
            double a = 0;
            
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    //Character handle event
                    Rabbit.handleEvent(e);

                    //Handle button
                    //START_MENU.handleEvent(e);
                    //PAUSE_MENU.handleEvent(e);
                }
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);

                
                //Background render
                bg.render(speedRender);
                vector<SDL_Rect> shroomClips; vector<SDL_Rect> rect_run = Rabbit.getColliders();
                //Temp random
                for (int i = 0; i <test.size(); i++) {
                    if (!test[i].isOver()) {
                            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                            shroomClips=test[i].getColliders();

                            test[i].render(speedRender);
                            for (auto& x : shroomClips) {
                                SDL_RenderDrawRect(gRenderer, &x);
                            }
                            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

                            if (checkCollision(shroomClips, rect_run)) {
                                switch (Rabbit.getStatus())
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
                                case FASTLANDING:
                                    cout << "fastlanding" << endl;
                                    break;
                                }
                            }
                    }
                    else {
                        int index = generateRandomNumber(0, randomDistance.size()-1);
                        int distance = randomDistance[index];
                        int distanceBetweenTwoObstacles = SCREEN_WIDTH + distance - test[(i==0?1:0)].getX();
                        if (distanceBetweenTwoObstacles >=550 && distanceBetweenTwoObstacles <=1500) {
                            test[i].setX(SCREEN_WIDTH + distance);
                        }
                    }
                }         
                //render rabbit
                Rabbit.show(gRenderer);
                for (auto& x : rect_run) {
                    SDL_RenderDrawRect(gRenderer, &x);
                }
                //Render start menu
                //START_MENU.show();

                //Render pause menu
                //PAUSE_MENU.show();
                
                //Set volume
                //gSound.setVolumeMusic(50);
                //gSound.setVolumeChunk(50);
                
                //render screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();
    return 0;
}