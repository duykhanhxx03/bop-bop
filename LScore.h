#pragma once
#include "LTexture.h"
#include "LTimer.h"
#include "CommonFunction.h"
#include <fstream>
#include <sstream>
class LScore {
private:
    SDL_Color textColor = { 255, 255, 255, 255 };
    //Text render
    LTexture gCurrentScoreTexture;
    LTexture gHighScoreTexture;

    //Current time start time
    Uint32 startTime = 0;
    stringstream timeText;
    LTimer gTimer;

    //ishighscore
    bool isHighSc = false;
    Uint32 score = 0;
    Uint32 savedScore = 0;
    Uint32 scoreAdded = 0;
public:
    void process();
    void pause();
    void start();
    void reStart();
    void render(SDL_Renderer* gRenderer, TTF_Font* Font);
    void shiftScore();
    void addScore(const Uint32& sc);
    void resume();
    void stop();
    void updateHighScore(SDL_Renderer* gRenderer, TTF_Font* Font);
    int getScore();
    bool isStarted();
    void setScoreFromSaved(const Uint32& sc, SDL_Renderer* gRenderer, TTF_Font* Font);
    Uint32 getHighScore() const;
    bool isHighScore();
    void loadMedia(bool& success, SDL_Renderer* gRenderer, TTF_Font* Font);
};
static LScore SCORE;
