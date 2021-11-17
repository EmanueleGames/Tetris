#pragma once

#include "clGameState.h"

class clGameOverState : public clGameState
{

    public:

        //Load State assets
        clGameOverState();
        //Free State assets
        ~clGameOverState();

        //Game loop functions
        void handle_events();
        void logic();
        void render();

    private:

        //Game Over screen resources and variables
        clTexture mGameOverBackground;
        clTexture mTextFinalPoints;
        clTexture mTextPlayAgain;

};

