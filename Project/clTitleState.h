#pragma once

#include "clGameState.h"

class clTitleState : public clGameState
{
    public:

        //Load State assets
        clTitleState();
        //Free State assets
        ~clTitleState();

        //Game loop functions
        void handle_events();
        void logic();
        void render();

    private:

        //Title screen resources and variables
        clTexture mTitleBackground;
        clTexture mTextAuthor;
        clTexture mTextEnter;

        bool mFadeOut;
        Uint8 mTextAlpha;
};

