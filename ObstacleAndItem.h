#pragma once
#include "CommonFunction.h"
#include "ObstacleAndItemProperties.h"
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
    void move(const int &speedRender);
    void render(SDL_Renderer* gRenderer, const int& speedRender);
    void show(SDL_Renderer* gRenderer, const int& speedRender);
    bool isOver();
    void shiftColliders(vector <SDL_Rect>& Colliders);
};

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

void RandomObstaclesAndItem(ObstacleAndItem& obstacle, vector <ObstacleAndItemProperties>& randomList);