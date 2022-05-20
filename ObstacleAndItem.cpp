#include "ObstacleAndItem.h"
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
void ObstacleAndItem::move(const int& speedRender) {
    mPosX -= speedRender;
    shiftColliders(mColliders);
}
void ObstacleAndItem::render(SDL_Renderer* gRenderer, const int& speedRender) {
    character->render(gRenderer, mPosX, mPosY);
    move(speedRender);
}
void ObstacleAndItem::show(SDL_Renderer* gRenderer, const int& speedRender) {
    if (++frame / 7 > mSpritesClips.size() - 1) {
        frame = 0;
    }
    mCurrentClips = &mSpritesClips[frame / 7];
    character->render(gRenderer, mPosX, mPosY, mCurrentClips);
    move(speedRender);
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
