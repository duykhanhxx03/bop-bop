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
    fstream soundSavedInfo("sound/volumeinfo.txt", ios::in);
    soundSavedInfo >> savedVolumeMusic >> savedVolumeChunk;
    soundSavedInfo.close();

    //SDL_RWops* fileVolumeMusic = SDL_RWFromFile("sound/volumemusic.bin", "r+b");
    ////File does not exist
    //if (fileVolumeMusic == NULL)
    //{
    //    cout << "Warning: Unable to open file! SDL Error: %s\n" << SDL_GetError();
    //    //Create file for writing
    //    fileVolumeMusic = SDL_RWFromFile("sound/volumemusic.bin", "w+b");
    //    if (fileVolumeMusic != NULL)
    //    {
    //        cout << "New file created!\n";

    //        savedVolumeMusic = 814;
    //        SDL_RWwrite(fileVolumeMusic, &savedVolumeMusic, sizeof(double), 1);
    //        //Close file handler
    //        SDL_RWclose(fileVolumeMusic);
    //    }
    //    else
    //    {
    //        cout << "Error: Unable to create file! SDL Error: %s\n" << SDL_GetError();
    //        success = false;
    //    }
    //}
    ////File exists
    //else
    //{
    //    //Load data
    //    SDL_RWread(fileVolumeMusic, &savedVolumeMusic, sizeof(double), 1);
    //    //Close file handler
    //    SDL_RWclose(fileVolumeMusic);
    //}

    //SDL_RWops* fileVolumeChunk = SDL_RWFromFile("sound/volumechunk.bin", "r+b");
    ////File does not exist
    //if (fileVolumeChunk == NULL)
    //{
    //    cout << "Warning: Unable to open file! SDL Error: %s\n" << SDL_GetError();
    //    //Create file for writing
    //    fileVolumeChunk = SDL_RWFromFile("sound/volumechunk.bin", "w+b");
    //    if (fileVolumeChunk != NULL)
    //    {
    //        cout << "New file created!\n";

    //        savedVolumeChunk = 814;
    //        SDL_RWwrite(fileVolumeChunk, &savedVolumeChunk, sizeof(double), 1);
    //        //Close file handler
    //        SDL_RWclose(fileVolumeChunk);
    //    }
    //    else
    //    {
    //        cout << "Error: Unable to create file! SDL Error: %s\n" << SDL_GetError();
    //        success = false;
    //    }
    //}
    ////File exists
    //else
    //{
    //    //Load data
    //    SDL_RWread(fileVolumeChunk, &savedVolumeChunk, sizeof(double), 1);
    //    //Close file handler
    //    SDL_RWclose(fileVolumeChunk);
    //}

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
