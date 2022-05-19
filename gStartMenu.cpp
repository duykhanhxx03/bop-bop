#include "gStartMenu.h"
//Start menu
void gStartMenu::handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre) {
    Introduce.handleEvent(&e, 110, 116, menuStatus, menuPre, TO_DO_INTRODUCE);
    Start.handleEvent(&e, 320, 113, menuStatus, menuPre, TO_DO_START);
    Options.handleEvent(&e, 320, 113, menuStatus, menuPre, TO_DO_OPTIONS);
    Exit.handleEvent(&e, 320, 113, menuStatus, menuPre, TO_DO_EXIT_MENU);
}
void gStartMenu::show(SDL_Renderer* gRenderer, TTF_Font* Font) {
    GameTitleText.render(gRenderer, 200, 150);
    GameTitleText.render(gRenderer, 300, 300);
    //Render start menu box
    StartMenuBox.render(gRenderer, 820, (SCREEN_HEIGHT - StartMenuBox.getHeight()) / 2);
    //Render button
    Introduce.render(IntroduceButtonTexture, IntroduceSpriteClips, gRenderer);
    Start.render(ButtonTexture, StartSpriteClips, gRenderer);
    Options.render(ButtonTexture, OptionsSpriteClips, gRenderer);
    Exit.render(ButtonTexture, ExitSpriteClips, gRenderer);
}
void gStartMenu::loadMedia(bool& success, SDL_Renderer* gRenderer, TTF_Font* Font) {
    if (!ButtonTexture.loadFromFile("imgs/menu/start/start_menu_button_sheet.png", gRenderer)) {
        cout << "Load gStartMenu failed! " << endl;
        success = false;
    }
    else
    {
        StartSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,320,113 };
        StartSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,0,320,113 };
        StartSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,0,320,113 };
        StartSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,0,320,113 };
        Start.setPosition(870, 210);

        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,122,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,122,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,122,320,113 };
        OptionsSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,122,320,113 };
        Options.setPosition(870, 330);

        ExitSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,244,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 320,244,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 640,244,320,113 };
        ExitSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 960,244,320,113 };
        Exit.setPosition(870, 450);

    }
    if (!IntroduceButtonTexture.loadFromFile("imgs/menu/start/info_button_sheet.png", gRenderer)) {
        cout << "Load IntroduceButtonTexture failed! " << endl;
        success = false;
    }
    else
    {
        IntroduceSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,110,116 };
        IntroduceSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 0,116,110,116 };
        IntroduceSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 0,232,110,116 };
        IntroduceSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 0,348,110,116 };
        Introduce.setPosition(10, 10);
    }
    if (!StartMenuBox.loadFromFile("imgs/menu/start/startmenubox.png", gRenderer)) {
        cout << "Load gStartMenu background failed! " << endl;
        success = false;
    }
    GameTitleText.loadFromRenderedText(gRenderer, "BOP!", textColor, Font);
}