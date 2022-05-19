#include "gExitMenu.h"
//Exit menu
void gExitMenu::handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre) {
    Yes.handleEvent(&e, 320, 113, menuStatus, menuPre, TO_DO_EXIT);
    No.handleEvent(&e, 320, 113, menuStatus, menuPre, TO_DO_BACK, true);
}
void gExitMenu::show(SDL_Renderer* gRenderer) {

    //Render start menu box
    ExitMenuBox.render(gRenderer, (SCREEN_WIDTH - ExitMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - ExitMenuBox.getHeight()) / 2);

    //Render button
    Yes.render(ButtonTexture, YesSpriteClips, gRenderer);
    No.render(ButtonTexture, NoSpriteClips, gRenderer);
}
void gExitMenu::loadMedia(bool& success, SDL_Renderer* gRenderer) {
    if (!ExitMenuBox.loadFromFile("imgs/menu/exit/exitmenubox.png", gRenderer)) {
        success = false;
        cout << "load ExitMenuBox failed" << endl;
    }
    if (!ButtonTexture.loadFromFile("imgs/menu/exit/Exit_menu_button_sheet.png", gRenderer)) {
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
