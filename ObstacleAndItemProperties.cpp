#include "ObstacleAndItemProperties.h"
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
void loadRandomList(bool& success, SDL_Renderer* gRenderer, vector <ObstacleAndItemProperties> &randomListObstacles, vector <ObstacleAndItemProperties> &randomListCoin, vector <ObstacleAndItemProperties> &randomListGems) {
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