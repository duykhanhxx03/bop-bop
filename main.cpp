// BopBop.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;
bool quit = false;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
int speedRender = 0;
// 10 12 15 18 20
int speedRenderSaved = 0;
int saved_speedRender = 10;
enum GAME_STATUS {
    GAME_STATUS_IDLE,
    GAME_STATUS_PLAYING,
    GAME_STATUS_PAUSED,
    GAME_STATUS_LOSE
};
//10 12 15 16
enum MENU_STATUS {
    MENU_STATUS_START,
    MENU_STATUS_PLAYING,
    MENU_STATUS_PAUSED,
    MENU_STATUS_OPTIONS,
    MENU_STATUS_LOSE,
    MENU_STATUS_EXIT
};
MENU_STATUS menuStatus = MENU_STATUS_START;
MENU_STATUS menuPre = menuStatus;
GAME_STATUS gameStatus = GAME_STATUS_IDLE;
void mainGameInit();
enum TO_DO {
    TO_DO_START,
    TO_DO_BACK,
    TO_DO_RESUME,
    TO_DO_RESTART,
    TO_DO_PAUSE,
    TO_DO_EXIT,
    TO_DO_OPTIONS,
    TO_DO_SET_VOL_BGM,
    TO_DO_SET_VOL_SFX,
    TO_DO_TURN_OFF_SOUND,
    TO_DO_TURN_ON_SOUND
};
void handle(const TO_DO& todo,const double &v=MIX_MAX_VOLUME/2);
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

class LSound {
private:
    Mix_Music* gBgm;
    Mix_Chunk* gJumpSound;
    Mix_Chunk* gLoseSound;
    //MIX_MAX_VOLUME=128
    int volumeMusic = 64;
    int volumeChunk = 64;
    
    int savedVolumeMusic = volumeMusic;
    int savedVolumeChunk = volumeChunk;
public:
    bool setBgm(const string& path);
    bool setJumpSound(const string& path);
    bool setLoseSound(const string& path);
    bool isPlayingMusic();
    bool isPausedMusic();
    void StopMusic();
    void PlayMusic();
    void PauseMusic();
    void ResumeMusic();
    void PlayJumpSound();
    void PlayLoseSound();
    int getVolumeMusic() {
        return volumeMusic;
    }
    int getVolumeChunk() {
        return volumeChunk;
    }
    void setVolumeMusic(const int& v);
    void setVolumeChunk(const int& v);
    void turnOffSound() {
        savedVolumeMusic = volumeMusic;
        savedVolumeChunk = volumeChunk;
        volumeChunk = volumeChunk = 0;
        setVolumeChunk(volumeChunk);
        setVolumeMusic(volumeMusic);
    }
    void turnOnSound() {
        volumeMusic = savedVolumeMusic;
        volumeChunk = savedVolumeChunk;
        setVolumeChunk(volumeChunk);
        setVolumeMusic(volumeMusic);
    }
};
LSound gSound;

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

    void render(int x, int y, SDL_Rect* clip = NULL, int mScale = 1);

    int getWidth();
    int getHeight();
private:
    SDL_Texture* mTexture;

    int mWidth;
    int mHeight;
};

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
    void handleEvent(SDL_Event& e) {
        Start.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT,TO_DO_START);
        Options.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_OPTIONS);
        Exit.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_EXIT);
    }
    void show() {
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
        ReStart.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_RESTART);
        Resume.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_RESUME);
        Options.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_OPTIONS);
        Exit.handleEvent(&e, BUTTON_WIDTH, BUTTON_HEIGHT, TO_DO_EXIT);
    }
    void show() {
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

};
gPauseMenu PAUSE_MENU;

class gOptionsMenu {
private:
public:
    gOptionsMenu() {};
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

    void handleEvent(SDL_Event& e) {
        Back.handleEvent(&e, BackTexture.getWidth(), BackTexture.getHeight(), TO_DO_BACK, true);
        SFX.handleEventController(&e, SFXTexture.getWidth(), SFXTexture.getHeight(), SFXstatus, TO_DO_SET_VOL_SFX);
        BGM.handleEventController(&e, BGMTexture.getWidth(), BGMTexture.getHeight(), BGMstatus, TO_DO_SET_VOL_BGM);
    }
    void show() {
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

};
gOptionsMenu OPTIONS_MENU;

class gIngameMenu {
private:
public:
    gIngameMenu() {};
    //Start menu texture
    LTexture PauseTexture;
    LTexture OptionsTexture;
    LTexture SoundTexture;

