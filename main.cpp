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
#include <sstream>
#include <math.h>
using namespace std;

bool init();
bool loadMedia();
void clode();

bool quit = false;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
int speedRender = 0;
// 10 12 15 18 20
int speedRenderSaved = 0;
int saved_speedRender = 10;
enum TO_DO {
    TO_DO_START,
    TO_DO_BACK,
    TO_DO_RESUME,
    TO_DO_RESTART,
    TO_DO_PAUSE,
    TO_DO_EXIT_MENU,
    TO_DO_EXIT,
    TO_DO_OPTIONS,
    TO_DO_SET_VOL_BGM,
    TO_DO_SET_VOL_SFX,
    TO_DO_TURN_OFF_SOUND,
    TO_DO_TURN_ON_SOUND,
    TO_DO_BACK_HOME,
    TO_DO_COUNTDOWN
};
enum ONOFFSTAR {
    OFF_STAR = 0,
    ON_STAR = 1,
    ONOFF_STAR_TOTAL = 2
};
enum GAME_STATUS {
    GAME_STATUS_IDLE,
    GAME_STATUS_PLAYING,
    GAME_STATUS_PAUSED,
    GAME_STATUS_LOSE
};
enum MENU_STATUS {
    MENU_STATUS_START,
    MENU_STATUS_PLAYING,
    MENU_STATUS_PAUSED,
    MENU_STATUS_OPTIONS,
    MENU_STATUS_COUNTDOWN,
    MENU_STATUS_LOSE,
    MENU_STATUS_EXIT
};
MENU_STATUS menuStatus = MENU_STATUS_START;
MENU_STATUS menuPre = menuStatus;
GAME_STATUS gameStatus = GAME_STATUS_IDLE;
void mainGameInit();
void handle(const TO_DO& todo, const double& v = MIX_MAX_VOLUME);
enum Otter_Sheet_Height {
    RUN_SHEET_HEIGHT = 160,
    JUMP_SHEET_HEIGHT = 160,
    FALL_SHEET_HEIGHT = 160,
    FAST_LANDING_HEIGHT = 160
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

class ControllerStatus {
public:
    bool isClick;
    int x_len;
    int y_len;
    ControllerStatus() {
        isClick = false;
        x_len = 0;
        y_len = 0;
    }
};
//Threshold controller
const int THRESHOLD_CONTROLER_LEFT = 570;
const int THRESHOLD_CONTROLER_RIGHT = 850;

enum LSwitchSprite {
    SWITCH_SPRITE_MOUSE_ON = 0,
    SWITCH_SPRITE_MOUSE_OFF = 1,
    SWITCH_SPRITE_TOTAL = 2
};

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
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;
TTF_Font* gFontBigSize = NULL;
TTF_Font* gFontMedSize = NULL;

class LTexture {
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(string path);
    void free();

#if defined(SDL_TTF_MAJOR_VERSION)
    //Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* gFont);
#endif

    //set color mudulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //set blending
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);

    void render(int x, int y, SDL_Rect* clip = NULL, int mScale = 1);

    int getWidth();
    int getHeight();
private:
    SDL_Texture* mTexture;

    int mWidth;
    int mHeight;
};
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

class LSound {
private:
    Mix_Music* gBgm;
    Mix_Chunk* gLoseSound;
    Mix_Chunk* gJumpSound;
    Mix_Chunk* gPassSound;
    Mix_Chunk* gGainSound;
    //MIX_MAX_VOLUME=128
    int volumeMusic = 128;
    int volumeChunk = 128;
    
    int savedVolumeMusic = volumeMusic;
    int savedVolumeChunk = volumeChunk;
public:
    bool isPlayingMusic();
    bool isPausedMusic();
    void StopMusic();
    void PlayMusic();
    void PlayPassSound();
    void PauseMusic();
    void ResumeMusic();
    void PlayJumpSound();
    void PlayGainSound();
    void PlayLoseSound();
    int getVolumeMusic();
    int getVolumeChunk();
    void setVolumeMusic(const int& v);
    void setVolumeChunk(const int& v);
    void turnOffSound();
    void turnOnSound();
    void loadMedia(bool& success);
};
LSound gSound;

class LCountDown {
private:
    int count;
    bool isEnd;
    stringstream timeText;
    LTexture textCountDown;
    LTimer timer;
    SDL_Color textColor = { 255, 255, 255, 255 };
public:
    void init();
    LCountDown();
    void render();
    void show();
    bool isEndCountDown();
};
LCountDown countDown;

class LScore {
private:
    SDL_Color textColor = { 255, 255, 255, 255 };
    //Text render
    LTexture gCurrentScoreTexture;
    LTexture gHighScoreTexture;

    //Current time start time
    Uint32 startTime = 0;
    stringstream timeText;
    LTimer gTimer;

    //ishighscore
    bool isHighSc=false;
    Uint32 score = 0;
    Uint32 savedScore = 0;
    Uint32 scoreAdded = 0;
public:
    void process();
    void pause();
    void start();
    void reStart();
    void render();
    void shiftScore();
    void addScore(const Uint32& sc);
    void resume();
    void stop();
    void updateHighScore();
    int getScore();
    bool isStarted();
    void setScoreFromSaved(const Uint32& sc);
    Uint32 getHighScore();
    bool isHighScore();
    void loadMedia(bool& success);
};
LScore SCORE;

//Button
class LButton
{
public:
    //Initializes internal variables
    LButton();

    //Sets top left position
    void setPosition(int x, int y);

    //Handles mouse event
    void handleEventController(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT, ControllerStatus& status,const TO_DO &todo);
    void handleEventSwitch(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT, bool& isOn, const TO_DO& todo);

    void handleEvent(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT,const TO_DO &todo=TO_DO_START, bool isBack = false);
    //Shows button sprite
    void render(LTexture& gButtonSpriteSheetTexture, SDL_Rect SpriteClips[]);
    void renderSwitch(LTexture& gButtonSpriteSheetTexture, SDL_Rect SpriteClips[]);
    void renderController(LTexture& gButtonSpriteSheetTexture);
    void setPositionX(const int& x);

private:
    //Top left position
    SDL_Point mPosition;

    //Currently used global sprite
    LButtonSprite mCurrentSprite;

    //Currently used global sprite
    LSwitchSprite mSwitchCurrentSprite;
    //Switch status
};

class gStartMenu {
private:
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
public:
    gStartMenu() {};
    void handleEvent(SDL_Event& e);
    void show();
    void loadMedia(bool& success);
};
gStartMenu START_MENU;

class gPauseMenu {
private:
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
public:
    gPauseMenu() {};
    void handleEvent(SDL_Event& e);
    void show();
    void loadMedia(bool& success);
};
gPauseMenu PAUSE_MENU;

class gOptionsMenu {
private:
    //Start menu texture
    LTexture OptionsMenuBox;
    LTexture MenuBackground;
    LTexture BGMTexture;
    LTexture SFXTexture;
    LTexture BackTexture;

    //Button
    LButton Back;
    LButton SFX;
    LButton BGM;

