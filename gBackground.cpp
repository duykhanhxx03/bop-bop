#include "gBackground.h"
//Background
void gBackground::render(SDL_Renderer* gRenderer,int speedRender) {
    //Render layer 1
    static int scrollingOffset_layer1 = 0;
    if (speedRender == 0) scrollingOffset_layer1 -= 0; else
        scrollingOffset_layer1 -= 1;
    if (scrollingOffset_layer1 <= -Layer1.getWidth())
    {
        scrollingOffset_layer1 = 0;
    }
    Layer1.render(gRenderer, scrollingOffset_layer1, 0);
    Layer1.render(gRenderer, scrollingOffset_layer1 + Layer1.getWidth(), 0);

    //Render layer 2
    static int scrollingOffset_layer2 = 0;
    if (speedRender == 0) scrollingOffset_layer2 -= 0; else
        scrollingOffset_layer2 -= 2;
    if (scrollingOffset_layer2 <= -Layer2.getWidth())
    {
        scrollingOffset_layer2 = 0;
    }
    Layer2.render(gRenderer, scrollingOffset_layer2, 0);
    Layer2.render(gRenderer, scrollingOffset_layer2 + Layer2.getWidth(), 0);

    //Render layer 3
    static int scrollingOffset_layer3 = 0;
    if (speedRender == 0) scrollingOffset_layer3 -= 0; else
        scrollingOffset_layer3 -= 4;
    if (scrollingOffset_layer3 <= -Layer3.getWidth())
    {
        scrollingOffset_layer3 = 0;
    }
    Layer3.render(gRenderer, scrollingOffset_layer3, 0);
    Layer3.render(gRenderer, scrollingOffset_layer3 + Layer3.getWidth(), 0);

    //Render layer 4
    static int scrollingOffset_layer4 = 0;
    scrollingOffset_layer4 -= speedRender;
    if (scrollingOffset_layer4 <= -Layer4.getWidth())
    {
        scrollingOffset_layer4 = 0;
    }
    Layer4.render(gRenderer, scrollingOffset_layer4, SCREEN_HEIGHT - 205);
    Layer4.render(gRenderer, scrollingOffset_layer4 + Layer4.getWidth(), SCREEN_HEIGHT - 205);


    //Render Ground
    static int scrollingOffset_ground = 0;
    scrollingOffset_ground -= speedRender;
    if (scrollingOffset_ground <= -Ground.getWidth())
    {
        scrollingOffset_ground = 0;
    }
    Ground.render(gRenderer, scrollingOffset_ground, SCREEN_HEIGHT - Ground.getHeight());
    Ground.render(gRenderer, scrollingOffset_ground + Ground.getWidth(), SCREEN_HEIGHT - Ground.getHeight());

}
void gBackground::loadMedia(bool& success,SDL_Renderer* gRenderer) {
    if (!Layer1.loadFromFile("imgs/background/background_layer_1.png", gRenderer)) {
        cout << "Load BG Layer1 failed! " << endl;
        success = false;
    }
    if (!Layer2.loadFromFile("imgs/background/background_layer_2.png", gRenderer)) {
        cout << "Load BG Layer2 failed! " << endl;
        success = false;
    }
    if (!Layer3.loadFromFile("imgs/background/background_layer_3.png", gRenderer)) {
        cout << "Load BG Layer3 failed! " << endl;
        success = false;
    }
    if (!Layer4.loadFromFile("imgs/background/background_layer_4.png", gRenderer)) {
        cout << "Load BG Layer4 failed! " << endl;
        success = false;
    }
    if (!Ground.loadFromFile("imgs/background/ground.png", gRenderer)) {
        cout << "Load Ground failed! " << endl;
        success = false;
    }
}