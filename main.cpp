// BopBop.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <math.h>

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

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
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
        static double scrollingOffset_layer1 = 0;
        if (speedRender==0) scrollingOffset_layer1-=0; else
            scrollingOffset_layer1-=1;
        if (scrollingOffset_layer1 < -Layer1.getWidth())
        {
            scrollingOffset_layer1 = 0;
        }
        Layer1.render(scrollingOffset_layer1, 0);
        Layer1.render(scrollingOffset_layer1 + Layer1.getWidth(), 0);

        //Render layer 2
        static double scrollingOffset_layer2 = 0;
        if (speedRender == 0) scrollingOffset_layer2 -= 0; else
            scrollingOffset_layer2 -= 1;
        if (scrollingOffset_layer2 < -Layer2.getWidth())
        {
            scrollingOffset_layer2 = 0;
        }
        Layer2.render(scrollingOffset_layer2, 0);
        Layer2.render(scrollingOffset_layer2 + Layer2.getWidth(), 0);

        //Render layer 3
        static double scrollingOffset_layer3 = 0;
        if (speedRender == 0) scrollingOffset_layer3 -= 0; else
            scrollingOffset_layer3 -= 1;
        if (scrollingOffset_layer3 < -Layer3.getWidth())
        {
            scrollingOffset_layer3 = 0;
        }
        Layer3.render(scrollingOffset_layer3, 0);
        Layer3.render(scrollingOffset_layer3 + Layer3.getWidth(), 0);

        //Render layer 4
        static double scrollingOffset_layer4 = 0;
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

const int GROUND = SCREEN_HEIGHT - 70;
const int JUMP_MAX = 320;
const double GRAVITY =120;

static int frame_run = 0;
static int frame_jump = 0;
static int frame_fall = 0;
static int frame_fastlanding = 0;

//Enemy
LTexture mShroom;
class Shroom {
private:
    
public:

    LTexture* character;
    int x;
    Shroom() {
        x = 0;
        character = NULL;
    }
    Shroom(LTexture &texture, const int &n) {
        character = &texture;
        x = n;
    }
    void setX(const int &n) {
        x = n;
    }
    int getWidth() const{
        return character->getWidth();
    }
    int getHeight() const{
        return character->getHeight();
    }
    int getX() const{
        return x;
    }
    void setCharacter(LTexture &texture){
        character = &texture;
    }
    void Free(){
        character = NULL;
    }
    void render(int speedRender) {
        mShroom.render(x, SCREEN_HEIGHT - 80 - character->getHeight());
        x -= speedRender;
    }
    bool isOver(){
        if (x <= -character->getWidth()) {
            //Free();
            //x = SCREEN_WIDTH;
            return true;
        }
        return false;
    }
};

bool isOverDistance(int distance, const int &x) {
    if (SCREEN_HEIGHT - x >= distance) {
        return true;
    }
    return false;
}
//timer
LTimer timeJump;
enum Rabbit_Sheet_Height {
    RUN_SHEET_HEIGHT=160,
    JUMP_SHEET_HEIGHT= 160,
    FALL_SHEET_HEIGHT= 160,
    FAST_LANDING_HEIGHT= 160
};
class gCharacter {
private:

    double mPosX;
    double mPosY;

    double mVelY;
    bool isFall;
    enum STATUS {
        RUN = 0,
        JUMP = 1,
        FALL = 2,
        FASTLANDING = 3
    };
    int status;

    //SDL_Rect mCollinder;
public:
    LTexture run;
    LTexture jump;
    LTexture fall;
    gCharacter(){
        mPosX = 150;
        mPosY = GROUND-RUN_SHEET_HEIGHT;
        mVelY = 0;
        
        status = 0;

        isFall = 0;
    };
    vector<SDL_Rect> spriteClip_run;
    vector<SDL_Rect> spriteClip_jump;
    vector<SDL_Rect> spriteClip_fall;