    //status
    ControllerStatus SFXstatus;
    ControllerStatus BGMstatus;

    //Clips
    SDL_Rect BackSpriteClips[BUTTON_SPRITE_TOTAL];
public:
    gOptionsMenu() {};
    void handleEvent(SDL_Event& e);
    void show();
    void setPositionX_BGM(const int& x);
    void setPositionX_SFX(const int& x);
    void loadMedia(bool& success);
};
gOptionsMenu OPTIONS_MENU;

class gIngameMenu {
private:
    //Start menu texture
    LTexture PauseTexture;
    LTexture OptionsTexture;
    LTexture SoundTexture;

    //Button
    LButton Pause;
    LButton Home;
    LButton Sound;

    //Clips
    SDL_Rect PauseSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect OptionsSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect SoundSpriteClips[BUTTON_SPRITE_TOTAL];
public:
    //status
    bool isPauseOn;
    bool isSoundOn;
    bool isOptionsOn;

    gIngameMenu();
    void handleEvent(SDL_Event& e);
    void show();
    void loadMedia(bool& success);
};
gIngameMenu INGAME_MENU;

class gLoseMenu { 
private:
    SDL_Color textColor = { 255, 255, 255, 255 };
    LTexture loseMenuBox;
    LTexture reStartTexture;
    LTexture fisrtStar[ONOFF_STAR_TOTAL];
    LTexture secondStar[ONOFF_STAR_TOTAL];
    LTexture thirdStar[ONOFF_STAR_TOTAL];

    LTexture tempTexture;
    LTexture yourScoreTexture;
    stringstream timeText;
    LButton reStart;
    SDL_Rect reStartSpriteClips[BUTTON_SPRITE_TOTAL];

public:
    gLoseMenu() {};
    void handleEvent(SDL_Event& e);
    void show();
    void loadMedia(bool& success);
};
gLoseMenu LOSE_MENU;

class gExitMenu {
private:
    //Start menu texture
    LTexture ExitMenuBox;
    LTexture ButtonTexture;

    //Button
    LButton Yes;
    LButton No;

    //Clips
    SDL_Rect YesSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect NoSpriteClips[BUTTON_SPRITE_TOTAL];
public:
    gExitMenu() {};
    void handleEvent(SDL_Event& e);
    void show();
    void loadMedia(bool& success);
};
gExitMenu EXIT_MENU;

bool checkCollision(vector<SDL_Rect>& a, vector<SDL_Rect>& b);
void increasingDifficultyLevels(int& speedRender, LScore& score);

class gBackground {
private:
    LTexture Layer1;
    LTexture Layer2;
    LTexture Layer3;
    LTexture Layer4;
    LTexture Ground;
public:
    gBackground(){}
    void render(int speedRender);
    void loadMedia(bool& success);
};
gBackground BACKGROUND;

static int count_run = 0;
static int count_jump = 0;
static int count_fall = 0;
static int count_fastlanding = 0;
static int count_idle = 0;

const int GROUND = SCREEN_HEIGHT - 70;
const int JUMP_MAX = 250;
const double GRAVITY = 120;

//Obstacles
LTexture mShroom_big;
LTexture mShroom_medium;
LTexture mShroom_small;

LTexture mPlantViolet_big;
LTexture mPlantViolet_medium;
LTexture mPlantViolet_small;

LTexture mPlantRed_small;
LTexture mPlantRed_medium;
LTexture mPlantRed_big;

LTexture mMaleBee;
LTexture mFemaleBee;

//Coin Gems
LTexture mMonedaD;
LTexture mMonedaP;
LTexture mMonedaR;

LTexture mCoinAma;
LTexture mCoinAzu;
LTexture mCoinRoj;
LTexture mCoinGri;
LTexture mCoinStrip;

class ObstacleAndItem {
private:
    vector <SDL_Rect> mColliders;
    vector <SDL_Rect> mColliders_offset;
    LTexture* character;

    vector <SDL_Rect> mSpritesClips;
    SDL_Rect* mCurrentClips;
public:
    int mPosX;
    int mPosY;
    int frame;
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
enum INDEX_LIST_OBSTACLES {
    SHROOM_BIG,
    SHROOM_MEDIUM,
    SHROOM_SMALL,
    PLANTRED_BIG,
    PLANTRED_MEDIUM,
    PLANTRED_SMALL,
    PLANTVIOLET_BIG,
    PLANTVIOLET_MEDIUM,
    PLANTVIOLET_SMALL,
    BEE_MALE,
    BEE_FEMALE,
    OBSTACLES_TOTAL
};
enum INDEX_LIST_GEMS {
    GEMS_MONEDA_D,
    GEMS_MONEDA_P,
    GEMS_MONEDA_R,
    GEMS_TOTAL
};
enum INDEX_LIST_COIN {
    COIN_AMA,
    COIN_AZU,
    COIN_ROJ,
    COIN_GRI,
    COIN_STRIP,
    COIN_TOTAL
};
class ObstacleAndItemProperties {
private:
    LTexture* character;
    vector<SDL_Rect> mColliders;

