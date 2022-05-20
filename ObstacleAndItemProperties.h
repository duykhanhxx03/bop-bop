#pragma once
#include "LTexture.h"
#include <vector>

static vector <int> randomListDistance = { 100, 300, 500, 200, 400 };
static vector <int> randomListCoinTime = { 4, 5, 6 };
static vector <int> randomListGemsTime = { 12, 14, 16 };

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
void loadRandomList(bool& success, SDL_Renderer* gRenderer, vector <ObstacleAndItemProperties>& randomListObstacles,
    vector <ObstacleAndItemProperties>& randomListCoin, vector <ObstacleAndItemProperties>& randomListGems);