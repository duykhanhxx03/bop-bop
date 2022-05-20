#pragma once
#include "CommonFunction.h"
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
//Button
class LButton
{
public:
    //Initializes internal variables
    LButton();

    //Sets top left position
    void setPosition(int x, int y);

    //Handles mouse event
    void handleEventController(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT, ControllerStatus& status, const TO_DO& todo);
    void handleEventSwitch(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT, bool& isOn, const TO_DO& todo);

    void handleEvent(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT, MENU_STATUS& menuStatus, MENU_STATUS& menuPre, const TO_DO& todo = TO_DO_START, bool isBack = false);
    //Shows button sprite
    void render(LTexture& gButtonSpriteSheetTexture, SDL_Rect SpriteClips[], SDL_Renderer* gRenderer);
    void renderSwitch(LTexture& gButtonSpriteSheetTexture, SDL_Rect SpriteClips[], SDL_Renderer* gRenderer);
    void renderController(LTexture& gButtonSpriteSheetTexture, SDL_Renderer* gRenderer);
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