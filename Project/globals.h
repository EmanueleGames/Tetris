#pragma once

#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "constants.h"
#include "clTexture.h"
#include "clTimer.h"
#include "clGameState.h"

using namespace std;

class clGameState;
class clTexture;
class clTimer;

//Gaming window and window renderer
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

//Game status variables
extern eGameStates gActualState;
extern eGameStates gNextState;
extern clGameState* ptrCurrentState;

//The event structure
extern SDL_Event gEvent;

//Game play variables
extern int gLevel;
extern int gPoints;
extern int gLines;
extern Uint32 gDropDelay;
extern clTexture gBlockTexture;
extern TTF_Font* gFontHUD;

//FPS timer and variables
extern clTimer   gCalcFPStimer;
extern clTexture gFPSTextTexture;
extern TTF_Font* gFontFPS;
extern float gAvgFPS;
extern int   gCountedFrames;
extern int   gDecimalDigitValue;
extern stringstream gTextFPS;

//Global Textures and Colors
extern const SDL_Color textColorBlack;
extern const SDL_Color textColorWhite;

/*************** FUNCTIONS ***************/

bool SDL_initialize();		//Starts up SDL and creates window
void SDL_close();			//Frees media and shuts down SDL
bool load_game_data();		//Loads media
void free_game_data();		//Frees media
bool game_initialize();		//Set the game starting variables

//State functions
void set_next_state(eGameStates newState);
void change_state();

//utility functions
float roundDown(float number, const int value);
void calc_and_texture_FPS();