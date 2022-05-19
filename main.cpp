// BopBop.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <SDL_ttf.h>
#include <sstream>
#include <math.h>
#include "LTexture.h"
#include "Ltimer.h"
#include "LSound.h"
#include "LCountDown.h"
#include "CommonFunction.h"
#include "gBackground.h"
#include "LScore.h"
#include "LButton.h"
#include "gStartMenu.h"
#include "gIntroduceMenu.h"
#include "gPauseMenu.h"
#include "gOptionsMenu.h"
#include "gIngameMenu.h"
#include "gLoseMenu.h"
#include "gExitMenu.h"
using namespace std;

bool init();
bool loadMedia();
void clode();
void mainGameInit();
bool checkCollision(vector<SDL_Rect>& a, vector<SDL_Rect>& b);
void increasingDifficultyLevels(int& speedRender, LScore& score);
void loadFont(bool& success);
void loadRandomList(bool& success);
int generateRandomNumber(const int min, const int max);
//Font Declare
TTF_Font* Font = NULL;
TTF_Font* FontBigSize = NULL;
TTF_Font* FontMedSize = NULL;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

MENU_STATUS menuStatus = MENU_STATUS_START;
MENU_STATUS menuPre = menuStatus;
GAME_STATUS gameStatus = GAME_STATUS_IDLE;

//Obstacles
static LTexture mShroom_big;
static LTexture mShroom_medium;
static LTexture mShroom_small;

static LTexture mPlantViolet_big;
static LTexture mPlantViolet_medium;
static LTexture mPlantViolet_small;

static LTexture mPlantRed_small;
static LTexture mPlantRed_medium;
static LTexture mPlantRed_big;

static LTexture mMaleBee;
static LTexture mFemaleBee;

//Coin Gems
static LTexture mMonedaD;
static LTexture mMonedaP;
static LTexture mMonedaR;

static LTexture mCoinAma;
static LTexture mCoinAzu;
static LTexture mCoinRoj;
static LTexture mCoinGri;
static LTexture mCoinStrip;

class ObstacleAndItem {
private:
    vector <SDL_Rect> mColliders;
    vector <SDL_Rect> mColliders_offset;
    LTexture* character;

    vector <SDL_Rect> mSpritesClips;
    SDL_Rect* mCurrentClips;
    int mPosX;
    int mPosY;
    int frame;
public:
    ObstacleAndItem();
    ObstacleAndItem(LTexture& texture, const int& n, const vector<SDL_Rect>& Colliders);
    vector<SDL_Rect> getColliders();
    void setSpritesClips(const vector<SDL_Rect>& spritesClips);
    vector<SDL_Rect> getSpritesClips();
    void setColliders(const vector<SDL_Rect>& Colliders);
    void setX(const int& n);
    void setY(const int& n);
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    void setCharacter(LTexture& texture);
    void Free();
    void move();
    void render();
    void show();
    bool isOver();
    void shiftColliders(vector <SDL_Rect>& Colliders);
};
class ObstacleAndItemProperties {
private:
    LTexture* character;
    vector<SDL_Rect> mColliders;
    vector <SDL_Rect> mSpritesClips;
public:
    vector<SDL_Rect> getColliders() const;
    void setSpritesClips(const vector<SDL_Rect>& spritesClips);
    vector<SDL_Rect> getSpritesClips() const;
    void setColliders(const vector<SDL_Rect>& Colliders);
    void setCharacter(LTexture& texture);
    LTexture* getCharacter() const;
};
static vector <ObstacleAndItemProperties> randomListObstacles;
static vector <ObstacleAndItemProperties> randomListCoin;
static vector <ObstacleAndItemProperties> randomListGems;

void RandomObstaclesAndItem(ObstacleAndItem& obstacle, vector <ObstacleAndItemProperties>& randomList);

class gCharacter {
private:
    double mPosX;
    double mPosY;
    
    int count_run;
    int count_jump;
    int count_fall;
    int count_fastlanding;
    int count_idle;

    int runPresentFrame;
    int jumpPresentFrame;
    int fallPresentFrame;
    int fastlandingPresentFrame;
    int idlePresentFrame;

    double mVelY;
    bool isFall;

    //timer
    LTimer timeJump;
    CHARACTER_ACTION action;
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

    //Texture 
    LTexture run;
    LTexture jump;
    LTexture fall;
    LTexture idle;

