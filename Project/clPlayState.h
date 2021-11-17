#pragma once

#include "clGameState.h"

#include "clTetromino.h"
#include "clStack.h"

class clPlayState : public clGameState
{

    public:

        //Load State assets
        clPlayState();
        //Free State assets
        ~clPlayState();

        //Main loop functions
        void handle_events();
        void logic();
        void render();

    private:

        //Playing screen resources and variables
        clTexture   mBackground;
        clTetromino mWaitingTetromino;
        clTetromino mActiveTetromino;
        clStack     mGrid;

        bool mGameOver;
        int  mGridRow;
        Uint32 mLastDropTime;
        Uint32 mLastAnimationTime;

        //HUD assets
        clTexture mNextLabel;
        clTexture mLevelLabel;
        clTexture mLevelNumber;
        clTexture mLinesLabel;
        clTexture mLinesNumber;
        clTexture mPointsLabel;
        clTexture mPointsNumber;

        // Music and Sounds
        Mix_Music* mGameboyNostalgia;
        Mix_Chunk* mGameOver_sfx;

        //utility functions
        void reset_game();

};