    vector <SDL_Rect> mSpritesClips;
public:
    vector<SDL_Rect> getColliders() {
        return mColliders;
    }
    void setSpritesClips(const vector<SDL_Rect>& spritesClips) {
        mSpritesClips = spritesClips;
    }
    vector<SDL_Rect> getSpritesClips() {
        return mSpritesClips;
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
vector <ObstacleAndItemProperties> randomListObstacles;
vector <ObstacleAndItemProperties> randomListCoin;
vector <ObstacleAndItemProperties> randomListGems;

int generateRandomNumber(const int min, const int max);
void RandomObstaclesAndItem(ObstacleAndItem& obstacle, vector <ObstacleAndItemProperties>& randomList);

class gCharacter {
private:
    double mPosX;
    double mPosY;
    
    int runPresentFrame;
    int jumpPresentFrame;
    int fallPresentFrame;
    int fastlandingPresentFrame;
    int idlePresentFrame;

    double mVelY;
    bool isFall;

    //timer
    LTimer timeJump;
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
gCharacter Otter;
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
            cout << "Khong tao duoc window! " << SDL_GetError();
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
    randomListObstacles.resize(11);
    randomListCoin.resize(5);
    randomListGems.resize(3);
    //Font
    gFont = TTF_OpenFont("font/Planes_ValMore.ttf", 50);
    if (gFont == NULL)
    {
        cout << "Failed to load Planes_ValMore font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
    gFontBigSize = TTF_OpenFont("font/Planes_ValMore.ttf", 120);
    if (gFont == NULL)
    {
        cout << "Failed to load Planes_ValMore font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }
    gFontMedSize = TTF_OpenFont("font/Planes_ValMore.ttf", 80);
    if (gFont == NULL)
    {
        cout << "Failed to load Planes_ValMore font! SDL_ttf Error: %s\n" << TTF_GetError();
        success = false;
    }

    //Otter
    Otter.loadMedia(success);

    //Sound
    gSound.loadMedia(success);

    //Score
    SCORE.loadMedia(success);

    //START MENU
    START_MENU.loadMedia(success);
    //PAUSE MENU
    PAUSE_MENU.loadMedia(success);
    //OPTIONS MENU
    OPTIONS_MENU.loadMedia(success);
    //Exit menu
    EXIT_MENU.loadMedia(success);
    //Ingame menu
    INGAME_MENU.loadMedia(success);
    //Lose menu
    LOSE_MENU.loadMedia(success);
    //Background
    BACKGROUND.loadMedia(success);

    //Shroom enemy mShroom
    if (!mShroom_big.loadFromFile("imgs/obstacle/shroom_big.png")) {
        cout << "Load shroom big enemy that bai!" << endl;
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
    if (!mShroom_medium.loadFromFile("imgs/obstacle/shroom_medium.png")) {
        cout << "Load shroom small enemy that bai!" << endl;
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
    if (!mShroom_small.loadFromFile("imgs/obstacle/shroom_small.png")) {
        cout << "Load shroom small enemy that bai!" << endl;
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
    if (!mPlantRed_big.loadFromFile("imgs/obstacle/plantred_big.png")) {
        cout << "Load plantred enemy that bai!" << endl;
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
    if (!mPlantRed_medium.loadFromFile("imgs/obstacle/plantred_medium.png")) {
        cout << "Load plantred enemy that bai!" << endl;
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
    if (!mPlantRed_small.loadFromFile("imgs/obstacle/plantred_small.png")) {
        cout << "Load plantred enemy that bai!" << endl;
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

    if (!mPlantViolet_big.loadFromFile("imgs/obstacle/plantviolet_big.png")) {
        cout << "Load plantviolet enemy that bai!" << endl;
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
    if (!mPlantViolet_medium.loadFromFile("imgs/obstacle/plantviolet_medium.png")) {
        cout << "Load plantviolet enemy that bai!" << endl;
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
    if (!mPlantViolet_small.loadFromFile("imgs/obstacle/plantviolet_small.png")) {
        cout << "Load plantviolet enemy that bai!" << endl;
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
    if (!mMaleBee.loadFromFile("imgs/obstacle/malebee.png")) {
        cout << "Load mGogleEyesBee enemy that bai!" << endl;
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
    if (!mFemaleBee.loadFromFile("imgs/obstacle/femalebee.png")) {
        cout << "Load mGogleEyesBee enemy that bai!" << endl;
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
    if (!mCoinAma.loadFromFile("imgs/coingems/CoinAma.png")) {
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
    if (!mCoinAzu.loadFromFile("imgs/coingems/CoinAzu.png")) {
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
    if (!mCoinRoj.loadFromFile("imgs/coingems/CoinRoj.png")) {
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
    if (!mCoinGri.loadFromFile("imgs/coingems/CoinGri.png")) {
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
    if (!mCoinStrip.loadFromFile("imgs/coingems/CoinStrip.png")) {
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
    if (!mMonedaD.loadFromFile("imgs/coingems/MonedaD.png")) {
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
    if (!mMonedaP.loadFromFile("imgs/coingems/MonedaP.png")) {
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
    if (!mMonedaR.loadFromFile("imgs/coingems/MonedaR.png")) {
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

    return success;
}

void close() {

    fstream soundSavedInfo("sound/VOLUMEINFO.txt", ios::out);
    soundSavedInfo << gSound.getVolumeMusic() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT << " "
        << gSound.getVolumeChunk() * (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36) * (double(1) / 128) + THRESHOLD_CONTROLER_LEFT ;
    soundSavedInfo.close();

    fstream savedScoreInfo("score.txt", ios::out);
    savedScoreInfo<< SCORE.getHighScore();
    savedScoreInfo.close();

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
//DECLARE
vector <int> randomDistance = { 100, 300, 500, 200, 400};
vector <int> randomCoinTimeList = { 3, 4, 5};
vector <int> randomGemsTimeList = {10, 12, 14};
ObstacleAndItem firstPlant, secondPlant;
vector <ObstacleAndItem> loopEnemy;

//Coin
ObstacleAndItem coin;
LTimer coinRandomTimer;
int coinRandomTime;
bool isCoinShow;
vector <ObstacleAndItem> loopCoin;

//Gems
ObstacleAndItem gems;
LTimer gemsRandomTimer;
int gemsRandomTime;
bool isGemsShow;
vector <ObstacleAndItem> loopGems;

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
    coinRandomTime = randomCoinTimeList[generateRandomNumber(0, randomCoinTimeList.size() - 1)];
    //set loopCoin
    loopCoin = { coin };
    coinRandomTimer.start();
    isCoinShow = false;

    RandomObstaclesAndItem(gems, randomListGems);
    coin.setX(1280);
    coin.setY(550);
    gemsRandomTime = randomGemsTimeList[generateRandomNumber(0, randomGemsTimeList.size() - 1)];
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
        countDown.init();
        break;
    case TO_DO_EXIT_MENU:
        menuStatus = MENU_STATUS_EXIT;
        break;
    }

}
void mainGameProcess() {
    //Background render
    BACKGROUND.render(speedRender);
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
                SCORE.updateHighScore();
                if (gameStatus == GAME_STATUS_PLAYING) gSound.PlayLoseSound();
                gameStatus = GAME_STATUS_LOSE;
                menuStatus = MENU_STATUS_LOSE;
            }
        }
        else {
            int index = generateRandomNumber(0, randomDistance.size() - 1);
            int distance = randomDistance[index];
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
                    RandomObstaclesAndItem(loopCoin[0], randomListCoin);
                    loopCoin[0].setX(1280);
                    loopCoin[0].setY(420);
                    isCoinShow = false;
                    coinRandomTime = randomCoinTimeList[generateRandomNumber(0, randomCoinTimeList.size() - 1)];
                    coinRandomTimer.start();
                }
            }
            else
            {
                RandomObstaclesAndItem(loopCoin[0], randomListCoin);
                loopCoin[0].setX(1280);
                loopCoin[0].setY(420);
                isCoinShow = false;
                coinRandomTime = randomCoinTimeList[generateRandomNumber(0, randomCoinTimeList.size() - 1)];
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
                    RandomObstaclesAndItem(loopGems[0], randomListGems);
                    loopGems[0].setX(1280);
                    loopGems[0].setY(550);
                    isGemsShow = false;
                    gemsRandomTime = randomGemsTimeList[generateRandomNumber(0, randomGemsTimeList.size() - 1)];
                    gemsRandomTimer.start();
                }
            }
            else
            {
                RandomObstaclesAndItem(loopGems[0], randomListGems);
                loopGems[0].setX(1280);
                loopGems[0].setY(550);
                isGemsShow = false;
                gemsRandomTime = randomGemsTimeList[generateRandomNumber(0, randomGemsTimeList.size() - 1)];
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
    SCORE.render();
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
                        START_MENU.handleEvent(e);
                        if (e.type == SDL_KEYDOWN)
                            if ((e.key.keysym.sym == SDLK_SPACE) && e.key.repeat == 0) handle(TO_DO_START);
                        break;
                    case MENU_STATUS_PLAYING:
                        Otter.handleEvent(e);
                        INGAME_MENU.handleEvent(e);
                        if (e.type == SDL_KEYDOWN)
                            if (e.key.keysym.sym == SDLK_ESCAPE && e.key.repeat == 0) handle(TO_DO_PAUSE);
                        break;
                    case MENU_STATUS_PAUSED:
                        PAUSE_MENU.handleEvent(e);
                        INGAME_MENU.handleEvent(e);
                        if (e.type == SDL_KEYDOWN)
                            if (e.key.keysym.sym == SDLK_ESCAPE && e.key.repeat == 0) handle(TO_DO_COUNTDOWN);
                        break;
                    case MENU_STATUS_OPTIONS:
                        OPTIONS_MENU.handleEvent(e);
                        INGAME_MENU.handleEvent(e);
                        break;
                    case MENU_STATUS_LOSE:
                        LOSE_MENU.handleEvent(e);
                        INGAME_MENU.handleEvent(e);
                        if (e.type == SDL_KEYDOWN)
                            if (e.key.keysym.sym == SDLK_ESCAPE && e.key.repeat == 0) handle(TO_DO_BACK_HOME);
                        break;
                    case MENU_STATUS_EXIT:
                        EXIT_MENU.handleEvent(e);
                        break;
                    }
                }
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);
                
                mainGameProcess();

                if (gameStatus == GAME_STATUS_PLAYING || gameStatus == GAME_STATUS_PAUSED || gameStatus == GAME_STATUS_LOSE) {
                    INGAME_MENU.show();
                }
                switch (menuStatus)
                {
                    case MENU_STATUS_START:
                        START_MENU.show();
                        break;
                    case MENU_STATUS_PAUSED:
                        PAUSE_MENU.show();
                        break;
                    case MENU_STATUS_OPTIONS:
                        OPTIONS_MENU.show();
                        break;
                    case MENU_STATUS_LOSE:
                        LOSE_MENU.show();
                        break;
                    case MENU_STATUS_EXIT:
                        EXIT_MENU.show();
                        break;
                    case MENU_STATUS_COUNTDOWN:
                        if (!countDown.isEndCountDown()) countDown.show(); else handle(TO_DO_RESUME);
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
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* gFont)
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

//Background
void gBackground::render(int speedRender) {
    //Render layer 1
    static int scrollingOffset_layer1 = 0;
    if (speedRender == 0) scrollingOffset_layer1 -= 0; else
        scrollingOffset_layer1 -= 1;
    if (scrollingOffset_layer1 <= -Layer1.getWidth())
    {
        scrollingOffset_layer1 = 0;
    }
    Layer1.render(scrollingOffset_layer1, 0);
    Layer1.render(scrollingOffset_layer1 + Layer1.getWidth(), 0);

    //Render layer 2
    static int scrollingOffset_layer2 = 0;
    if (speedRender == 0) scrollingOffset_layer2 -= 0; else
        scrollingOffset_layer2 -= 2;
    if (scrollingOffset_layer2 <= -Layer2.getWidth())
    {
        scrollingOffset_layer2 = 0;
    }
    Layer2.render(scrollingOffset_layer2, 0);
    Layer2.render(scrollingOffset_layer2 + Layer2.getWidth(), 0);

    //Render layer 3
    static int scrollingOffset_layer3 = 0;
    if (speedRender == 0) scrollingOffset_layer3 -= 0; else
        scrollingOffset_layer3 -= 4;
    if (scrollingOffset_layer3 <= -Layer3.getWidth())
    {
        scrollingOffset_layer3 = 0;
    }
    Layer3.render(scrollingOffset_layer3, 0);
    Layer3.render(scrollingOffset_layer3 + Layer3.getWidth(), 0);

    //Render layer 4
    static int scrollingOffset_layer4 = 0;
    scrollingOffset_layer4 -= speedRender;
    if (scrollingOffset_layer4 <= -Layer4.getWidth())
    {
        scrollingOffset_layer4 = 0;
    }
    Layer4.render(scrollingOffset_layer4, SCREEN_HEIGHT - 205);
    Layer4.render(scrollingOffset_layer4 + Layer4.getWidth(), SCREEN_HEIGHT - 205);


    //Render Ground
    static int scrollingOffset_ground = 0;
    scrollingOffset_ground -= speedRender;
    if (scrollingOffset_ground <= -Ground.getWidth())
    {
        scrollingOffset_ground = 0;
    }
    Ground.render(scrollingOffset_ground, SCREEN_HEIGHT - Ground.getHeight());
    Ground.render(scrollingOffset_ground + Ground.getWidth(), SCREEN_HEIGHT - Ground.getHeight());

}
void gBackground::loadMedia(bool& success) {
    if (!Layer1.loadFromFile("imgs/background/background_layer_1.png")) {
        cout << "Load BG Layer1 that bai! " << endl;
        success = false;
    }
    if (!Layer2.loadFromFile("imgs/background/background_layer_2.png")) {
        cout << "Load BG Layer2 that bai! " << endl;
        success = false;
    }
    if (!Layer3.loadFromFile("imgs/background/background_layer_3.png")) {
        cout << "Load BG Layer3 that bai! " << endl;
        success = false;
    }
    if (!Layer4.loadFromFile("imgs/background/background_layer_4.png")) {
        cout << "Load BG Layer4 that bai! " << endl;
        success = false;
    }
    if (!Ground.loadFromFile("imgs/background/ground.png")) {
        cout << "Load Ground that bai! " << endl;
        success = false;
    }
}

//Start menu
void gStartMenu::handleEvent(SDL_Event& e) {
    Start.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_START);
    Options.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_OPTIONS);
    Exit.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_EXIT_MENU);
}
void gStartMenu::show() {
    static int alpha = 0;
    //Blur
    if (alpha > 255) alpha = 255;
    MenuBackground.setAlpha(alpha);
    alpha += 40;

    //Render menu background
    //MenuBackground.render(0, 0);

    //Render start menu box
    StartMenuBox.render((SCREEN_WIDTH - StartMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - StartMenuBox.getHeight()) / 2);

    //Render button
    Start.render(ButtonTexture, StartSpriteClips);
    Options.render(ButtonTexture, OptionsSpriteClips);
    Exit.render(ButtonTexture, ExitSpriteClips);
}
void gStartMenu::loadMedia(bool& success) {
    if (!MenuBackground.loadFromFile("imgs/menu/menu_bg.jpg")) {
        cout << "Load menu_bg that bai! " << endl;
        success = false;
    }
    else
    {
        MenuBackground.setBlendMode(SDL_BLENDMODE_BLEND);
    }

    if (!ButtonTexture.loadFromFile("imgs/menu/start/start_menu_button_sheet.png")) {
        cout << "Load gStartMenu that bai! " << endl;
        success = false;
    }
    else
    {
        StartSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,320,113 };
        StartSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,0,320,113 };
        StartSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,0,320,113 };
        StartSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,0,320,113 };
        Start.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 210);

        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,122,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,122,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,122,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,122,320,113 };
        Options.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 330);

        ExitSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,244,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,244,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,244,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,244,320,113 };
        Exit.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 450);

    }
    if (!StartMenuBox.loadFromFile("imgs/menu/start/startmenubox.png")) {
        cout << "Load gStartMenu background that bai! " << endl;
        success = false;
    }
}

//Pause menu
void gPauseMenu::handleEvent(SDL_Event& e) {
    ReStart.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_RESTART);
    Resume.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_COUNTDOWN);
    Options.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_OPTIONS);
    Exit.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_EXIT_MENU);
}
void gPauseMenu::show() {
    static int alpha = 0;
    //Blur
    if (alpha > 255) alpha = 255;
    MenuBackground.setAlpha(alpha);
    alpha += 5;

    //Render menu background
    //MenuBackground.render(0, 0);

    //Render start menu box
    PauseMenuBox.render((SCREEN_WIDTH - PauseMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - PauseMenuBox.getHeight()) / 2);

    //Render button
    ReStart.render(ButtonTexture, ReStartSpriteClips);
    Resume.render(ButtonTexture, ResumeSpriteClips);
    Options.render(ButtonTexture, OptionsSpriteClips);
    Exit.render(ButtonTexture, ExitSpriteClips);
}
void gPauseMenu::loadMedia(bool& success) {
    if (!MenuBackground.loadFromFile("imgs/menu/menu_bg.jpg")) {
        cout << "Load menu_bg that bai! " << endl;
        success = false;
    }
    else
    {
        MenuBackground.setBlendMode(SDL_BLENDMODE_BLEND);
    }
    if (!ButtonTexture.loadFromFile("imgs/menu/pause/pause_menu_button_sheet.png")) {
        cout << "Load gPauseMenu that bai! " << endl;
        success = false;
    }
    else
    {
        ResumeSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,320,113 };
        ResumeSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,0,320,113 };
        ResumeSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,0,320,113 };
        ResumeSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,0,320,113 };
        Resume.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 160);

        ReStartSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,122,320,113 };
        ReStartSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,122,320,113 };
        ReStartSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,122,320,113 };
        ReStartSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,122,320,113 };
        ReStart.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 280);

        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,241,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,241,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,241,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,241,320,113 };
        Options.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 400);

        ExitSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,361,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,361,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,361,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,361,320,113 };
        Exit.setPosition((SCREEN_WIDTH - BUTTON_WIDTH) / 2, 515);
    }
    if (!PauseMenuBox.loadFromFile("imgs/menu/pause/pausemenubox.png")) {
        cout << "Load gPauseMenu background that bai! " << endl;
        success = false;
    }
}

