#include "gOptionsMenu.h"
//Options menu
void gOptionsMenu::handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre) {
    Back.handleEvent(&e, 144, 154, menuStatus, menuPre, TO_DO_BACK, true);
    SFX.handleEventController(&e, SFXTexture.getWidth(), SFXTexture.getHeight(), SFXstatus, TO_DO_SET_VOL_SFX);
    BGM.handleEventController(&e, BGMTexture.getWidth(), BGMTexture.getHeight(), BGMstatus, TO_DO_SET_VOL_BGM);
}
void gOptionsMenu::show(SDL_Renderer* gRenderer) {
    //Render start menu box
    OptionsMenuBox.render(gRenderer, (SCREEN_WIDTH - OptionsMenuBox.getWidth()) / 2, (SCREEN_HEIGHT - OptionsMenuBox.getHeight()) / 2);

    //Render button
    Back.render(BackTexture, BackSpriteClips, gRenderer);
    SFX.renderController(SFXTexture, gRenderer);
    BGM.renderController(BGMTexture, gRenderer);
}
void gOptionsMenu::setPositionX_BGM(const int& x) {
    BGM.setPositionX(x);
}
void gOptionsMenu::setPositionX_SFX(const int& x) {
    SFX.setPositionX(x);
}
void gOptionsMenu::loadMedia(bool& success,SDL_Renderer* gRenderer, LSound &gSound) {
    double savedVolumeChunk, savedVolumeMusic;
    fstream soundSavedInfo("sound/VOLUMEINFO.txt", ios::in);
    soundSavedInfo >> savedVolumeMusic >> savedVolumeChunk;
    soundSavedInfo.close();
    if (!OptionsMenuBox.loadFromFile("imgs/menu/options/optionsmenubox.png", gRenderer)) {
        success = false;
        cout << "Load OptionsMenuBox failed! " << endl;
    }
    if (!BGMTexture.loadFromFile("imgs/menu/options/button_bgm.png", gRenderer)) {
        success = false;
        cout << "Load BGMTexture failed! " << endl;
    }
    else
    {
        BGM.setPosition(savedVolumeMusic, 385);
        gSound.setVolumeMusic((double(savedVolumeMusic - THRESHOLD_CONTROLER_LEFT) / (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36)) * 128);
    }
    if (!SFXTexture.loadFromFile("imgs/menu/options/button_sfx.png", gRenderer)) {
        success = false;
        cout << "Load SFXTexture failed! " << endl;
    }
    else {
        SFX.setPosition(savedVolumeChunk, 285);
        gSound.setVolumeChunk((double(savedVolumeChunk - THRESHOLD_CONTROLER_LEFT) / (THRESHOLD_CONTROLER_RIGHT - THRESHOLD_CONTROLER_LEFT - 36)) * 128);
    }
    if (!BackTexture.loadFromFile("imgs/menu/options/back_button_sheet.png", gRenderer)) {
        success = false;
        cout << "Load menu_bg failed! " << endl;
    }
    else
    {
        BackSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,144,152 };
        BackSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 0,156,144,154 };
        BackSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 0,312,144,154 };
        BackSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 0,469,144,154 };
        Back.setPosition(320, 470);
    }
}
