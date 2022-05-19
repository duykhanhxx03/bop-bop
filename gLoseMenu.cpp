#include "gLoseMenu.h"
//Lose menu
void gLoseMenu::handleEvent(SDL_Event& e, MENU_STATUS& menuStatus, MENU_STATUS& menuPre) {
    reStart.handleEvent(&e, 144, 154, menuStatus, menuPre, TO_DO_RESTART);
    if (e.type == SDL_KEYDOWN)
        if (e.key.keysym.sym == SDLK_SPACE && e.key.repeat == 0) {
            handle(TO_DO_RESTART);
        }
}
void gLoseMenu::show(SDL_Renderer* gRenderer, TTF_Font* Font, TTF_Font* FontMedSize, LScore &SCORE) {

    if (SCORE.isHighScore()) tempTexture.loadFromRenderedText(gRenderer, "HIGH SCORE", textColor, Font);
    else {
        tempTexture.loadFromRenderedText(gRenderer, "YOUR SCORE", textColor, Font);
    }
    timeText.str("");

    timeText << SCORE.getScore();
    yourScoreTexture.loadFromRenderedText(gRenderer, timeText.str().c_str(), textColor, FontMedSize);
    loseMenuBox.render(gRenderer, (SCREEN_WIDTH - loseMenuBox.getWidth()) / 2, 60);
    if (SCORE.getScore() < 100) {
        fisrtStar[OFF_STAR].render(gRenderer, 512, 210);
        secondStar[OFF_STAR].render(gRenderer, 602, 190);
        thirdStar[OFF_STAR].render(gRenderer, 702, 210);
    }
    if (SCORE.getScore() >= 100 && SCORE.getScore() < 200) {
        fisrtStar[ON_STAR].render(gRenderer, 512, 210);
        secondStar[OFF_STAR].render(gRenderer, 602, 190);
        thirdStar[OFF_STAR].render(gRenderer, 702, 210);
    }
    if (SCORE.getScore() >= 200 && SCORE.getScore() < 400) {
        fisrtStar[ON_STAR].render(gRenderer, 512, 210);
        secondStar[ON_STAR].render(gRenderer, 602, 190);
        thirdStar[OFF_STAR].render(gRenderer, 702, 210);
    }
    if (SCORE.getScore() >= 400) {
        fisrtStar[ON_STAR].render(gRenderer, 512, 210);
        secondStar[ON_STAR].render(gRenderer, 602, 190);
        thirdStar[ON_STAR].render(gRenderer, 702, 210);
    }

    tempTexture.render(gRenderer, (SCREEN_WIDTH - tempTexture.getWidth()) / 2, 290);
    yourScoreTexture.render(gRenderer, (SCREEN_WIDTH - yourScoreTexture.getWidth()) / 2, 350);
    reStart.render(reStartTexture, reStartSpriteClips, gRenderer);
}
void gLoseMenu::loadMedia(bool& success, SDL_Renderer* gRenderer) {
    if (!loseMenuBox.loadFromFile("imgs/menu/lose/gameoverbox.png", gRenderer)) {
        cout << "failed to load loseTexture" << endl;
        success = false;
    }
    if (!fisrtStar[OFF_STAR].loadFromFile("imgs/menu/lose/1_OFF.png", gRenderer)) {
        success = false;
        cout << "load firstStar failed" << endl;
    }
    if (!fisrtStar[ON_STAR].loadFromFile("imgs/menu/lose/1_ON.png", gRenderer)) {
        success = false;
        cout << "load firstStar failed" << endl;
    }
    if (!secondStar[OFF_STAR].loadFromFile("imgs/menu/lose/2_OFF.png", gRenderer)) {
        success = false;
        cout << "load secondStar failed" << endl;
    }
    if (!secondStar[ON_STAR].loadFromFile("imgs/menu/lose/2_ON.png", gRenderer)) {
        success = false;
        cout << "load secondStar failed" << endl;
    }
    if (!thirdStar[OFF_STAR].loadFromFile("imgs/menu/lose/3_OFF.png", gRenderer)) {
        success = false;
        cout << "load thirdStar failed" << endl;
    }
    if (!thirdStar[ON_STAR].loadFromFile("imgs/menu/lose/3_ON.png", gRenderer)) {
        success = false;
        cout << "load thirdStar failed" << endl;
    }
    if (!reStartTexture.loadFromFile("imgs/menu/lose/restart.png", gRenderer)) {
        success = false;
        cout << "load reStartTexture failed" << endl;
    }
    else
    {
        reStartSpriteClips[BUTTON_SPRITE_MOUSE_OUT] = { 0,0,144,154 };
        reStartSpriteClips[BUTTON_SPRITE_MOUSE_OVER_MOTION] = { 0,154,144,154 };
        reStartSpriteClips[BUTTON_SPRITE_MOUSE_DOWN] = { 0,308,144,154 };
        reStartSpriteClips[BUTTON_SPRITE_MOUSE_UP] = { 0,462,144,154 };
        reStart.setPosition(568, 450);

    }
}