//Options menu
void gOptionsMenu::handleEvent(SDL_Event& e) {
    Back.handleEvent(&e, 144, 154, TO_DO_BACK, true);
    SFX.handleEventController(&e, SFXTexture.getWidth(), SFXTexture.getHeight(), SFXstatus, TO_DO_SET_VOL_SFX);
    BGM.handleEventController(&e, BGMTexture.getWidth(), BGMTexture.getHeight(), BGMstatus, TO_DO_SET_VOL_BGM);
}
void gOptionsMenu::show() {
    static int alpha = 0;
    //Blur
    if (alpha > 255) alpha = 255;
    MenuBackground.setAlpha(alpha);
    alpha += 5;

    //Render menu background
    //MenuBackground.render(0, 0);

    //Render start menu box
    OptionsMenuBox.render((SCREEN_WIDTH - OptionsMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - OptionsMenuBox.getHeight()) / 2);

    //Render button
    Back.render(BackTexture, BackSpriteClips);
    SFX.renderController(SFXTexture);
    BGM.renderController(BGMTexture);
}
void gOptionsMenu::setPositionX_BGM(const int& x) {
    BGM.setPositionX(x);
}
void gOptionsMenu::setPositionX_SFX(const int& x) {
    SFX.setPositionX(x);
}
void gOptionsMenu::loadMedia(bool& success) {
    double savedVolumeChunk, savedVolumeMusic;
    fstream soundSavedInfo("sound/VOLUMEINFO.txt", ios::in);
    soundSavedInfo >> savedVolumeMusic >> savedVolumeChunk;
    soundSavedInfo.close();
    if (!OptionsMenuBox.loadFromFile("imgs/menu/options/optionsmenubox.png")) {
        success = false;
        cout << "Load OptionsMenuBox failed! " << endl;
    }
    if (!BGMTexture.loadFromFile("imgs/menu/options/button_bgm.png")) {
        success = false;
        cout << "Load BGMTexture failed! " << endl;
    }
    else
    {
        BGM.setPosition(savedVolumeMusic, 385);
        gSound.setVolumeMusic((double(savedVolumeMusic - THRESHOLD_CONTROLER_LEFT) / (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36)) * 128);
    }
    if (!SFXTexture.loadFromFile("imgs/menu/options/button_sfx.png")) {
        success = false;
        cout << "Load SFXTexture failed! " << endl;
    }
    else {
        SFX.setPosition(savedVolumeChunk, 285);
        gSound.setVolumeChunk((double(savedVolumeChunk - THRESHOLD_CONTROLER_LEFT) / (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36)) * 128);
    }
    if (!MenuBackground.loadFromFile("imgs/menu/menu_bg.jpg")) {
        success = false;
        cout << "Load menu_bg failed! " << endl;
    }
    else
    {
        MenuBackground.setBlendMode(SDL_BLENDMODE_BLEND);
    }
    if (!BackTexture.loadFromFile("imgs/menu/options/back_button_sheet.png")) {
        success = false;
        cout << "Load menu_bg failed! " << endl;
    }
    else
    {
        BackSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,144,152 };
        BackSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 0,156,144,154 };
        BackSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 0,312,144,154 };
        BackSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 0,469,144,154 };
        Back.setPosition(320, 470);
    }
}

