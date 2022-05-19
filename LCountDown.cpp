#include "LCountDown.h"
//CountDown
LCountDown::LCountDown() {
    init();
}
void LCountDown::init() {
    count = 3;
    isEnd = false;
    timer.start();
    timeText.str("");
}
void LCountDown::render(SDL_Renderer* gRenderer) {
    textCountDown.render(gRenderer, (SCREEN_WIDTH - textCountDown.getWidth()) / 2, (SCREEN_HEIGHT - textCountDown.getHeight()) / 2);
}
void LCountDown::show(SDL_Renderer* gRenderer, TTF_Font *Font) {
    timeText.str("");
    if (!isEnd) {
        if (timer.getTicks() / 400 == 1) {
            if (count == -1) {
                isEnd = true;
                timer.stop();
                timeText.str("");
                textCountDown.free();
            }
            else
            {
                if (count == 0) {
                    timeText << "GO!";
                    timer.start();
                    --count;
                }
                else {
                    timeText << count;
                    timer.start();
                    --count;
                }
                if (!textCountDown.loadFromRenderedText(gRenderer, timeText.str(), textColor, Font)) {
                    cout << "Failed to load text" << endl;
                }
            }
        }
    }
    //Render textures
    render(gRenderer);
}
bool LCountDown::isEndCountDown() {
    return isEnd;
}