    //status
    bool pauseIsOn = true;
    bool soundIsOn = true;
    bool optionsIsOn = false;

    //Button
    LButton Pause;
    LButton Options;
    LButton Sound;

    //Clips
    SDL_Rect PauseSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect OptionsSpriteClips[BUTTON_SPRITE_TOTAL];
    SDL_Rect SoundSpriteClips[BUTTON_SPRITE_TOTAL];
    void handleEvent(SDL_Event& e) {
        Pause.handleEventSwitch(&e, 70, 70, pauseIsOn, TO_DO_PAUSE);
        Options.handleEventSwitch(&e, 70, 70, optionsIsOn, TO_DO_OPTIONS);
        Sound.handleEventSwitch(&e, 70, 70, soundIsOn, (soundIsOn==true?TO_DO_TURN_OFF_SOUND: TO_DO_TURN_ON_SOUND));
    }
    void show() {
        //Render button
        Pause.renderSwitch(PauseTexture, PauseSpriteClips);
        Options.renderSwitch(OptionsTexture, OptionsSpriteClips);
        Sound.renderSwitch(SoundTexture, SoundSpriteClips);
    }

};
gIngameMenu INGAME_MENU;

class gLoseMenu {
public:
    LTexture LoseTexture;
    gLoseMenu() {};
    void handleEvent(SDL_Event& e) {
        if (e.type == SDL_KEYDOWN)
            if (e.key.keysym.sym == SDLK_SPACE && e.key.repeat == 0) {
                handle(TO_DO_RESTART);
            }
    }
    void show() {
        LoseTexture.render((SCREEN_WIDTH - LoseTexture.getWidth()) / 2, (SCREEN_HEIGHT - LoseTexture.getHeight()) / 2);
    }
};
gLoseMenu LOSE_MENU;

class Score {
private:
    SDL_Color textColor = { 255, 255, 255, 255 };
    //Text render
    LTexture gTextTexture;
    //Current time start time
    Uint32 startTime = 0;
    stringstream timeText;
    LTimer gTimer;

    Uint32 score=0;
public:
    void process();
    void pause();
    void start();
    void reStart();
    void render();
    void shiftScore();
    void resume();
    void stop() {
        if (gTimer.isStarted()) gTimer.stop();
    }
    int getScore();
    bool isStarted() {
        return gTimer.isStarted();
    }
};
Score score;
bool init();

bool loadMedia();

void clode();

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

void increasingDifficultyLevels(int& speedRender, Score& score);

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
        static double scrollingOffset_ground = 0;
        scrollingOffset_ground -= speedRender;
        if (scrollingOffset_ground <= -Ground.getWidth())
        {
            scrollingOffset_ground = 0;
        }
        Ground.render(scrollingOffset_ground, SCREEN_HEIGHT - Ground.getHeight());
        Ground.render(scrollingOffset_ground + Ground.getWidth(), SCREEN_HEIGHT - Ground.getHeight());

    }
};
gBackground bg;

static int count_run = 0;
static int count_jump = 0;
static int count_fall = 0;
static int count_fastlanding = 0;
static int count_idle = 0;

const int GROUND = SCREEN_HEIGHT - 70;
const int JUMP_MAX = 270;
const double GRAVITY = 120;

//Obstacles
LTexture mShroom;
LTexture mPlantRed;
LTexture mPlantViolet;

class Obstacle {
private:
    vector <SDL_Rect> mColliders;
    vector <SDL_Rect> mColliders_offset;
    LTexture* character;
public:

    int mPosX;
    int mPosY;
    Obstacle() {
        mPosX = 0;
        character = NULL;
        mPosY = 0;
    }
    Obstacle(LTexture& texture, const int& n, const vector<SDL_Rect>& Colliders) {
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
    void setX(const int& n) {
        mPosX = n;
    }
    int getWidth() const {
        return character->getWidth();
    }
    int getHeight() const {
        return character->getHeight();
    }
    int getX() const {
        return mPosX;
    }
    void setCharacter(LTexture& texture) {
        character = &texture;
        mPosY = SCREEN_HEIGHT - 80 - character->getHeight() + 5;
    }
    void Free() {
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
    bool isOver() {
        if (mPosX <= -character->getWidth()) {
            //Free();
            //x = SCREEN_WIDTH;
            return true;
        }
        return false;
    }
    void shiftColliders(vector <SDL_Rect>& Colliders)
    {
        //Go through the dot's collision boxes
        for (int set = 0; set < Colliders.size(); ++set)
        {
            //Center the collision box
            Colliders[set].x = mPosX + mColliders_offset[set].x;
            //Set the collision box at its row offset
            Colliders[set].y = mPosY + mColliders_offset[set].y;
            //Move the row offset down the height of the collision box
        }
    }
};
enum INDEX_LIST_OBSTACLES {
    SHROOM = 0,
    PLANTRED = 1,
    PLANTVIOLET = 2,
    OBSTACLES_TOTAL = 3
};
class ObstacleProperties {
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
vector <ObstacleProperties> randomListObstacles;
int generateRandomNumber(const int min, const int max);
void RandomObstacles(Obstacle& obstacle);

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
public:
    LTexture run;
    LTexture jump;
    LTexture fall;
    LTexture idle;

    gCharacter() {
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
    vector<SDL_Rect> spriteClip_run;
    vector<SDL_Rect> spriteClip_jump;
    vector<SDL_Rect> spriteClip_fall;
    vector<SDL_Rect> spriteClip_idle;

    //use only for character
    void shiftColliders() {
        shiftColliders(mCollidersFastLanding, mCollidersFastLanding_offset, numberOffastlandingClips);
        shiftColliders(mCollidersJump, mCollidersJump_offset, numberOfjumpClips);
        shiftColliders(mCollidersFall, mCollidersFall_offset, numberOffallClips);
        shiftColliders(mCollidersRun, mCollidersRun_offset, numberOfrunClips);
    }
    void shiftColliders(vector <SDL_Rect> Colliders[], vector <SDL_Rect> Colliders_offset[], const int& numberOfClips)
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
    void shiftColliders(vector <SDL_Rect>& Colliders)
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
    void show(SDL_Renderer* gRenderer) {
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
    void handleEvent(SDL_Event& e) {

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
            mPosY += -(mVelY * (deltaTime)-0.5 * 40 * (deltaTime * deltaTime));
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
            if (mPosY <= GROUND - FALL_SHEET_HEIGHT) {
                mVelY = 90;
                double deltaTime = timeJump.getTicks() / 1000.f;
                mPosY += mVelY * (deltaTime)+0.5 * 40 * (deltaTime * deltaTime);
                shiftColliders();
                //cout << "fall_2" << endl;
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
    vector <SDL_Rect>& getColliders()
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

    //Font
    gFont = TTF_OpenFont("font/Planes_ValMore.ttf", 48);
    if (gFont == NULL)
    {
        cout << "Failed to load lazy font! SDL_ttf Error: %s\n" << TTF_GetError();
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
    if (!gSound.setLoseSound("sound/losesound.wav")) {
        cout << "Failed to load losesound" << Mix_GetError() << endl;
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

    //OPTIONS MENU
    if (!OPTIONS_MENU.OptionsMenuBox.loadFromFile("imgs/menu/options/optionsmenubox.png")) {
        success = false;
        cout << "Load OptionsMenuBox failed! " << endl;
    }
    if (!OPTIONS_MENU.BGMTexture.loadFromFile("imgs/menu/options/button_bgm.png")) {
        success = false;
        cout << "Load BGMTexture failed! " << endl;
    }
    else
    {
        OPTIONS_MENU.BGM.setPosition(700, 385);
    }
    if (!OPTIONS_MENU.SFXTexture.loadFromFile("imgs/menu/options/button_sfx.png")) {
        success = false;
        cout << "Load SFXTexture failed! " << endl;
    }
    else {
        OPTIONS_MENU.SFX.setPosition(700, 285);
    }
    if (!OPTIONS_MENU.MenuBackground.loadFromFile("imgs/menu/menu_bg.jpg")) {
        success = false;
        cout << "Load menu_bg failed! " << endl;
    }
    else
    {
        OPTIONS_MENU.MenuBackground.setBlendMode(SDL_BLENDMODE_BLEND);
    }
    if (!OPTIONS_MENU.BackTexture.loadFromFile("imgs/menu/options/back_button_sheet.png")) {
        success = false;
        cout << "Load menu_bg failed! " << endl;
    }
    else
    {
        OPTIONS_MENU.BackSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,144,152 };
        OPTIONS_MENU.BackSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 0,156,144,154 };
        OPTIONS_MENU.BackSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 0,312,144,154 };
        OPTIONS_MENU.BackSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 0,469,144,154 };
        OPTIONS_MENU.Back.setPosition(320, 470);
    }
    //Ingame menu
    if (!INGAME_MENU.PauseTexture.loadFromFile("imgs/menu/ingame_menu/pause_onclick.png")) {
        success = false;
        cout << "Load switch failed! " << endl;
    }
    else
    {
        INGAME_MENU.PauseSpriteClips[SWITCH_SPRITE_MOUSE_ON] = { 0,0,65,65 };
        INGAME_MENU.PauseSpriteClips[SWITCH_SPRITE_MOUSE_OFF] = { 65,0,65,65 };
        INGAME_MENU.Pause.setPosition(10, 10);
    }
    if (!INGAME_MENU.OptionsTexture.loadFromFile("imgs/menu/ingame_menu/options_onclick.png")) {
        success = false;
        cout << "Load switch failed! " << endl;
    }
    else
    {
        INGAME_MENU.OptionsSpriteClips[SWITCH_SPRITE_MOUSE_ON] = { 0,0,65,65 };
        INGAME_MENU.OptionsSpriteClips[SWITCH_SPRITE_MOUSE_OFF] = { 65,0,65,65 };
        INGAME_MENU.Options.setPosition(95, 10);
    }
    if (!INGAME_MENU.SoundTexture.loadFromFile("imgs/menu/ingame_menu/sound_onclick.png")) {
        success = false;
        cout << "Load switch failed! " << endl;
    }
    else
    {
        INGAME_MENU.SoundSpriteClips[SWITCH_SPRITE_MOUSE_ON] = { 0,0,65,65 };
        INGAME_MENU.SoundSpriteClips[SWITCH_SPRITE_MOUSE_OFF] = { 65,0,65,65 };
        INGAME_MENU.Sound.setPosition(180, 10);
    }
    if (!LOSE_MENU.LoseTexture.loadFromFile("imgs/menu/lose/gameover.png")) {
        cout << "failed to load loseTexture" << endl;
        success = false;
    }
    
    //Background
    if (!bg.Layer1.loadFromFile("imgs/background/background_layer_1.png")) {
        cout << "Load BG Layer1 that bai! " << endl;
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
        ObstacleProperties buffer;
        buffer.setCharacter(mShroom);
        buffer.setColliders(Colliders);
        randomListObstacles.push_back(buffer);
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
        ObstacleProperties buffer;
        buffer.setCharacter(mPlantRed);
        buffer.setColliders(Colliders);
        randomListObstacles.push_back(buffer);
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
        ObstacleProperties buffer;
        buffer.setCharacter(mPlantViolet);
        buffer.setColliders(Colliders);
        randomListObstacles.push_back(buffer);
    }
    //Otter
    if (!Otter.run.loadFromFile("imgs/characters/Otter/Run.png")) {
        cout << "Load Otter run that bai!" << endl;
        success = false;
    }
    else
    {
        Otter.spriteClip_run = {
            //Otter
            {0,0,200,160},
            {200,0,200,160},
            {400,0,200,160}
        };
    }
    if (!Otter.jump.loadFromFile("imgs/characters/Otter/Jump.png")) {
        cout << "Load Otter jump that bai!" << endl;
        success = false;
    }
    else
    {
        Otter.spriteClip_jump = {
            //Otter
            {0,0,200,160}
            //{200,0,200,160},
            //{400,0,200,160}
        };
    }
    if (!Otter.fall.loadFromFile("imgs/characters/Otter/land.png")) {
        cout << "Load Otter land that bai!" << endl;
        success = false;
    }
    else
    {
        Otter.spriteClip_fall = {
            //Otter
            {0,0,200,160}
            //{200,0,200,160},
            //{400,0,200,160}
        };
    }
    if (!Otter.idle.loadFromFile("imgs/characters/Otter/idle.png")) {
        cout << "Load Otter idle that bai!" << endl;
        success = false;
    }
    else
    {
        Otter.spriteClip_idle = {
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
//DECLARE
vector <int> randomDistance = { 100, 300, 500, 200, 400};
Obstacle one, two;
vector <Obstacle> loopEnemy;
void mainGameInit() {
    //first obstacle
    RandomObstacles(one);
    one.setX(SCREEN_WIDTH);

    //second obstacle
    RandomObstacles(two);
    two.setX(SCREEN_WIDTH+ SCREEN_WIDTH);

    //set loopEnemy
    loopEnemy = { one, two };
    if (!score.isStarted()) {
        score.start();
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
        score.resume();
        break;
    case TO_DO_RESTART:
        score.stop();
        mainGameInit();
        menuStatus = MENU_STATUS_PLAYING;
        gameStatus = GAME_STATUS_PLAYING;
        break;
    case TO_DO_OPTIONS:
        menuStatus = MENU_STATUS_OPTIONS;
        break;
    case TO_DO_PAUSE:
        speedRenderSaved = speedRender;
        speedRender = 0;
        gameStatus = GAME_STATUS_PAUSED;
        menuStatus = MENU_STATUS_PAUSED;
        score.pause();
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
    }


}
void mainGameProcess() {
    //Background render
    bg.render(speedRender);

    vector<SDL_Rect> rect_run = Otter.getColliders();
    for (int i = 0; i < loopEnemy.size(); i++) {
        if (!loopEnemy[i].isOver()) {
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
            vector<SDL_Rect> obstaclesClips = loopEnemy[i].getColliders();
            loopEnemy[i].render(speedRender);
            if (checkCollision(obstaclesClips, rect_run)) {
                score.pause();
                speedRender = 0;
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
                RandomObstacles(loopEnemy[i]);
            }
        }
    }
    //render Otter
    Otter.show(gRenderer);
    increasingDifficultyLevels(speedRender, score);
    //INGAME MENU
    INGAME_MENU.show();
    
    //Score render
    score.process();
    score.render();
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
            SDL_Event e;
            gSound.PlayMusic();

            bool isPaused = false;
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
                            if (e.key.keysym.sym == SDLK_ESCAPE && e.key.repeat == 0) handle(TO_DO_RESUME);
                        break;
                    case MENU_STATUS_OPTIONS:
                        OPTIONS_MENU.handleEvent(e);
                        INGAME_MENU.handleEvent(e);
                        break;
                    case MENU_STATUS_LOSE:
                        LOSE_MENU.handleEvent(e);
                        INGAME_MENU.handleEvent(e);
                        break;
                    case MENU_STATUS_EXIT:

                        break;
                    }
                }
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                SDL_RenderClear(gRenderer);

                mainGameProcess();
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
            isOn = (isOn ? false : true);
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
bool LSound::setBgm(const string& path) {
    gBgm = Mix_LoadMUS(path.c_str());
    return gBgm != NULL;
}
bool LSound::setJumpSound(const string& path) {
    gJumpSound = Mix_LoadWAV(path.c_str());
    return gJumpSound != NULL;
}
bool LSound::setLoseSound(const string& path) {
    gLoseSound = Mix_LoadWAV(path.c_str());
    return gLoseSound != NULL;
}
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
    Mix_VolumeChunk(gJumpSound, volumeChunk);
}
void LSound::setVolumeMusic(const int& v) {
    volumeMusic = v;
    Mix_VolumeMusic(volumeMusic);
}
void LSound::setVolumeChunk(const int& v) {
    volumeChunk = v;
    Mix_VolumeChunk(gJumpSound, volumeChunk);
    Mix_VolumeChunk(gLoseSound, volumeChunk);
}

//Score
void Score::process() {
    timeText.str("");
    shiftScore();
    timeText << score - startTime;
}
void Score::pause() {
    if (!gTimer.isPaused()) gTimer.pause();
}
void Score::start() {
    if (!gTimer.isStarted()) gTimer.start();
}
void Score::reStart() {
    if (gTimer.isStarted()) gTimer.start();
}
void Score::render() {
    if (!gTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor)) {
        cout << "Unable to render time texture!" << endl;
    }
    else {
        gTextTexture.render(SCREEN_WIDTH - gTextTexture.getWidth() - 50, 10);
    }
}
int Score::getScore() {
    shiftScore();
    return score;
}
void Score::shiftScore() {
    score = gTimer.getTicks() / 400;
}
void Score::resume() {
    if (gTimer.isPaused()) gTimer.unpause();
}

void RandomObstacles(Obstacle& obstacle) {
    vector <int> randomSrc = { 0,1,2,2,1,0,0,1,2,2,1,0};
    int index = generateRandomNumber(0, randomSrc.size() - 1);
    obstacle.setCharacter(*randomListObstacles[randomSrc[index]].getCharacter());
    obstacle.setColliders(randomListObstacles[randomSrc[index]].getColliders());
}
int generateRandomNumber(const int min, const int max)
{
    srand(time(0));
    // TODO: Return a random integer number between min and max
    return rand() % (max - min + 1) + min;
}
void increasingDifficultyLevels(int& speedRender, Score& score) {
    switch (score.getScore())
    {
    case 100:
        speedRender = 12;
        break;
    case 200:
        speedRender = 15;
        break;
    case 300:
        speedRender = 16;
        break;
    case 400:
        speedRender = 20;
        break;
    }
}