//Ingame menu
gIngameMenu::gIngameMenu() {
    isPauseOn = true;
    isSoundOn = true;
    isOptionsOn = false;
};
void gIngameMenu::handleEvent(SDL_Event& e) {
    Pause.handleEventSwitch(&e, 70, 70, isPauseOn, (gameStatus == GAME_STATUS_LOSE ? TO_DO_BACK_HOME : (isPauseOn == true ? TO_DO_PAUSE : TO_DO_RESUME)));
    Home.handleEventSwitch(&e, 70, 70, isOptionsOn, TO_DO_BACK_HOME);
    Sound.handleEventSwitch(&e, 70, 70, isSoundOn, (isSoundOn == true ? TO_DO_TURN_OFF_SOUND : TO_DO_TURN_ON_SOUND));
}
void gIngameMenu::show() {
    //Render button
    Pause.renderSwitch(PauseTexture, PauseSpriteClips);
    Home.renderSwitch(OptionsTexture, OptionsSpriteClips);
    Sound.renderSwitch(SoundTexture, SoundSpriteClips);
}
void gIngameMenu::loadMedia(bool& success) {
    if (!PauseTexture.loadFromFile("imgs/menu/ingame_menu/pause_onclick.png")) {
        success = false;
        cout << "Load switch failed! " << endl;
    }
    else
    {
        PauseSpriteClips[SWITCH_SPRITE_MOUSE_ON] = { 0,0,65,65 };
        PauseSpriteClips[SWITCH_SPRITE_MOUSE_OFF] = { 65,0,65,65 };
        Pause.setPosition(10, 10);
    }
    if (!OptionsTexture.loadFromFile("imgs/menu/ingame_menu/home_onclick.png")) {
        success = false;
        cout << "Load switch failed! " << endl;
    }
    else
    {
        OptionsSpriteClips[SWITCH_SPRITE_MOUSE_ON] = { 0,0,65,65 };
        OptionsSpriteClips[SWITCH_SPRITE_MOUSE_OFF] = { 65,0,65,65 };
        Home.setPosition(95, 10);
    }
    if (!SoundTexture.loadFromFile("imgs/menu/ingame_menu/sound_onclick.png")) {
        success = false;
        cout << "Load switch failed! " << endl;
    }
    else
    {
        SoundSpriteClips[SWITCH_SPRITE_MOUSE_ON] = { 0,0,65,65 };
        SoundSpriteClips[SWITCH_SPRITE_MOUSE_OFF] = { 65,0,65,65 };
        Sound.setPosition(180, 10);
    }
}

