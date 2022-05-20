#include "LScore.h"

//Score
void LScore::loadMedia(bool& success, SDL_Renderer* gRenderer, TTF_Font* Font) {
    Uint32 savedScore;
    fstream savedScoreInfo("score/score.txt", ios::in);
    if (!savedScoreInfo.is_open()) {
        success = false;
    }
    else
    {
        savedScoreInfo >> savedScore;
        setScoreFromSaved(savedScore,gRenderer,Font);
        savedScoreInfo.close();
    }
}
void LScore::addScore(const Uint32& sc) {
    scoreAdded += sc;
    timeTextScoreAdded.str("");
    timeTextScoreAdded << "+" << scoreAdded;
}
void LScore::stop() {
    scoreAdded = 0;
    if (gTimer.isStarted()) gTimer.stop();
}
void LScore::updateHighScore(SDL_Renderer* gRenderer, TTF_Font* Font) {
    timeText.str("");
    if (score +scoreAdded> savedScore) {
        isHighSc = true;
        savedScore = score+scoreAdded;
        timeText << "HI: " << savedScore;
        if (!gHighScoreTexture.loadFromRenderedText(gRenderer, timeText.str().c_str(), textColor, Font)) {
            cout << "Unable to render savedtime texture!" << endl;
        }
    }
    else if (score < savedScore) isHighSc = false;
}
void LScore::setScoreFromSaved(const Uint32& sc, SDL_Renderer* gRenderer, TTF_Font* Font) {
    savedScore = sc;
    timeText << "HI: " << savedScore;
    if (!gHighScoreTexture.loadFromRenderedText(gRenderer, timeText.str().c_str(), textColor, Font)) {
        cout << "Unable to render savedtime texture!" << endl;
    }
}
void LScore::process() {
    //Current score
    timeText.str("");
    shiftScore();
    timeText << score - startTime;

    //Added score
    timeTextScoreAdded.str("");
    timeTextScoreAdded << "+" << scoreAdded;
}
void LScore::pause() {
    if (!gTimer.isPaused()) gTimer.pause();
}
void LScore::start() {
    scoreAdded = 0;
    timeTextScoreAdded.str("");
    if (!gTimer.isStarted()) gTimer.start();
    
}
void LScore::reStart() {
    scoreAdded = 0;
    timeTextScoreAdded.str("");
    if (gTimer.isStarted()) gTimer.start();
}
void LScore::render(SDL_Renderer* gRenderer, TTF_Font* Font) {
    if (!gCurrentScoreTexture.loadFromRenderedText(gRenderer, timeText.str().c_str(), textColor, Font)) {
        cout << "Unable to render timetexture!" << endl;
    }
    if (!gScoreAddedTexture.loadFromRenderedText(gRenderer, timeTextScoreAdded.str().c_str(), textColor, Font))
    {
        cout << "Unable to render timeTextScoreAdded! " << endl;
    }   
    gCurrentScoreTexture.render(gRenderer, SCREEN_WIDTH - gCurrentScoreTexture.getWidth()-20, 10);
    gHighScoreTexture.render(gRenderer, (SCREEN_WIDTH - gHighScoreTexture.getWidth()) / 2, 10);
    gScoreAddedTexture.render(gRenderer, SCREEN_WIDTH - gScoreAddedTexture.getWidth()-20, 60);  
}
void LScore::shiftScore() {
    score = gTimer.getTicks() / 250;
}
void LScore::resume() {
    if (gTimer.isPaused()) gTimer.unpause();
}
bool LScore::isStarted() {
    return gTimer.isStarted();
}
bool LScore::isHighScore() {
    return isHighSc;
}
Uint32 LScore::getHighScore() const {
    return savedScore;
}
int LScore::getScore() {
    shiftScore();
    return score+scoreAdded;
}