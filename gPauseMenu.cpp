#include "gPauseMenu.h"
//Pause menu
void gPauseMenu::handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre) {
    ReStart.handleEvent(&e, 320, 113, menuStatus, menuPre, TO_DO_RESTART);
    Resume.handleEvent(&e, 320, 113, menuStatus, menuPre, TO_DO_COUNTDOWN);
    Options.handleEvent(&e, 320, 113, menuStatus, menuPre, TO_DO_OPTIONS);
    Exit.handleEvent(&e, 320, 113, menuStatus, menuPre, TO_DO_EXIT_MENU);
}
void gPauseMenu::show(SDL_Renderer* gRenderer) {
    //Render start menu box
    PauseMenuBox.render(gRenderer, (SCREEN_WIDTH - PauseMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - PauseMenuBox.getHeight()) / 2);

    //Render button
    ReStart.render(ButtonTexture, ReStartSpriteClips, gRenderer);
    Resume.render(ButtonTexture, ResumeSpriteClips, gRenderer);
    Options.render(ButtonTexture, OptionsSpriteClips, gRenderer);
    Exit.render(ButtonTexture, ExitSpriteClips, gRenderer);
}
void gPauseMenu::loadMedia(bool& success, SDL_Renderer* gRenderer) {
    if (!ButtonTexture.loadFromFile("imgs/menu/pause/pause_menu_button_sheet.png", gRenderer)) {
        cout << "Load gPauseMenu failed! " << endl;
        success = false;
    }
    else
    {
        ResumeSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,320,113 };
        ResumeSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,0,320,113 };
        ResumeSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,0,320,113 };
        ResumeSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,0,320,113 };
        Resume.setPosition((SCREEN_WIDTH - 320) / 2, 160);

        ReStartSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,122,320,113 };
        ReStartSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,122,320,113 };
        ReStartSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,122,320,113 };
        ReStartSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,122,320,113 };
        ReStart.setPosition((SCREEN_WIDTH - 320) / 2, 280);

        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,241,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,241,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,241,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,241,320,113 };
        Options.setPosition((SCREEN_WIDTH - 320) / 2, 400);

        ExitSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,361,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,361,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,361,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,361,320,113 };
        Exit.setPosition((SCREEN_WIDTH - 320) / 2, 515);
    }
    if (!PauseMenuBox.loadFromFile("imgs/menu/pause/pausemenubox.png", gRenderer)) {
        cout << "Load gPauseMenu background failed! " << endl;
        success = false;
    }
}
