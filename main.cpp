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
#include "LButton.h"
#include "LScore.h"
#include "gStartMenu.h"
#include "gIntroduceMenu.h"
#include "gPauseMenu.h"
#include "gOptionsMenu.h"
#include "gIngameMenu.h"
#include "gLoseMenu.h"
#include "gExitMenu.h"
#include "ObstacleAndItem.h"
#include "gCharacter.h"
#include "Font.h"
using namespace std;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

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
    loadFont(success, Font, FontMedSize, FontBigSize);
    //Otter
    Otter.loadMedia(success, gRenderer);
    //Sound
    gSound.loadMedia(success);
    //Score
    SCORE.loadMedia(success,gRenderer,Font);
    //START MENU
    START_MENU.loadMedia(success, gRenderer, FontBigSize);
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
    loadRandomList(success, gRenderer, randomListObstacles, randomListCoin, randomListGems);
    return success;
}
void close() {
    //Writing sound infomation
    ofstream soundSavedInfo("sound/VOLUMEINFO.txt", ios::out);
    soundSavedInfo << gSound.getVolumeMusic() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT << " "
        << gSound.getVolumeChunk() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT ;
    soundSavedInfo.close();

    //Writting high score infomation
    ofstream savedScoreInfo("score/score.txt", ios::out);
    savedScoreInfo << SCORE.getHighScore();
    savedScoreInfo.close();

    //double savedVolumeMusic = gSound.getVolumeMusic() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT;
    //double savedVolumeChunk = gSound.getVolumeChunk() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT;

    //SDL_RWops* fileVolumeMusic = SDL_RWFromFile("sound/volumemusic.bin", "r+b");
    //if (fileVolumeMusic != NULL)
    //{
    //    //Save data
    //    SDL_RWwrite(fileVolumeMusic, &savedVolumeMusic, sizeof(double), 1);
    //    //Close file handler
    //    SDL_RWclose(fileVolumeMusic);
    //}
    //else
    //{
    //    cout << "Error: Unable to save file! %s\n" << SDL_GetError();
    //}

    //SDL_RWops* fileVolumeChunk = SDL_RWFromFile("sound/volumechunk.bin", "r+b");
    //if (fileVolumeChunk != NULL)
    //{
    //    //Save data
    //    SDL_RWwrite(fileVolumeChunk, &savedVolumeChunk, sizeof(double), 1);
    //    //Close file handler
    //    SDL_RWclose(fileVolumeChunk);
    //}
    //else
    //{
    //    cout << "Error: Unable to save file! %s\n" << SDL_GetError();
    //}

    ////Open data for writing
    //int savedScore = SCORE.getHighScore();
    //SDL_RWops* fileScore = SDL_RWFromFile("score/score.bin", "w+b");
    //if (fileScore != NULL)
    //{
    //    //Save data
    //    SDL_RWwrite(fileScore, &savedScore, sizeof(int), 1);
    //    //Close file handler
    //    SDL_RWclose(fileScore);
    //}
    //else
    //{
    //    cout << "Error: Unable to save file! %s\n" << SDL_GetError();
    //}

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

void increasingDifficultyLevels(int& speedRender, LScore& score, GAME_STATUS& gameStatus, LSound& gSound);
void mainGameInit();
void mainGameProcess();

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
                        Otter.handleEvent(e, gSound);
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
void increasingDifficultyLevels(int& speedRender, LScore& score, GAME_STATUS& gameStatus, LSound& gSound) {
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

            loopEnemy[i].show(gRenderer, speedRender);
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
                loopCoin[i].show(gRenderer, speedRender);

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
                loopGems[i].show(gRenderer, speedRender);

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
    Otter.show(gRenderer, gameStatus);
    //for (auto& x : rect_run) {
    //    SDL_RenderDrawRect(gRenderer, &x);
    //}
    increasingDifficultyLevels(speedRender, SCORE, gameStatus, gSound);
    //Score render
    SCORE.process();
    SCORE.render(gRenderer,Font);
}
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