#pragma once
#include "CommonFunction.h"
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
    void show(SDL_Renderer* gRenderer, GAME_STATUS& gameStatus);
    void handleEvent(SDL_Event& e, LSound& gSound);
    void move();
    void loadMedia(bool& success, SDL_Renderer* gRenderer);
    vector <SDL_Rect>& getColliders();
    int getStatus();
};
static gCharacter Otter;