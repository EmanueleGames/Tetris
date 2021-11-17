#pragma once

//Window resolution
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

//Assets dimension
const int BLOCK_DIM = 30;
const int STACK_WIDTH = 10;
const int STACK_HEIGHT = 20;
const int GRID_LEFT_MARGIN = 490;
const int GRID_TOP_MARGIN  = 70;
const int MINI_GRID_LEFT_MARGIN = 185;
const int MINI_GRID_TOP_MARGIN  = 300;

//Game Settings
const int ANIMATION_DELAY   = 100;  //milliseconds
const int STARTING_LEVEL    = 0;
const int MAX_LEVEL         = 9;
const int STARTING_DELAY    = 1500; //milliseconds
const int SPEED_UP_VALUE    = 125;  //milliseconds
const int LINES_FOR_LEVELUP = 10;

//FPS limit
const int FPS_DECIMAL_DIGIT = 0;

//Game states
enum class eGameStates
{
    STATE_NULL,
    STATE_TITLE,
    STATE_PLAY,
    STATE_GAME_OVER,
    STATE_EXIT,
};
