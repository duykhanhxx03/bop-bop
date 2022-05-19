#include "gIntroduceMenu.h"
//Introduce menu
void gIntroduceMenu::loadMedia(bool& success, SDL_Renderer* gRenderer) {
    if (!IntroduceMenuBox.loadFromFile("imgs/menu/introduce/Introducemenubox.png", gRenderer)) {
        success = false;
        cout << "failed to load IntroduceMenuBox" << endl;
    }
    if (!BackTexture.loadFromFile("imgs/menu/introduce/back_button_sheet.png", gRenderer)) {
        success = false;
        cout << "failed to load IntroduceMenuBox" << endl;
    }
    else
    {
        BackSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,144,152 };
        BackSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 0,156,144,154 };
        BackSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 0,312,144,154 };
        BackSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 0,469,144,154 };
        Back.setPosition(200, 470);
    }
}
void gIntroduceMenu::handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre) {
    Back.handleEvent(&e, 144, 154, menuStatus, menuPre, TO_DO_BACK, true);
}
void gIntroduceMenu::show(SDL_Renderer* gRenderer) {
    IntroduceMenuBox.render(gRenderer, (SCREEN_WIDTH - IntroduceMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - IntroduceMenuBox.getHeight()) / 2);
    Back.render(BackTexture, BackSpriteClips, gRenderer);
}