//Lose menu
void gLoseMenu::handleEvent(SDL_Event& e) {
    reStart.handleEvent(&e, 144, 154, TO_DO_RESTART);
    if (e.type == SDL_KEYDOWN)
        if (e.key.keysym.sym == SDLK_SPACE && e.key.repeat == 0) {
            handle(TO_DO_RESTART);
        }
}
void gLoseMenu::show() {

    if (SCORE.isHighScore()) tempTexture.loadFromRenderedText("HIGH SCORE", textColor, gFont);
    else {
        tempTexture.loadFromRenderedText("YOUR SCORE", textColor, gFont);
    }
    timeText.str("");

    timeText << SCORE.getScore();
    yourScoreTexture.loadFromRenderedText(timeText.str().c_str(), textColor, gFontMedSize);
    loseMenuBox.render((SCREEN_WIDTH - loseMenuBox.getWidth()) / 2, 60);
    if (SCORE.getScore() < 100) {
        fisrtStar[OFF_STAR].render(512, 210);
        secondStar[OFF_STAR].render(602, 190);
        thirdStar[OFF_STAR].render(702, 210);
    }
    if (SCORE.getScore() >= 100 && SCORE.getScore() < 200) {
        fisrtStar[ON_STAR].render(512, 210);
        secondStar[OFF_STAR].render(602, 190);
        thirdStar[OFF_STAR].render(702, 210);
    }
    if (SCORE.getScore() >= 200 && SCORE.getScore() < 400) {
        fisrtStar[ON_STAR].render(512, 210);
        secondStar[ON_STAR].render(602, 190);
        thirdStar[OFF_STAR].render(702, 210);
    }
    if (SCORE.getScore() >= 400) {
        fisrtStar[ON_STAR].render(512, 210);
        secondStar[ON_STAR].render(602, 190);
        thirdStar[ON_STAR].render(702, 210);
    }

    tempTexture.render((SCREEN_WIDTH - tempTexture.getWidth()) / 2, 290);
    yourScoreTexture.render((SCREEN_WIDTH - yourScoreTexture.getWidth()) / 2, 350);
    reStart.render(reStartTexture, reStartSpriteClips);
}
void gLoseMenu::loadMedia(bool& success) {
    if (!loseMenuBox.loadFromFile("imgs/menu/lose/gameoverbox.png")) {
        cout << "failed to load loseTexture" << endl;
        success = false;
    }
    if (!fisrtStar[OFF_STAR].loadFromFile("imgs/menu/lose/1_OFF.png")) {
        success = false;
        cout << "load firstStar failed" << endl;
    }
    if (!fisrtStar[ON_STAR].loadFromFile("imgs/menu/lose/1_ON.png")) {
        success = false;
        cout << "load firstStar failed" << endl;
    }
    if (!secondStar[OFF_STAR].loadFromFile("imgs/menu/lose/2_OFF.png")) {
        success = false;
        cout << "load secondStar failed" << endl;
    }
    if (!secondStar[ON_STAR].loadFromFile("imgs/menu/lose/2_ON.png")) {
        success = false;
        cout << "load secondStar failed" << endl;
    }
    if (!thirdStar[OFF_STAR].loadFromFile("imgs/menu/lose/3_OFF.png")) {
        success = false;
        cout << "load thirdStar failed" << endl;
    }
    if (!thirdStar[ON_STAR].loadFromFile("imgs/menu/lose/3_ON.png")) {
        success = false;
        cout << "load thirdStar failed" << endl;
    }
    if (!reStartTexture.loadFromFile("imgs/menu/lose/restart.png")) {
        success = false;
        cout << "load reStartTexture failed" << endl;
    }
    else
    {
        reStartSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,144,154 };
        reStartSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 0,154,144,154 };
        reStartSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 0,308,144,154 };
        reStartSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 0,462,144,154 };
        reStart.setPosition(568, 450);

    }
}

//Exit menu
void gExitMenu::handleEvent(SDL_Event& e) {
    Yes.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_EXIT);
    No.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_BACK, true);
}
void gExitMenu::show() {

    //Render start menu box
    ExitMenuBox.render((SCREEN_WIDTH - ExitMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - ExitMenuBox.getHeight()) / 2);

    //Render button
    Yes.render(ButtonTexture, YesSpriteClips);
    No.render(ButtonTexture, NoSpriteClips);
}
void gExitMenu::loadMedia(bool& success) {
    if (!ExitMenuBox.loadFromFile("imgs/menu/exit/exitmenubox.png")) {
        success = false;
        cout << "load ExitMenuBox failed" << endl;
    }
    if (!ButtonTexture.loadFromFile("imgs/menu/exit/Exit_menu_button_sheet.png")) {
        success = false;
        cout << "load buttonTexture Exitmenu failed" << endl;
    }
    else
    {
        NoSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,144,154 };
        NoSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 0,154,144,154 };
        NoSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 0,308,144,154 };
        NoSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 0,462,144,154 };
        No.setPosition(410, 450);

        YesSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 156,0,144,154 };
        YesSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 156,154,144,154 };
        YesSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 156,308,144,154 };
        YesSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 156,462,144,154 };
        Yes.setPosition(725, 450);
    }
}