    void show(SDL_Renderer *gRenderer) {

        move();
        if (status == RUN) {
            SDL_Rect* currentClip = &spriteClip_run[frame_run / 7];
            const int RUN_FRAME_SIZE = spriteClip_run.size() - 1;
            ++frame_run;
            if (frame_run / 7 > RUN_FRAME_SIZE) {
                frame_run = 0;
            }
            run.render(mPosX, mPosY, currentClip);
        }

        if (status == JUMP) {
            SDL_Rect* currentClip = &spriteClip_jump[frame_jump / 30];
            const int JUMP_FRAME_SIZE = spriteClip_jump.size() - 1;
            ++frame_jump;
            if (frame_jump / 30 > JUMP_FRAME_SIZE) {
                frame_jump = 0;
            }
            jump.render(mPosX, mPosY, currentClip);
        }

        if (status == FALL) {
            SDL_Rect* currentClip = &spriteClip_fall[frame_fall / 30];
            const int JUMP_FRAME_SIZE = spriteClip_fall.size() - 1;
            ++frame_fall;
            if (frame_fall / 30 > JUMP_FRAME_SIZE) {
                frame_fall = 0;
            }
            fall.render(mPosX, mPosY, currentClip);
        }

        if (status == FASTLANDING) {

            SDL_Rect* currentClip = &spriteClip_fall[frame_fastlanding / 8];
            const int JUMP_FRAME_SIZE = spriteClip_fall.size() - 1;
            ++frame_fastlanding;
            if (frame_fastlanding / 8 > JUMP_FRAME_SIZE) {
                frame_fastlanding = 0;
            }
            fall.render(mPosX, mPosY, currentClip);
        }
    }
    void handleEvent(SDL_Event& e) {
        
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_SPACE) {
                if (status == RUN) {
                    status = JUMP;
                    mPosY = GROUND-JUMP_SHEET_HEIGHT;
                    frame_jump = 0;
                    timeJump.start();
                }
            }
            if (e.key.keysym.sym == SDLK_DOWN) {
                if (status != RUN) {
                    mVelY = 18;
                    status = FASTLANDING;
                    frame_fastlanding = 0;
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

        //Fast landing
        if (status == FASTLANDING) {
            mPosY += 30;

            if (mPosY > GROUND - FALL_SHEET_HEIGHT)
            {
                //Move back
                mPosY = GROUND - RUN_SHEET_HEIGHT;
                status = RUN;
                isFall = false;
            }
        }

        if (status == JUMP && mPosY >= JUMP_MAX) {
            mVelY = 220;
            double deltaTime = timeJump.getTicks() / 1000.f;
            mPosY += - (mVelY * (deltaTime)-0.5 * 40 * (deltaTime * deltaTime));
        }
        else if (mPosY <= JUMP_MAX && isFall == false) {
            frame_fall = 0;
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
                //cout << "fall_2" << endl;
            } else { 
                mPosY = GROUND - RUN_SHEET_HEIGHT;
                status = RUN;
                timeJump.stop();
                isFall = false;
            }
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
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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
    //BG layer load

    gFont = TTF_OpenFont("font/lazy.ttf", 28);
    if (gFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
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

    //rabbit
    if (!Rabbit.run.loadFromFile("imgs/characters/Rabbit/Run.png")) {
        cout << "Load rabbit run that bai!" << endl;
        success = false;
    }
    else
    {
        Rabbit.spriteClip_run = {
            /*{2156,0,227,184},
            {1917,0,227,184},
            {1677,0,227,184},
            {1438,0,227,184},
            {1198,0,227,184},
            {958,0,227,184},
            {719,0,227,184},
            {480,0,227,184},
            {240,0,227,184},
            {0,0,227,184}*/

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
            /*{0,0,217,328},
            {229,0,217,328},
            {456,0,217,328},
            {681,0,217,328},
            {907,0,217,328},
            {1133,0,217,328},
            {1358,0,217,328},
            {1585,0,217,328},
            {1810,0,217,328},
            {2036,0,217,328}  */

            
            //Otter
            {0,0,200,160},
            {200,0,200,160},
            {400,0,200,160},
            {600,0,200,160}
        };
    }
    if (!Rabbit.fall.loadFromFile("imgs/characters/Rabbit/land.png")) {
        cout << "Load rabbit land that bai!" << endl;
        success = false;
    }
    else
    {
        Rabbit.spriteClip_fall = {

            /*{2262,0,217,328},
            {2488,0,217,328},
            {2714,0,217,328},
            {2940,0,217,328},
            {3166,0,217,328},
            {3392,0,217,328},
            {3617,0,217,328},
            {3844,0,217,328}*/
            //{4067,0,217,328}

            //Otter
            {0,0,200,160},
            {200,0,200,160},
            {400,0,200,160},
            {600,0,200,160}
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
            //Queue shroom
            //Random
            int previousRandomNumber = 500;
            int randomDistance[5] = { 800,500, 600, 440,700 };
            Shroom one(mShroom, SCREEN_WIDTH), two(mShroom, SCREEN_WIDTH + 500);
            vector <Shroom> test;
            test.push_back(one);
            test.push_back(two);
            
            int speedRender = 10;
            
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
                }
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);

                //Background render
                bg.render(speedRender);
                
                //Temp random
                for (int i = 0; i <2; i++) {
                    if (!test[i].isOver()) {
                            test[i].render(speedRender);
                    }
                    else {

                        int index = generateRandomNumber(0, 4);
                        int distance = randomDistance[index];
                        test[i].setX(SCREEN_WIDTH + distance);
                    }
                }

                //render rabbit
                Rabbit.show(gRenderer);
                
                //Render start menu
                //START_MENU.show();
                SDL_RenderPresent(gRenderer);
               
            }
        }
    }
    close();
    return 0;
}