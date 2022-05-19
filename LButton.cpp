#include "LButton.h"
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
void LButton::handleEvent(SDL_Event* e, const int& BUTTON_WIDTH, const int& BUTTON_HEIGHT, MENU_STATUS& menuStatus, MENU_STATUS& menuPre, const TO_DO& todo, bool isBack)
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
                if (!isBack) menuPre = menuStatus;
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
        handle(todo, (double(mPosition.x - THRESHOLD_CONTROLER_LEFT) / (THRESHOLD_CONTROLER_RIGHT - 36 - THRESHOLD_CONTROLER_LEFT)) * MIX_MAX_VOLUME);

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
void LButton::renderController(LTexture& gButtonSpriteSheetTexture, SDL_Renderer* gRenderer)
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render(gRenderer, mPosition.x, mPosition.y);
}
void LButton::renderSwitch(LTexture& gButtonSpriteSheetTexture, SDL_Rect SpriteClips[], SDL_Renderer* gRenderer) {
    gButtonSpriteSheetTexture.render(gRenderer, mPosition.x, mPosition.y, &SpriteClips[mSwitchCurrentSprite]);
}
void LButton::render(LTexture& gButtonSpriteSheetTexture, SDL_Rect SpriteClips[], SDL_Renderer* gRenderer)
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render(gRenderer, mPosition.x, mPosition.y, &SpriteClips[mCurrentSprite]);
}
void LButton::setPositionX(const int& x) {
    mPosition.x = x;
}