    vector<SDL_Rect> spriteClip_run;
    vector<SDL_Rect> spriteClip_jump;
    vector<SDL_Rect> spriteClip_fall;
    vector<SDL_Rect> spriteClip_idle;

public:
    gCharacter();
    //use only for character
    void shiftColliders();
    void shiftColliders(vector <SDL_Rect> Colliders[], vector <SDL_Rect> Colliders_offset[], const int& numberOfClips);
    void shiftColliders(vector <SDL_Rect>& Colliders);
    void show(SDL_Renderer* gRenderer);
    void handleEvent(SDL_Event& e);
    void move();
    void loadMedia(bool& success);
    vector <SDL_Rect>& getColliders();
    int getStatus();
};
static gCharacter Otter;

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "Init error: " << SDL_GetError();
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            cout << "Warning: Linear texture filtering not enabled!";
        }

        gWindow = SDL_CreateWindow("Bop!Bop! by duykhanhxx03", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            cout << "Failed to create window! " << SDL_GetError();
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                cout << "Failed to create renderer! " << SDL_GetError();
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    cout << "SDL_image could not initialize! " << IMG_GetError();
                    success = false;
                }

                if (TTF_Init() == -1)
                {
                    cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError();
                    success = false;
                }
                //Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
                {
                    cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError();
                    success = false;
                }
            }
        }
    }
    return success;
}
bool loadMedia() {
    bool success = true;
    //Font
    loadFont(success);
    //Otter
    Otter.loadMedia(success);
    //Sound
    gSound.loadMedia(success);
    //Score
    SCORE.loadMedia(success,gRenderer,Font);
    //START MENU
    START_MENU.loadMedia(success, gRenderer,FontBigSize);
    //PAUSE MENU
    PAUSE_MENU.loadMedia(success,gRenderer);
    //OPTIONS MENU
    OPTIONS_MENU.loadMedia(success, gRenderer, gSound);
    //Exit menu
    EXIT_MENU.loadMedia(success,gRenderer);
    //Ingame menu
    INGAME_MENU.loadMedia(success,gRenderer);
    //Lose menu
    LOSE_MENU.loadMedia(success,gRenderer);
    INTRODUCE_MENU.loadMedia(success,gRenderer);
    //Background
    BACKGROUND.loadMedia(success, gRenderer);
    //Load random list
    loadRandomList(success);
    return success;
}
void close() {
    //Writing sound infomation
    fstream soundSavedInfo("sound/VOLUMEINFO.txt", ios::out);
    soundSavedInfo << gSound.getVolumeMusic() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT << " "
        << gSound.getVolumeChunk() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT ;
    soundSavedInfo.close();

    //Writting high score infomation
    fstream savedScoreInfo("score.txt", ios::out);
    savedScoreInfo<< SCORE.getHighScore();
    savedScoreInfo.close();

    //Free font
    TTF_CloseFont(Font);
    Font = NULL;
    TTF_CloseFont(FontMedSize);
    FontMedSize = NULL;
    TTF_CloseFont(FontBigSize);
    FontBigSize = NULL;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

//DECLARE
static vector <int> randomListDistance = { 100, 300, 500, 200, 400};
static vector <int> randomListCoinTime = { 4, 5, 6};
static vector <int> randomListGemsTime = {12, 14, 16};
static ObstacleAndItem firstPlant, secondPlant;
static vector <ObstacleAndItem> loopEnemy;

//Coin
static ObstacleAndItem coin;
static LTimer coinRandomTimer;
static int coinRandomTime;
static bool isCoinShow;
static vector <ObstacleAndItem> loopCoin;

//Gems
static ObstacleAndItem gems;
static LTimer gemsRandomTimer;
static int gemsRandomTime;
static bool isGemsShow;
static vector <ObstacleAndItem> loopGems;

void mainGameInit() {
    //first plant obstacle
    RandomObstaclesAndItem(firstPlant, randomListObstacles);
    firstPlant.setX(SCREEN_WIDTH);
    //second plant obstacle
    RandomObstaclesAndItem(secondPlant, randomListObstacles);
    secondPlant.setX(SCREEN_WIDTH+ SCREEN_WIDTH);
    //set loopEnemy
    loopEnemy = { firstPlant, secondPlant };

    //Coin
    RandomObstaclesAndItem(coin, randomListCoin);
    coin.setX(1280);
    coin.setY(420);
    coinRandomTime = randomListCoinTime[generateRandomNumber(0, randomListCoinTime.size() - 1)];
    //set loopCoin
    loopCoin = { coin };
    coinRandomTimer.start();
    isCoinShow = false;

    RandomObstaclesAndItem(gems, randomListGems);
    gems.setX(1280);
    gems.setY(555);
    gemsRandomTime = randomListGemsTime[generateRandomNumber(0, randomListGemsTime.size() - 1)];
    //set loopCoin
    loopGems = { gems };
    gemsRandomTimer.start();
    isGemsShow = false;
    
    if (!SCORE.isStarted()) {
        SCORE.start();
        speedRender = 10;
    }
}
void handle(const TO_DO& todo, const double &v) {
    switch (todo)
    {
    case TO_DO_START:
        mainGameInit();
        menuStatus = MENU_STATUS_PLAYING;
        gameStatus = GAME_STATUS_PLAYING;
        break;
    case TO_DO_RESUME:
        speedRender = speedRenderSaved;
        menuStatus = MENU_STATUS_PLAYING;
        gameStatus = GAME_STATUS_PLAYING;
        INGAME_MENU.isPauseOn = true;
        SCORE.resume();
        break;
    case TO_DO_RESTART:
        SCORE.stop();
        mainGameInit();
        menuStatus = MENU_STATUS_PLAYING;
        gameStatus = GAME_STATUS_PLAYING;
        break;
    case TO_DO_OPTIONS:
        menuStatus = MENU_STATUS_OPTIONS;
        break;
    case TO_DO_PAUSE:
        if (speedRender!=0) speedRenderSaved = speedRender;
        speedRender = 0;
        gameStatus = GAME_STATUS_PAUSED;
        menuStatus = MENU_STATUS_PAUSED;
        INGAME_MENU.isPauseOn = false;
        SCORE.pause();
        break;
    case TO_DO_EXIT:
        quit = true;
        break;
    case TO_DO_BACK:
        menuStatus = menuPre;
        break;
    case TO_DO_SET_VOL_BGM:
        gSound.setVolumeMusic(v);
        break;
    case TO_DO_SET_VOL_SFX:
        gSound.setVolumeChunk(v);
        break;
    case TO_DO_TURN_ON_SOUND:
        gSound.turnOnSound();
        OPTIONS_MENU.setPositionX_BGM(gSound.getVolumeMusic() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT);
        OPTIONS_MENU.setPositionX_SFX(gSound.getVolumeChunk() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT);
        INGAME_MENU.isSoundOn = true;
        break;
    case TO_DO_TURN_OFF_SOUND:
        gSound.turnOffSound();
        OPTIONS_MENU.setPositionX_BGM(THRESHOLD_CONTROLER_LEFT);
        OPTIONS_MENU.setPositionX_SFX(THRESHOLD_CONTROLER_LEFT);
        INGAME_MENU.isSoundOn = false;
        break;
    case TO_DO_BACK_HOME:
        SCORE.stop();
        speedRender = 0;
        gameStatus = GAME_STATUS_IDLE;
        menuStatus = MENU_STATUS_START;
        break;
    case TO_DO_COUNTDOWN:
        menuStatus = MENU_STATUS_COUNTDOWN;
        gCountDown.init();
        break;
    case TO_DO_EXIT_MENU:
        menuStatus = MENU_STATUS_EXIT;
        break;
    case TO_DO_INTRODUCE:
        menuStatus = MENU_STATUS_INTRODUCE;
        break;
    }
}
void mainGameProcess() {
    //Background render
    BACKGROUND.render(gRenderer,speedRender);
    vector<SDL_Rect> rect_run = Otter.getColliders();
    for (int i = 0; i < loopEnemy.size(); ++i) {
        if (!loopEnemy[i].isOver()) {
            vector<SDL_Rect> rect_obstacles = loopEnemy[i].getColliders();

            loopEnemy[i].show();
            /*for (auto& x : rect_obstacles) {
                SDL_RenderDrawRect(gRenderer, &x);
            }*/
            //Avoid repeat check collision
            if (menuStatus!=MENU_STATUS_START&& gameStatus!=GAME_STATUS_IDLE&&menuStatus != GAME_STATUS_PAUSED && gameStatus != GAME_STATUS_PAUSED)
            if (checkCollision(rect_obstacles, rect_run)) {
                SCORE.pause();
                speedRender = 0;
                SCORE.updateHighScore(gRenderer,Font);
                if (gameStatus == GAME_STATUS_PLAYING) gSound.PlayLoseSound();
                gameStatus = GAME_STATUS_LOSE;
                menuStatus = MENU_STATUS_LOSE;
            }
        }
        else {
            int index = generateRandomNumber(0, randomListDistance.size() - 1);
            int distance = randomListDistance[index];
            int distanceBetweenTwoObstacles = SCREEN_WIDTH + distance - loopEnemy[(i == 0 ? 1 : 0)].getX();
            if (distanceBetweenTwoObstacles >= 600 && distanceBetweenTwoObstacles <= 1500) {
                loopEnemy[i].setX(SCREEN_WIDTH + distance);
                RandomObstaclesAndItem(loopEnemy[i], randomListObstacles);
            }
        }
    }
    //Coin
    for (int i = 0; i < loopCoin.size(); ++i) {
        if (coinRandomTimer.getTicks() / 1000 == coinRandomTime) {
            isCoinShow = true;
        }
        if (isCoinShow == true) {
            if (!loopCoin[i].isOver()) {
                loopCoin[i].show();

                vector<SDL_Rect> rect_coin = loopCoin[i].getColliders();
                if (checkCollision(rect_coin, rect_run)) {
                    gSound.PlayGainSound();
                    SCORE.addScore(10);
                    RandomObstaclesAndItem(loopCoin[i], randomListCoin);
                    loopCoin[0].setX(1280);
                    loopCoin[0].setY(420);
                    isCoinShow = false;
                    coinRandomTime = randomListCoinTime[generateRandomNumber(0, randomListCoinTime.size() - 1)];
                    coinRandomTimer.start();
                }
            }
            else
            {
                RandomObstaclesAndItem(loopCoin[i], randomListCoin);
                loopCoin[i].setX(1280);
                loopCoin[i].setY(420);
                isCoinShow = false;
                coinRandomTime = randomListCoinTime[generateRandomNumber(0, randomListCoinTime.size() - 1)];
                coinRandomTimer.start();
            }
        }

    }
    //Gems
    for (int i = 0; i < loopGems.size(); ++i) {
        if (gemsRandomTimer.getTicks() / 1000 == gemsRandomTime) {
            isGemsShow = true;
        }
        if (isGemsShow == true) {
            if (!loopGems[i].isOver()) {
                loopGems[i].show();

                vector<SDL_Rect> rect_gems = loopGems[i].getColliders();
                if (checkCollision(rect_gems, rect_run)) {
                    gSound.PlayGainSound();
                    SCORE.addScore(20);
                    RandomObstaclesAndItem(loopGems[i], randomListGems);
                    loopGems[i].setX(1280);
                    loopGems[i].setY(555);
                    isGemsShow = false;
                    gemsRandomTime = randomListGemsTime[generateRandomNumber(0, randomListGemsTime.size() - 1)];
                    gemsRandomTimer.start();
                }
            }
            else
            {
                RandomObstaclesAndItem(loopGems[i], randomListGems);
                loopGems[i].setX(1280);
                loopGems[i].setY(550);
                isGemsShow = false;
                gemsRandomTime = randomListGemsTime[generateRandomNumber(0, randomListGemsTime.size() - 1)];
                gemsRandomTimer.start();
            }
        }

    }
    //render Otter
    Otter.show(gRenderer);
    //for (auto& x : rect_run) {
    //    SDL_RenderDrawRect(gRenderer, &x);
    //}
    increasingDifficultyLevels(speedRender, SCORE);
    //Score render
    SCORE.process();
    SCORE.render(gRenderer,Font);
}

int main(int argc, char* argv[])
{

    srand(time(NULL));
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
            SDL_Event e;
            gSound.PlayMusic();

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } 
                    switch (menuStatus)
                    {
                    case MENU_STATUS_START:
                        START_MENU.handleEvent(e, menuStatus, menuPre);
                        if (e.type == SDL_KEYDOWN)
                            if ((e.key.keysym.sym == SDLK_SPACE) && e.key.repeat == 0) handle(TO_DO_START);
                        break;
                    case MENU_STATUS_PLAYING:
                        Otter.handleEvent(e);
                        INGAME_MENU.handleEvent(e,gameStatus);
                        if (e.type == SDL_KEYDOWN)
                            if (e.key.keysym.sym == SDLK_ESCAPE && e.key.repeat == 0) handle(TO_DO_PAUSE);
                        break;
                    case MENU_STATUS_PAUSED:
                        PAUSE_MENU.handleEvent(e,menuStatus, menuPre);
                        INGAME_MENU.handleEvent(e,gameStatus);
                        if (e.type == SDL_KEYDOWN)
                            if ((e.key.keysym.sym == SDLK_ESCAPE|| e.key.keysym.sym ==SDLK_SPACE) && e.key.repeat == 0) handle(TO_DO_COUNTDOWN);
                        break;
                    case MENU_STATUS_OPTIONS:
                        OPTIONS_MENU.handleEvent(e,menuStatus, menuPre);
                        INGAME_MENU.handleEvent(e,gameStatus);
                        break;
                    case MENU_STATUS_LOSE:
                        LOSE_MENU.handleEvent(e,menuStatus, menuPre);
                        INGAME_MENU.handleEvent(e,gameStatus);
                        if (e.type == SDL_KEYDOWN)
                            if (e.key.keysym.sym == SDLK_ESCAPE && e.key.repeat == 0) handle(TO_DO_BACK_HOME);
                        break;
                    case MENU_STATUS_EXIT:
                        EXIT_MENU.handleEvent(e,menuStatus,menuPre);
                        break;
                    case MENU_STATUS_INTRODUCE:
                        INTRODUCE_MENU.handleEvent(e, menuStatus, menuPre);
                        break;
                    }
                }
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                
                mainGameProcess();

                if (gameStatus == GAME_STATUS_PLAYING || gameStatus == GAME_STATUS_PAUSED || gameStatus == GAME_STATUS_LOSE) {
                    INGAME_MENU.show(gRenderer);
                }
                switch (menuStatus)
                {
                    case MENU_STATUS_START:
                        START_MENU.show(gRenderer, FontBigSize);
                        break;
                    case MENU_STATUS_INTRODUCE:
                        INTRODUCE_MENU.show(gRenderer);
                        break;
                    case MENU_STATUS_PAUSED:
                        PAUSE_MENU.show(gRenderer);
                        break;
                    case MENU_STATUS_OPTIONS:
                        OPTIONS_MENU.show(gRenderer);
                        break;
                    case MENU_STATUS_LOSE:
                        LOSE_MENU.show(gRenderer, Font, FontMedSize, SCORE);
                        break;
                    case MENU_STATUS_EXIT:
                        EXIT_MENU.show(gRenderer);
                        break;
                    case MENU_STATUS_COUNTDOWN:
                        if (!gCountDown.isEndCountDown()) gCountDown.show(gRenderer, FontBigSize); else handle(TO_DO_RESUME);
                        break;
                }
                
                //render screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();
    return 0;
}