//Button
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
void LButton::handleEvent(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT,const TO_DO &todo, bool isBack)
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
                    if(!isBack) menuPre = menuStatus;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                    if (isBack) {
                        menuStatus = menuPre;
                    }
                    else
                    {
                        handle(todo);
                    }
                    break;
            }

        }
    }
}
void LButton::handleEventController(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT, ControllerStatus& status, const TO_DO& todo)
{
    if (e->type == SDL_MOUSEBUTTONDOWN) {
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

        if (inside) {
            status.isClick = true;
            status.x_len = x - mPosition.x;
            status.y_len = y - mPosition.y;
        };
        //Mouse is outside button
    }
    if (e->type == SDL_MOUSEMOTION && status.isClick) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        mPosition.x = x - status.x_len;
        if (mPosition.x < THRESHOLD_CONTROLER_LEFT) mPosition.x = THRESHOLD_CONTROLER_LEFT;
        if (mPosition.x + BUTTON_WIDTH > THRESHOLD_CONTROLER_RIGHT) mPosition.x = THRESHOLD_CONTROLER_RIGHT - BUTTON_WIDTH;
        handle(todo, (double(mPosition.x - THRESHOLD_CONTROLER_LEFT) / (THRESHOLD_CONTROLER_RIGHT - 36 - THRESHOLD_CONTROLER_LEFT))*MIX_MAX_VOLUME);

    }
    if (e->type == SDL_MOUSEBUTTONUP) {
        status.isClick = false;
    }
}
void LButton::handleEventSwitch(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT, bool& isOn, const TO_DO& todo) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
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
        if (inside) {
            handle(todo);
        }

    }
    switch (isOn)
    {
    case true:
        mSwitchCurrentSprite = SWITCH_SPRITE_MOUSE_ON;
        break;
    case false:
        mSwitchCurrentSprite = SWITCH_SPRITE_MOUSE_OFF;
        break;
    }

}
void LButton::renderController(LTexture& gButtonSpriteSheetTexture)
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y);
}
void LButton::renderSwitch(LTexture& gButtonSpriteSheetTexture, SDL_Rect SpriteClips[]) {
    gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &SpriteClips[mSwitchCurrentSprite]);
}
void LButton::render(LTexture& gButtonSpriteSheetTexture, SDL_Rect SpriteClips[])
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &SpriteClips[mCurrentSprite]);
}
void LButton::setPositionX(const int& x) {
    mPosition.x = x;
}
//Timer
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

//Game sound
bool LSound::isPlayingMusic() {
    return Mix_PlayingMusic();
}
bool LSound::isPausedMusic() {
    return Mix_PausedMusic();
}
void LSound::StopMusic() {
    Mix_HaltMusic();
}
void LSound::PlayMusic() {
    if (!isPlayingMusic())
        Mix_PlayMusic(gBgm, -1);
    Mix_VolumeMusic(volumeMusic);
}
void LSound::PauseMusic() {
    if (isPlayingMusic())
        Mix_PauseMusic();
}
void LSound::ResumeMusic() {
    if (isPausedMusic())
        Mix_ResumeMusic();
}
void LSound::PlayJumpSound() {
    Mix_PlayChannel(-1, gJumpSound, 0);
    Mix_VolumeChunk(gJumpSound, volumeChunk);
}
void LSound::PlayLoseSound() {
    Mix_PlayChannel(-1, gLoseSound, 0);
    Mix_VolumeChunk(gLoseSound, volumeChunk);
}
void LSound::PlayGainSound() {
    Mix_PlayChannel(-1, gGainSound, 0);
    Mix_VolumeChunk(gGainSound, volumeChunk);
}
void LSound::PlayPassSound() {
    Mix_PlayChannel(-1, gPassSound, 0);
    Mix_VolumeChunk(gPassSound, volumeChunk);
}
void LSound::setVolumeMusic(const int& v) {
    volumeMusic = v;
    Mix_VolumeMusic(volumeMusic);
}
void LSound::setVolumeChunk(const int& v) {
    volumeChunk = v;
    Mix_VolumeChunk(gJumpSound, volumeChunk);
    Mix_VolumeChunk(gLoseSound, volumeChunk);
    Mix_VolumeChunk(gPassSound, volumeChunk);
    Mix_VolumeChunk(gGainSound, volumeChunk);
}
void LSound::loadMedia(bool& success) {
    gBgm = Mix_LoadMUS("sound/bgm.wav");
    if (!gBgm) {
        cout << "Failed to load bgm" << Mix_GetError() << endl;
        success = false;
    }
    gJumpSound = Mix_LoadWAV("sound/jumpsound.wav");
    if (!gJumpSound) {
        cout << "Failed to load jumpsound1" << Mix_GetError() << endl;
        success = false;
    }
    gPassSound = Mix_LoadWAV("sound/passsound.wav");
    if (!gPassSound) {
        cout << "Failed to load gPassSound" << Mix_GetError() << endl;
        success = false;
    }
    gLoseSound = Mix_LoadWAV("sound/losesound.wav");
    if (!gLoseSound) {
        cout << "Failed to load losesound" << Mix_GetError() << endl;
        success = false;
    }
    gGainSound = Mix_LoadWAV("sound/gainsound.wav");
    if (!gGainSound) {
        cout << "Failed to load gGainSound" << Mix_GetError() << endl;
        success = false;
    }
}
void LSound::turnOffSound() {
    savedVolumeMusic = volumeMusic;
    savedVolumeChunk = volumeChunk;
    volumeChunk = volumeMusic = 0;
    setVolumeChunk(volumeChunk);
    setVolumeMusic(volumeMusic);
}
void LSound::turnOnSound() {
    volumeMusic = savedVolumeMusic;
    volumeChunk = savedVolumeChunk;
    setVolumeChunk(volumeChunk);
    setVolumeMusic(volumeMusic);
}
int LSound::getVolumeMusic() {
    return volumeMusic;
}
int LSound::getVolumeChunk() {
    return volumeChunk;
}
//Score
void LScore::loadMedia(bool& success) {
    Uint32 savedScore;
    fstream savedScoreInfo("score.txt", ios::in);
    if (!savedScoreInfo.is_open()) {
        success = false;
    }
    else
    {
        savedScoreInfo >> savedScore;
        setScoreFromSaved(savedScore);
        savedScoreInfo.close();
    }
}
void LScore::addScore(const Uint32& sc) {
    scoreAdded += sc;
}
void LScore::stop() {
    if (gTimer.isStarted()) gTimer.stop();
}
void LScore::updateHighScore() {
    timeText.str("");
    if (score > savedScore) {
        isHighSc = true;
        savedScore = score;
        timeText << "HI: " << savedScore;
        if (!gHighScoreTexture.loadFromRenderedText(timeText.str().c_str(), textColor, gFont)) {
            cout << "Unable to render savedtime texture!" << endl;
        }
    }
    else if (score < savedScore) isHighSc = false;
}
void LScore::setScoreFromSaved(const Uint32& sc) {
    savedScore = sc;
    timeText << "HI: " << savedScore;
    if (!gHighScoreTexture.loadFromRenderedText(timeText.str().c_str(), textColor, gFont)) {
        cout << "Unable to render savedtime texture!" << endl;
    }
}
void LScore::process() {
    timeText.str("");
    shiftScore();
    timeText << score - startTime;
}
void LScore::pause() {
    if (!gTimer.isPaused()) gTimer.pause();
}
void LScore::start() {
    scoreAdded = 0;
    if (!gTimer.isStarted()) gTimer.start();
}
void LScore::reStart() {
    scoreAdded = 0;
    if (gTimer.isStarted()) gTimer.start();
}
void LScore::render() {
    if (!gCurrentScoreTexture.loadFromRenderedText(timeText.str().c_str(), textColor, gFont)) {
        cout << "Unable to render time texture!" << endl;
    }
    else {
        gCurrentScoreTexture.render(SCREEN_WIDTH - gCurrentScoreTexture.getWidth() - 50, 10);
        gHighScoreTexture.render(SCREEN_WIDTH - gHighScoreTexture.getWidth() - 200, 10);
    }
} 
void LScore::shiftScore() {
    score = scoreAdded+gTimer.getTicks() / 200;
}
void LScore::resume() {
    if (gTimer.isPaused()) gTimer.unpause();
}
bool LScore::isStarted() {
    return gTimer.isStarted();
}
bool LScore::isHighScore() {
    return isHighSc;
}
Uint32 LScore::getHighScore() {
    return savedScore;
}
int LScore::getScore() {
    shiftScore();
    return score;
}

