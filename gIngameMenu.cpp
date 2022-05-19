#include "gIngameMenu.h"
//Ingame menu
gIngameMenu::gIngameMenu() {
    isPauseOn = true;
    isSoundOn = true;
    isOptionsOn = false;
};
void gIngameMenu::handleEvent(SDL_Event& e, GAME_STATUS &gameStatus) {
    Pause.handleEventSwitch(&e, 70, 70, isPauseOn, (gameStatus == GAME_STATUS_LOSE ? TO_DO_BACK_HOME : (isPauseOn == true ? TO_DO_PAUSE : TO_DO_RESUME)));
    Home.handleEventSwitch(&e, 70, 70, isOptionsOn, TO_DO_BACK_HOME);
    Sound.handleEventSwitch(&e, 70, 70, isSoundOn, (isSoundOn == true ? TO_DO_TURN_OFF_SOUND : TO_DO_TURN_ON_SOUND));
}
void gIngameMenu::show(SDL_Renderer* gRenderer) {
    //Render button
    Pause.renderSwitch(PauseTexture, PauseSpriteClips, gRenderer);
    Home.renderSwitch(OptionsTexture, OptionsSpriteClips, gRenderer);
    Sound.renderSwitch(SoundTexture, SoundSpriteClips, gRenderer);
}
void gIngameMenu::loadMedia(bool& success, SDL_Renderer* gRenderer) {
    if (!PauseTexture.loadFromFile("imgs/menu/ingame_menu/pause_onclick.png", gRenderer)) {
        success = false;
        cout << "Load switch failed! " << endl;
    }
    else
    {
        PauseSpriteClips[SWITCH_SPRITE_MOUSE_ON] = { 0,0,65,65 };
        PauseSpriteClips[SWITCH_SPRITE_MOUSE_OFF] = { 65,0,65,65 };
        Pause.setPosition(30, 10);
    }
    if (!OptionsTexture.loadFromFile("imgs/menu/ingame_menu/home_onclick.png", gRenderer)) {
        success = false;
        cout << "Load switch failed! " << endl;
    }
    else
    {
        OptionsSpriteClips[SWITCH_SPRITE_MOUSE_ON] = { 0,0,65,65 };
        OptionsSpriteClips[SWITCH_SPRITE_MOUSE_OFF] = { 65,0,65,65 };
        Home.setPosition(115, 10);
    }
    if (!SoundTexture.loadFromFile("imgs/menu/ingame_menu/sound_onclick.png", gRenderer)) {
        success = false;
        cout << "Load switch failed! " << endl;
    }
    else
    {
        SoundSpriteClips[SWITCH_SPRITE_MOUSE_ON] = { 0,0,65,65 };
        SoundSpriteClips[SWITCH_SPRITE_MOUSE_OFF] = { 65,0,65,65 };
        Sound.setPosition(200, 10);
    }
}