//Otter
gCharacter::gCharacter() {
    count_run = 0;
    count_jump = 0;
    count_fall = 0;
    count_fastlanding = 0;
    count_idle = 0;

    runPresentFrame = 0;
    jumpPresentFrame = 0;
    fallPresentFrame = 0;
    fastlandingPresentFrame = 0;

    mPosX = 150;
    mPosY = GROUND - RUN_SHEET_HEIGHT;
    mVelY = 0;
    
    action = CHARACTER_ACTION_RUN;

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

    mCollidersRun[0] = mCollidersRun_offset[0] = {
        {60,112,123,24},
        {127,135,23,9},
        {118,144,22,6},
        {112,150,16,4},
        {60,134,47,26}
    };
    mCollidersRun[1] = mCollidersRun_offset[1] = {
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
    mCollidersRun[2] = mCollidersRun_offset[2] = {
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
void gCharacter::shiftColliders() {
    shiftColliders(mCollidersFastLanding, mCollidersFastLanding_offset, numberOffastlandingClips);
    shiftColliders(mCollidersJump, mCollidersJump_offset, numberOfjumpClips);
    shiftColliders(mCollidersFall, mCollidersFall_offset, numberOffallClips);
    shiftColliders(mCollidersRun, mCollidersRun_offset, numberOfrunClips);
}
void gCharacter::shiftColliders(vector <SDL_Rect> Colliders[], vector <SDL_Rect> Colliders_offset[], const int& numberOfClips)
{
    //Go through the dot's collision boxes
    for (int index = 0; index < numberOfClips; index++)
        for (int set = 0; set < Colliders[index].size(); ++set)
        {
            //Center the collision box
            Colliders[index][set].x = mPosX + Colliders_offset[index][set].x;
            //Set the collision box at its row offset
            Colliders[index][set].y = mPosY + Colliders_offset[index][set].y;

        }
}
void gCharacter::shiftColliders(vector <SDL_Rect>& Colliders)
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
void gCharacter::show(SDL_Renderer* gRenderer) {
    if (gameStatus == GAME_STATUS_IDLE) {
        idlePresentFrame = count_idle++ / 10;

        SDL_Rect* currentClip = &spriteClip_idle[idlePresentFrame];
        const int IDLE_FRAME_SIZE = spriteClip_idle.size() - 1;
        if (count_idle / 10 > IDLE_FRAME_SIZE) {
            count_idle = 0;
        }
        idle.render(gRenderer, mPosX, mPosY, currentClip);
    }
    else {
        if (action == CHARACTER_ACTION_RUN) {
            runPresentFrame = count_run / 7;
            if (gameStatus == GAME_STATUS_PLAYING) count_run++;
            SDL_Rect* currentClip = &spriteClip_run[runPresentFrame];
            const int RUN_FRAME_SIZE = spriteClip_run.size() - 1;
            if (count_run / 7 > RUN_FRAME_SIZE) {
                count_run = 0;
            }
            run.render(gRenderer, mPosX, mPosY, currentClip);
        }

        if (action == CHARACTER_ACTION_JUMP) {
            jumpPresentFrame = count_jump / 20;
            if (gameStatus == GAME_STATUS_PLAYING) count_jump++;
            SDL_Rect* currentClip = &spriteClip_jump[jumpPresentFrame];
            const int JUMP_FRAME_SIZE = spriteClip_jump.size() - 1;
            if (count_jump / 20 > JUMP_FRAME_SIZE) {
                count_jump = 0;
            }
            jump.render(gRenderer, mPosX, mPosY, currentClip);
        }

        if (action == CHARACTER_ACTION_FALL) {
            fallPresentFrame = count_fall / 7;
            if (gameStatus == GAME_STATUS_PLAYING) count_fall++;
            SDL_Rect* currentClip = &spriteClip_fall[fallPresentFrame];
            const int FALL_FRAME_SIZE = spriteClip_fall.size() - 1;
            if (count_fall / 7 > FALL_FRAME_SIZE) {
                count_fall = 0;
            }
            fall.render(gRenderer, mPosX, mPosY, currentClip);
        }

        if (action == CHARACTER_ACTION_FASTLANDING) {
            fastlandingPresentFrame = count_fastlanding / 8;
            if (gameStatus == GAME_STATUS_PLAYING) count_fastlanding++;
            SDL_Rect* currentClip = &spriteClip_fall[fastlandingPresentFrame];
            const int FASTLANDING_FRAME_SIZE = spriteClip_fall.size() - 1;
            if (count_fastlanding / 8 > FASTLANDING_FRAME_SIZE) {
                count_fastlanding = 0;
            }
            fall.render(gRenderer, mPosX, mPosY, currentClip);
        }
        move();
    }
}
void gCharacter::handleEvent(SDL_Event& e) {

    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_SPACE) {
            if (action == CHARACTER_ACTION_RUN) {
                action = CHARACTER_ACTION_JUMP;
                mPosY = GROUND - JUMP_SHEET_HEIGHT;
                count_jump = 0;
                timeJump.start();
                gSound.PlayJumpSound();
            }
        }
        if (e.key.keysym.sym == SDLK_DOWN) {
            if (action != CHARACTER_ACTION_RUN) {
                mVelY = 18;
                action = CHARACTER_ACTION_FASTLANDING;
                count_fastlanding = 0;
            }
        }
    }
}
void gCharacter::move()
{
    //Processing

    shiftColliders();
    //Fast landing
    if (action == CHARACTER_ACTION_FASTLANDING) {
        mPosY += 45;
        shiftColliders();
        if (mPosY > GROUND - FALL_SHEET_HEIGHT)
        {
            //Move back
            mPosY = GROUND - RUN_SHEET_HEIGHT;
            shiftColliders();
            action = CHARACTER_ACTION_RUN;
            isFall = false;
        }
    }

    if (action == CHARACTER_ACTION_JUMP && mPosY >= JUMP_MAX) {
        mVelY = 245;
        double deltaTime = timeJump.getTicks() / 1000.f;
        mPosY += -(mVelY * (deltaTime)-0.5 * 40 * (deltaTime * deltaTime));
        shiftColliders();
    }
    else if (mPosY <= JUMP_MAX && isFall == false) {
        count_fall = 0;
        action = CHARACTER_ACTION_FALL;
        //timeJump.stop();
        timeJump.start();
        isFall = true;
    }
    if (action == CHARACTER_ACTION_FALL) {
        if (mPosY <= GROUND - FALL_SHEET_HEIGHT) {
            mVelY = 85;
            double deltaTime = timeJump.getTicks() / 1000.f;
            mPosY += mVelY * (deltaTime)+0.5 * 40 * (deltaTime * deltaTime);
            shiftColliders();
        }
        else {
            mPosY = GROUND - RUN_SHEET_HEIGHT;
            shiftColliders();
            action = CHARACTER_ACTION_RUN;
            timeJump.stop();
            isFall = false;
        }
    }
}
void gCharacter::loadMedia(bool& success) {
    if (!run.loadFromFile("imgs/characters/Otter/Run.png", gRenderer)) {
        cout << "Load Otter run failed!" << endl;
        success = false;
    }
    else
    {
        spriteClip_run = {
            //Otter
            {0,0,200,160},
            {200,0,200,160},
            {400,0,200,160}
        };
    }
    if (!jump.loadFromFile("imgs/characters/Otter/Jump.png", gRenderer)) {
        cout << "Load Otter jump failed!" << endl;
        success = false;
    }
    else
    {
        spriteClip_jump = {
            //Otter
            {0,0,200,160}
            //{200,0,200,160},
            //{400,0,200,160}
        };
    }
    if (!fall.loadFromFile("imgs/characters/Otter/land.png", gRenderer)) {
        cout << "Load Otter land failed!" << endl;
        success = false;
    }
    else
    {
        spriteClip_fall = {
            //Otter
            {0,0,200,160}
            //{200,0,200,160},
            //{400,0,200,160}
        };
    }
    if (!idle.loadFromFile("imgs/characters/Otter/idle.png", gRenderer)) {
        cout << "Load Otter idle failed!" << endl;
        success = false;
    }
    else
    {
        spriteClip_idle = {
            {0,0,250,160},
            {250,0,250,160},
            {500,0,250,160},
            {750,0,250,160},
            {1000,0,250,160},
            {1250,0,250,160},
            {1500,0,250,160},
            {1750,0,250,160},
            {2000,0,250,160},
            {2250,0,250,160},
            {2500,0,250,160},
            {2750,0,250,160},
            {3000,0,250,160},
            {3250,0,250,160},
            {3500,0,250,160},
            {3750,0,250,160},
        };
    }
}
vector <SDL_Rect>& gCharacter::getColliders()
{
    switch (action)
    {
    case CHARACTER_ACTION_JUMP:
        return mCollidersJump[jumpPresentFrame];
        break;
    case CHARACTER_ACTION_FALL:
        return mCollidersFall[fallPresentFrame];
        break;
    case CHARACTER_ACTION_FASTLANDING:
        return mCollidersFastLanding[fastlandingPresentFrame];
        break;
    default:
        return mCollidersRun[runPresentFrame];
        break;
    }
}
int gCharacter::getStatus() {
    return action;
}

//Obstacle and item
ObstacleAndItem::ObstacleAndItem() {
    mPosX = 0;
    character = NULL;
    mPosY = 0;
    mCurrentClips = NULL;
    frame = 0;
}
ObstacleAndItem::ObstacleAndItem(LTexture& texture, const int& n, const vector<SDL_Rect>& Colliders) {
    character = &texture;
    mPosX = n;
    mPosY = SCREEN_HEIGHT - 80 - character->getHeight() + 5;
    //set colliders
    mColliders = mColliders_offset = Colliders;
    shiftColliders(mColliders);
}
vector<SDL_Rect> ObstacleAndItem::getColliders() {
    return mColliders;
}
vector<SDL_Rect> ObstacleAndItem::getSpritesClips() {
    return mSpritesClips;
}
void ObstacleAndItem::setSpritesClips(const vector<SDL_Rect>& spritesClips) {
    mSpritesClips = spritesClips;
}
void ObstacleAndItem::setColliders(const vector<SDL_Rect>& Colliders) {
    mColliders = mColliders_offset = Colliders;
    shiftColliders(mColliders);
}
void ObstacleAndItem::setX(const int& n) {
    mPosX = n;
}
void ObstacleAndItem::setY(const int& n) {
    mPosY = n;
}
void ObstacleAndItem::setCharacter(LTexture& texture) {
    character = &texture;
    mPosY = SCREEN_HEIGHT - 80 - character->getHeight() + 5;
}
void ObstacleAndItem::Free() {
    character = NULL;
}
void ObstacleAndItem::move() {
    mPosX -= speedRender;
    shiftColliders(mColliders);
}
void ObstacleAndItem::render() {
    character->render(gRenderer, mPosX, mPosY);
    move();
}
void ObstacleAndItem::show() {
    if (++frame / 7 > mSpritesClips.size() - 1) {
        frame = 0;
    }
    mCurrentClips = &mSpritesClips[frame / 7];
    character->render(gRenderer, mPosX, mPosY, mCurrentClips);
    move();
}
void ObstacleAndItem::shiftColliders(vector <SDL_Rect>& Colliders)
{
    for (int set = 0; set < Colliders.size(); ++set)
    {
        Colliders[set].x = mPosX + mColliders_offset[set].x;
        Colliders[set].y = mPosY + mColliders_offset[set].y;
    }
}
bool ObstacleAndItem::isOver() {
    if (mPosX <= -character->getWidth()) {
        return true;
    }
    return false;
}
int ObstacleAndItem::getX() const {
    return mPosX;
}
int ObstacleAndItem::getY() const {
    return mPosY;
}
int ObstacleAndItem::getWidth() const {
    return character->getWidth();
}
int ObstacleAndItem::getHeight() const {
    return character->getHeight();
}

//Obstacle and item properties
vector<SDL_Rect> ObstacleAndItemProperties::getColliders() const {
    return mColliders;
}
void ObstacleAndItemProperties::setSpritesClips(const vector<SDL_Rect>& spritesClips) {
    mSpritesClips = spritesClips;
}
vector<SDL_Rect> ObstacleAndItemProperties::getSpritesClips() const {
    return mSpritesClips;
}
void ObstacleAndItemProperties::setColliders(const vector<SDL_Rect>& Colliders) {
    mColliders = Colliders;
}
void ObstacleAndItemProperties::setCharacter(LTexture& texture) {
    character = &texture;
}
LTexture* ObstacleAndItemProperties::getCharacter() const {
    return character;
}

void RandomObstaclesAndItem(ObstacleAndItem& obstacle, vector <ObstacleAndItemProperties>& randomList) {
    int index = generateRandomNumber(0, randomList.size() - 1);

    obstacle.setCharacter(*randomList[index].getCharacter());
    obstacle.setColliders(randomList[index].getColliders());
    obstacle.setSpritesClips(randomList[index].getSpritesClips());
    static vector <int> PosYofBee = { 500,550, 580 };
    
    //500 550 580
    if (index == BEE_FEMALE || index == BEE_MALE) {
        int index_ = generateRandomNumber(0, PosYofBee.size() - 1);
        obstacle.setY(PosYofBee[index_]);
    }
}
int generateRandomNumber(const int min, const int max)
{
    // TODO: Return a random integer number between min and max
    return rand() % (max - min + 1) + min;
}
void increasingDifficultyLevels(int& speedRender, LScore& score) {
    //switch (score.getScore())
    //{
    //case 100:
    //    if (speedRender == 10) gSound.PlayPassSound();
    //    speedRender = 12;
    //    break;
    //case 200:
    //    if (speedRender == 12) gSound.PlayPassSound();
    //    speedRender = 15;
    //    break;
    //case 300:
    //    if (speedRender == 15) gSound.PlayPassSound();
    //    speedRender = 16;
    //    break;
    //case 400:
    //    if (speedRender == 16) gSound.PlayPassSound();
    //    speedRender = 20;
    //    break;
    //}//
    if (gameStatus == GAME_STATUS_PLAYING) {
        if (score.getScore() >= 100 && score.getScore() < 200) {
            if (speedRender == 10) gSound.PlayPassSound();
            speedRender = 12;
        }
        if (score.getScore() >= 200 && score.getScore() < 300) {
            if (speedRender == 12) gSound.PlayPassSound();
            speedRender = 15;
        }
        if (score.getScore() >= 300 && score.getScore() < 400) {
            if (speedRender == 15) gSound.PlayPassSound();
            speedRender = 16;
        }
        if (score.getScore() >= 400) {
            if (speedRender == 16) gSound.PlayPassSound();
            speedRender = 20;
        }
    }
}
bool checkCollision(vector<SDL_Rect>& a, vector<SDL_Rect>& b)
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
void loadFont(bool& success) {
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
}
void loadRandomList(bool& success) {
    randomListObstacles.resize(11);
    randomListCoin.resize(5);
    randomListGems.resize(3);
    //Shroom enemy mShroom
    if (!mShroom_big.loadFromFile("imgs/obstacle/shroom_big.png", gRenderer)) {
        cout << "Load shroom big enemy failed!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> spritesClips = {
            {0,0,79,90}
        };
        vector <SDL_Rect> Colliders = {
            {19,0,40,4},
            {11,4,57,5},
            {7,9,65,5},
            {2,14,75,7},
            {0,20,79,16},
            {2,36,75,7},
            {6,43,67,5},
            {13,48,53,42}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mShroom_big);
        buffer.setColliders(Colliders);
        randomListObstacles[SHROOM_BIG] = buffer;
    }
    if (!mShroom_medium.loadFromFile("imgs/obstacle/shroom_medium.png", gRenderer)) {
        cout << "Load shroom small enemy failed!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> spritesClips = {
            {0,0,66,75}
        };
        vector <SDL_Rect> Colliders = {
            {16,0,33,4},
            {9,4,48,4},
            {6,8,54,4},
            {1,12,63,5},
            {0,17,66,13},
            {1,30,64,6},
            {5,36,56,4},
            {11,40,44,35}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mShroom_medium);
        buffer.setColliders(Colliders);
        randomListObstacles[SHROOM_MEDIUM] = buffer;
    }
    if (!mShroom_small.loadFromFile("imgs/obstacle/shroom_small.png", gRenderer)) {
        cout << "Load shroom small enemy failed!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> spritesClips = {
            {0,0,53,60}
        };
        vector <SDL_Rect> Colliders = {
            {13,0,27,3},
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
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mShroom_small);
        buffer.setColliders(Colliders);
        randomListObstacles[SHROOM_SMALL] = buffer;
    }
    if (!mPlantRed_big.loadFromFile("imgs/obstacle/plantred_big.png", gRenderer)) {
        cout << "Load plantred enemy failed!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> spritesClips = {
            {0,0,80,90}
        };
        vector <SDL_Rect> Colliders = {
            {27,0,17,3},
            {25,3,23,3},
            {25,6,25,3},
            {24,9,29,3},
            {9,12,60,3},
            {7,15,64,3},
            {7,18,67,8},
            {3,27,72,6},
            {1,33,73,4},
            {0,37,77,5},
            {0,42,80,6},
            {1,47,79,6},
            {3,53,74,7},
            {4,60,55,8},
            {20,70,53,20}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mPlantRed_big);
        buffer.setColliders(Colliders);
        randomListObstacles[PLANTRED_BIG] = buffer;
    }
    if (!mPlantRed_medium.loadFromFile("imgs/obstacle/plantred_medium.png", gRenderer)) {
        cout << "Load plantred enemy failed!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> spritesClips = {
            {0,0,66,75}
        };
        vector <SDL_Rect> Colliders = {
            {22,0,16,3},
            {21,3,19,3},
            {20,6,23,4},
            {6,10,53,4},
            {6,14,55,8},
            {4,22,58,3},
            {2,25,60,3},
            {1,28,60,3},
            {0,31,64,3},
            {0,34,66,6},
            {1,40,65,5},
            {2,45,61,5},
            {3,50,46,6},
            {17,58,43,17}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mPlantRed_medium);
        buffer.setColliders(Colliders);
        randomListObstacles[PLANTRED_MEDIUM] = buffer;
    }
    if (!mPlantRed_small.loadFromFile("imgs/obstacle/plantred_small.png", gRenderer)) {
        cout << "Load plantred enemy failed!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> spritesClips = {
            {0,0,53,60}
        };
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
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mPlantRed_small);
        buffer.setColliders(Colliders);
        randomListObstacles[PLANTRED_SMALL] = buffer;
    }

    if (!mPlantViolet_big.loadFromFile("imgs/obstacle/plantviolet_big.png", gRenderer)) {
        cout << "Load plantviolet enemy failed!" << endl;
        success = false;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
            {0,0,79,90}
        };
        vector <SDL_Rect> Colliders = {
            {6,70,67,20},
            {12,56,55,15},
            {0,50,79,6},
            {2,42,75,8},
            {1,21,77,21},
            {2,12,75,9},
            {0,0,79,12}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mPlantViolet_big);
        buffer.setColliders(Colliders);
        randomListObstacles[PLANTVIOLET_BIG] = buffer;
    }
    if (!mPlantViolet_medium.loadFromFile("imgs/obstacle/plantviolet_medium.png", gRenderer)) {
        cout << "Load plantviolet enemy failed!" << endl;
        success = false;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
            {0,0,66,75}
        };
        vector <SDL_Rect> Colliders = {
            {5,58,56,17},
            {10,47,47,11},
            {0,42,66,5},
            {2,36,62,6},
            {1,18,64,19},
            {1,10,64,8},
            {0,0,66,10}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mPlantViolet_medium);
        buffer.setColliders(Colliders);
        randomListObstacles[PLANTVIOLET_MEDIUM] = buffer;
    }
    if (!mPlantViolet_small.loadFromFile("imgs/obstacle/plantviolet_small.png", gRenderer)) {
        cout << "Load plantviolet enemy failed!" << endl;
        success = false;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
            {0,0,53,60}
        };
        vector <SDL_Rect> Colliders = {
            {0,0,53,38},
            {8,37,37,9},
            {6,46,40,6},
            {4,52,45,8}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mPlantViolet_small);
        buffer.setColliders(Colliders);
        randomListObstacles[PLANTVIOLET_SMALL] = buffer;
    }
    if (!mMaleBee.loadFromFile("imgs/obstacle/malebee.png", gRenderer)) {
        cout << "Load mGogleEyesBee enemy failed!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> spritesClips = {
            {0,0,61,40},
            {62,0,61,40}
        };
        vector <SDL_Rect> Colliders = {
            {3,2,55,36}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mMaleBee);
        buffer.setColliders(Colliders);
        randomListObstacles[BEE_MALE] = buffer;
    }
    if (!mFemaleBee.loadFromFile("imgs/obstacle/femalebee.png", gRenderer)) {
        cout << "Load mGogleEyesBee enemy failed!" << endl;
        success = false;
    }
    else {
        vector <SDL_Rect> spritesClips = {
            {0,0,61,40},
            {62,0,61,40}
        };
        vector <SDL_Rect> Colliders = {
            {3,2,55,36}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mFemaleBee);
        buffer.setColliders(Colliders);
        randomListObstacles[BEE_FEMALE] = buffer;
    }

    //Coin
    if (!mCoinAma.loadFromFile("imgs/coingems/CoinAma.png", gRenderer)) {
        success = false;
        cout << "" << endl;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
            {0,0,48,48},
           {48,0,48,48},
           {96,0,48,48},
           {144,0,48,48}
        };
        vector <SDL_Rect> Colliders = {
            {3,3,30,45}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mCoinAma);
        buffer.setColliders(Colliders);
        randomListCoin[COIN_AMA] = buffer;
    }
    if (!mCoinAzu.loadFromFile("imgs/coingems/CoinAzu.png", gRenderer)) {
        success = false;
        cout << "" << endl;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
            {0,0,48,48},
           {48,0,48,48},
           {96,0,48,48},
           {144,0,48,48}
        };
        vector <SDL_Rect> Colliders = {
            {3,3,30,45}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mCoinAzu);
        buffer.setColliders(Colliders);
        randomListCoin[COIN_AZU] = buffer;
    }
    if (!mCoinRoj.loadFromFile("imgs/coingems/CoinRoj.png", gRenderer)) {
        success = false;
        cout << "" << endl;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
            {0,0,48,48},
           {48,0,48,48},
           {96,0,48,48},
           {144,0,48,48}
        };
        vector <SDL_Rect> Colliders = {
            {3,3,30,45}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mCoinRoj);
        buffer.setColliders(Colliders);
        randomListCoin[COIN_ROJ] = buffer;
    }
    if (!mCoinGri.loadFromFile("imgs/coingems/CoinGri.png", gRenderer)) {
        success = false;
        cout << "" << endl;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
            {0,0,48,48},
           {48,0,48,48},
           {96,0,48,48},
           {144,0,48,48}
        };
        vector <SDL_Rect> Colliders = {
            {3,3,30,45}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mCoinGri);
        buffer.setColliders(Colliders);
        randomListCoin[COIN_GRI] = buffer;
    }
    if (!mCoinStrip.loadFromFile("imgs/coingems/CoinStrip.png", gRenderer)) {
        success = false;
        cout << "" << endl;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
            {0,0,48,48},
           {48,0,48,48},
           {96,0,48,48},
           {144,0,48,48}
        };
        vector <SDL_Rect> Colliders = {
            {3,3,30,45}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mCoinStrip);
        buffer.setColliders(Colliders);
        randomListCoin[COIN_STRIP] = buffer;
    }

    //Gems
    if (!mMonedaD.loadFromFile("imgs/coingems/MonedaD.png", gRenderer)) {
        success = false;
        cout << "" << endl;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
            {0,0,48,48},
            {48,0,48,48},
            {96,0,48,48},
            {144,0,48,48},
            {192,0,48,48}
        };
        vector <SDL_Rect> Colliders = {
            {3,0,42,48}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mMonedaD);
        buffer.setColliders(Colliders);
        randomListGems[GEMS_MONEDA_D] = buffer;
    }
    if (!mMonedaP.loadFromFile("imgs/coingems/MonedaP.png", gRenderer)) {
        success = false;
        cout << "" << endl;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
           {0,0,48,48},
           {48,0,48,48},
           {96,0,48,48},
           {144,0,48,48},
           {192,0,48,48}
        };
        vector <SDL_Rect> Colliders = {
            {3,0,42,48}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mMonedaP);
        buffer.setColliders(Colliders);
        randomListGems[GEMS_MONEDA_P] = buffer;
    }
    if (!mMonedaR.loadFromFile("imgs/coingems/MonedaR.png", gRenderer)) {
        success = false;
        cout << "" << endl;
    }
    else
    {
        vector <SDL_Rect> spritesClips = {
           {0,0,48,48},
           {48,0,48,48},
           {96,0,48,48},
           {144,0,48,48},
           {192,0,48,48}
        };
        vector <SDL_Rect> Colliders = {
            {3,0,42,48}
        };
        ObstacleAndItemProperties buffer;
        buffer.setSpritesClips(spritesClips);
        buffer.setCharacter(mMonedaR);
        buffer.setColliders(Colliders);
        randomListGems[GEMS_MONEDA_R] = buffer;
    }
}