//CountDown
LCountDown::LCountDown() {
    init();
}
void LCountDown::init() {
    count = 3;
    isEnd = false;
    timer.start();
    timeText.str("");
}
void LCountDown::render() {
    textCountDown.render((SCREEN_WIDTH - textCountDown.getWidth()) / 2, (SCREEN_HEIGHT - textCountDown.getHeight()) / 2);
}
void LCountDown::show() {
    timeText.str("");
    if (!isEnd) {
        if (timer.getTicks() / 400 == 1) {
            if (count == -1) {
                isEnd = true;
                timer.stop();
                timeText.str("");
                textCountDown.free();
            }
            else
            {
                if (count == 0) {
                    timeText << "GO!";
                    timer.start();
                    --count;
                }
                else {
                    timeText << count;
                    timer.start();
                    --count;
                }
                if (!textCountDown.loadFromRenderedText(timeText.str(), textColor, gFontBigSize)) {
                    cout << "Failed to load text" << endl;
                }
            }
        }
    }
    //Render textures
    render();
}
bool LCountDown::isEndCountDown() {
    return isEnd;
}

//Obstacle and item
gCharacter::gCharacter() {
    runPresentFrame = 0;
    jumpPresentFrame = 0;
    fallPresentFrame = 0;
    fastlandingPresentFrame = 0;

    mPosX = 150;
    mPosY = GROUND - RUN_SHEET_HEIGHT;
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
        idle.render(mPosX, mPosY, currentClip);
    }
    else {
        if (status == RUN) {
            runPresentFrame = count_run / 7;
            if (gameStatus == GAME_STATUS_PLAYING) count_run++;
            SDL_Rect* currentClip = &spriteClip_run[runPresentFrame];
            const int RUN_FRAME_SIZE = spriteClip_run.size() - 1;
            if (count_run / 7 > RUN_FRAME_SIZE) {
                count_run = 0;
            }
            run.render(mPosX, mPosY, currentClip);
        }

        if (status == JUMP) {
            jumpPresentFrame = count_jump / 20;
            if (gameStatus == GAME_STATUS_PLAYING) count_jump++;
            SDL_Rect* currentClip = &spriteClip_jump[jumpPresentFrame];
            const int JUMP_FRAME_SIZE = spriteClip_jump.size() - 1;
            if (count_jump / 20 > JUMP_FRAME_SIZE) {
                count_jump = 0;
            }
            jump.render(mPosX, mPosY, currentClip);
        }

        if (status == FALL) {
            fallPresentFrame = count_fall / 7;
            if (gameStatus == GAME_STATUS_PLAYING) count_fall++;
            SDL_Rect* currentClip = &spriteClip_fall[fallPresentFrame];
            const int FALL_FRAME_SIZE = spriteClip_fall.size() - 1;
            if (count_fall / 7 > FALL_FRAME_SIZE) {
                count_fall = 0;
            }
            fall.render(mPosX, mPosY, currentClip);
        }

        if (status == FASTLANDING) {
            fastlandingPresentFrame = count_fastlanding / 8;
            if (gameStatus == GAME_STATUS_PLAYING) count_fastlanding++;
            SDL_Rect* currentClip = &spriteClip_fall[fastlandingPresentFrame];
            const int FASTLANDING_FRAME_SIZE = spriteClip_fall.size() - 1;
            if (count_fastlanding / 8 > FASTLANDING_FRAME_SIZE) {
                count_fastlanding = 0;
            }
            fall.render(mPosX, mPosY, currentClip);
        }
        move();
    }
}
void gCharacter::handleEvent(SDL_Event& e) {

    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_SPACE) {
            if (status == RUN) {
                status = JUMP;
                mPosY = GROUND - JUMP_SHEET_HEIGHT;
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
void gCharacter::move()
{
    //Processing

    shiftColliders();
    //Fast landing
    if (status == FASTLANDING) {
        mPosY += 45;
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
        mVelY = 245;
        double deltaTime = timeJump.getTicks() / 1000.f;
        mPosY += -(mVelY * (deltaTime)-0.5 * 40 * (deltaTime * deltaTime));
        shiftColliders();
    }
    else if (mPosY <= JUMP_MAX && isFall == false) {
        count_fall = 0;
        status = FALL;
        //timeJump.stop();
        timeJump.start();
        isFall = true;
    }
    if (status == FALL) {
        if (mPosY <= GROUND - FALL_SHEET_HEIGHT) {
            mVelY = 85;
            double deltaTime = timeJump.getTicks() / 1000.f;
            mPosY += mVelY * (deltaTime)+0.5 * 40 * (deltaTime * deltaTime);
            shiftColliders();
        }
        else {
            mPosY = GROUND - RUN_SHEET_HEIGHT;
            shiftColliders();
            status = RUN;
            timeJump.stop();
            isFall = false;
        }
    }
}
void gCharacter::loadMedia(bool& success) {
    if (!run.loadFromFile("imgs/characters/Otter/Run.png")) {
        cout << "Load Otter run that bai!" << endl;
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
    if (!jump.loadFromFile("imgs/characters/Otter/Jump.png")) {
        cout << "Load Otter jump that bai!" << endl;
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
    if (!fall.loadFromFile("imgs/characters/Otter/land.png")) {
        cout << "Load Otter land that bai!" << endl;
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
    if (!idle.loadFromFile("imgs/characters/Otter/idle.png")) {
        cout << "Load Otter idle that bai!" << endl;
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
int gCharacter::getStatus() {
    return status;
}

//Obstacle and item properties
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
    character->render(mPosX, mPosY);
    move();
}
void ObstacleAndItem::show() {
    if (++frame / 7 > mSpritesClips.size() - 1) {
        frame = 0;
    }
    mCurrentClips = &mSpritesClips[frame / 7];
    character->render(mPosX, mPosY, mCurrentClips);
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