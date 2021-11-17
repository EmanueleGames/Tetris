#include "clTitleState.h"

using namespace std;

clTitleState::clTitleState()
{
    mFadeOut = false;
    mTextAlpha = 255;

    //Load title background
    if (!mTitleBackground.createFromFile(gRenderer, "data/tetris_title_background.png"))
    {
        cout << "Background image loading failed!" << endl;
    }
    mTitleBackground.setBlendMode(SDL_BLENDMODE_BLEND); //ready to fade out

    string messageAuthor = "by Emanuele Carrino";
    if (!mTextAuthor.createFromText(gRenderer, gFontHUD, messageAuthor.c_str(), textColorWhite))
    {
        cout << "Unable to render Intro message texture!" << endl;
    }
    mTextAuthor.setBlendMode(SDL_BLENDMODE_BLEND); //ready to fade out

    string messageEnter = "Press Enter";
    if (!mTextEnter.createFromText(gRenderer, gFontHUD, messageEnter.c_str(), textColorWhite))
    {
        cout << "Unable to render Enter message texture!" << endl;
    }
    mTextEnter.setBlendMode(SDL_BLENDMODE_BLEND); //ready to fade out

}

clTitleState::~clTitleState()
{
    //Free the resources
    mTitleBackground.freeTexture();
    mTextAuthor.freeTexture();
    mTextEnter.freeTexture();
}

void clTitleState::handle_events()
{
    //Polling for a new event
    while (SDL_PollEvent(&gEvent))
    {
        //User requests to exit
        if (gEvent.type == SDL_QUIT)
        {
            set_next_state(eGameStates::STATE_EXIT);
        }
        //Key press
        else if ((gEvent.type == SDL_KEYDOWN))
        {
            if (gEvent.key.keysym.sym == SDLK_RETURN)    // user confirms option
            {
                //Move to the Play screen
                //set_next_state(eGameStates::STATE_PLAY);  //removed to realize fade out
                if (mFadeOut == false)
                {
                    mFadeOut = true;
                }
            }
        }
    }
}

void clTitleState::logic()
{
    //Calculate create texture for FPS
    calc_and_texture_FPS();

    //Fade out
    if (mFadeOut == true)
    {
        mTextAlpha = mTextAlpha - 3;

        //fade out
        mTitleBackground.setAlphaModulation(mTextAlpha);
        mTextAuthor.setAlphaModulation(mTextAlpha);
        mTextEnter.setAlphaModulation(mTextAlpha);
    }
    if (mTextAlpha == 0)
    {
        //Move to the Play screen
        set_next_state(eGameStates::STATE_PLAY);
    }
}

void clTitleState::render()
{
    //Clear the window
    SDL_SetRenderDrawColor(gRenderer, 20, 20, 20, 255);  //almost black
    SDL_RenderClear(gRenderer);

    //Show the background
    mTitleBackground.render(gRenderer, 0, 0);

    //Average FPS
    gFPSTextTexture.render(gRenderer, 2, 0);

    //Show the Text
    mTextAuthor.render(gRenderer, (WINDOW_WIDTH - mTextAuthor.getWidth()) / 2, 260);
    mTextEnter.render(gRenderer, (WINDOW_WIDTH - mTextEnter.getWidth()) / 2, 600);
}