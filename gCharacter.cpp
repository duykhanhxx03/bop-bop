#include "gCharacter.h"
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
void gCharacter::show(SDL_Renderer* gRenderer, GAME_STATUS &gameStatus) {
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
void gCharacter::handleEvent(SDL_Event& e, LSound &gSound) {

    if (e.type == SDL_KEYDOWN && e.key.repeat == 0 )
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
void gCharacter::loadMedia(bool& success, SDL_Renderer *gRenderer) {
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