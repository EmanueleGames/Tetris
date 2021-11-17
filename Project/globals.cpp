#include <iostream>

#include "globals.h"
#include "clTitleState.h"
#include "clPlayState.h"
#include "clGameOverState.h"

using namespace std;

//Gaming window and window renderer
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//Game status variables
eGameStates  gActualState = eGameStates::STATE_NULL;
eGameStates  gNextState = eGameStates::STATE_NULL;
clGameState* ptrCurrentState = NULL;

//The event structure
SDL_Event gEvent;

//Game play variables
int gLevel  = STARTING_LEVEL;
int gPoints = 0;
int gLines  = 0;
Uint32 gDropDelay = STARTING_DELAY - (SPEED_UP_VALUE * STARTING_LEVEL);
clTexture gBlockTexture;
TTF_Font* gFontHUD;

//FPS timer and variables
clTimer   gCalcFPStimer;
clTexture gFPSTextTexture;
TTF_Font* gFontFPS;
float gAvgFPS;
int   gCountedFrames;
int   gDecimalDigitValue;
stringstream gTextFPS;

//Global Textures and Colors
const SDL_Color textColorBlack = { 0, 0, 0, 255 };
const SDL_Color textColorWhite = { 240, 240, 240, 255 };

/*************** FUNCTIONS ***************/

bool SDL_initialize()
{

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		cout << "SDL failed to initialize! SDL Error: " << SDL_GetError() << endl;
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		cout << "Warning: Linear texture filtering not enabled!" << endl;
	}

	//Create Game Window
	gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		cout << "Window creation failed! SDL Error: " << SDL_GetError() << endl;
		return false;
	}

	//Create Renderer for the window (vsync ON)
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
	{
		cout << "Renderer creation failed! SDL Error: " << SDL_GetError() << endl;
		return false;
	}

	//PNG loading initialization (SDL_image)
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		cout << "SDL_image failed to initialize! SDL_image Error: " << IMG_GetError() << endl;
		return false;
	}

	//Font loading initialization (SDL_ttf)
	if (TTF_Init() == -1)
	{
		cout << "SDL_ttf failed to initialize! SDL_ttf Error: " << TTF_GetError() << endl;
		return false;
	}

	//Audio loading initialization (SDL_mixer)
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "SDL_mixer failed to initialize! SDL_mixer Error: " << Mix_GetError() << endl;
		return false;
	}

	//Everything was fine
	return true;
}

void SDL_close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool load_game_data()
{
	//Success flag
	bool fl_success = true;

	//Open the fonts
	gFontFPS = TTF_OpenFont("data/Quicksand.ttf", 12);
	if (gFontFPS == NULL)
	{
		cout << "Failed to load Quicksand font! SDL_ttf Error: " << TTF_GetError() << endl;
		fl_success = false;
	}

	//Open HUD font
	gFontHUD = TTF_OpenFont("data/early_gameboy.ttf", 40);
	if (gFontHUD == NULL)
	{
		cout << "Failed to load early_gameboy font for labels! SDL_ttf Error: " << TTF_GetError() << endl;
		fl_success = false;
	}

	return fl_success;
}

void free_game_data()
{
	TTF_CloseFont(gFontFPS);
	TTF_CloseFont(gFontHUD);
	gFontFPS = NULL;
	gFontHUD = NULL;
}

bool game_initialize()
{
	//Success flag
	bool fl_success = true;

	//FPS counter initialization
	gDecimalDigitValue = 1;
	if (FPS_DECIMAL_DIGIT > 0)
	{
		for (int i = 0; i < FPS_DECIMAL_DIGIT; i++)
		{
			gDecimalDigitValue *= 10;
		}
	}
	float gAvgFPS = 0;
	int gCountedFrames = 0;

	//Game starts in Intro State
	gActualState = eGameStates::STATE_TITLE;
	//Set the current game state object
	ptrCurrentState = new clTitleState();

	srand((unsigned int)time(NULL));

	return fl_success;
}

void set_next_state(eGameStates newState)
{
	//Only set next state if the user hasn't requested to exit
	if (gNextState != eGameStates::STATE_EXIT)
	{
		gNextState = newState;
	}
}

void change_state()
{
	//If next state is a valid one
	if (gNextState != eGameStates::STATE_NULL)
	{
		//If we need to exit, we don't delete the current state
		if (gNextState != eGameStates::STATE_EXIT)
		{
			delete ptrCurrentState;
		}

		//Change the state
		switch (gNextState)
		{
		case eGameStates::STATE_TITLE:
			ptrCurrentState = new clTitleState();
			break;
		case eGameStates::STATE_PLAY:
			ptrCurrentState = new clPlayState();
			break;
		case eGameStates::STATE_GAME_OVER:
			ptrCurrentState = new clGameOverState();
			break;
		}

		//update the actual anx next state
		gActualState = gNextState;
		gNextState = eGameStates::STATE_NULL;
	}
}

float roundDown(float number, const int value)
{
	// EXAMPLE with 2 digits (value = 100):
	// 12.3456 * 100 = 1234.56		to shift the point
	// 1234.56 + .5 = 1235.06		for rounding off value 
	// then type cast to int so value is 1235
	// then divided by 100 so the value converted into 12.35 

	number = (int)(number * value + .5);
	return (number / value);
}

void calc_and_texture_FPS()
{
	gAvgFPS = gCountedFrames / (gCalcFPStimer.getTicks() / 1000.f);
	if (gAvgFPS > 2000000)
	{
		gAvgFPS = 0;
	}
	else
	{
		gAvgFPS = roundDown(gAvgFPS, gDecimalDigitValue);
	}

	//FPS text
	gTextFPS.str("");
	gTextFPS << "Average FPS (vsync ON):  " << gAvgFPS;
	if (!gFPSTextTexture.createFromText(gRenderer, gFontFPS, gTextFPS.str().c_str(), textColorWhite))
	{
		cout << "Unable to render FPS texture!" << endl;
	}
}