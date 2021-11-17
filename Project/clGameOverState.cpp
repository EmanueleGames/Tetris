#include "clGameOverState.h"

using namespace std;

clGameOverState::clGameOverState()
{

    //Load game over background
    if (!mGameOverBackground.createFromFile(gRenderer, "data/tetris_gameover_background.png"))
    {
        cout << "Background image loading failed!" << endl;
    }

    //Game Over messagges
    stringstream stringFinalScore;
    stringFinalScore.str("");
    stringFinalScore << "Final Score:  " << gPoints;
    if (!mTextFinalPoints.createFromText(gRenderer, gFontHUD, stringFinalScore.str().c_str(), textColorWhite))
    {
        cout << "Unable to create Final Points message texture!" << endl;
    }

    string messagePlayAgain = "Press ENTER to play again!";
    if (!mTextPlayAgain.createFromText(gRenderer, gFontHUD, messagePlayAgain.c_str(), textColorWhite))
    {
        cout << "Unable to create Play Again message texture!" << endl;
    }
}

clGameOverState::~clGameOverState()
{
    //Free the resources
    mGameOverBackground.freeTexture();
    mTextFinalPoints.freeTexture();
    mTextPlayAgain.freeTexture();
}

void clGameOverState::handle_events()
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
            if (gEvent.key.keysym.sym == SDLK_RETURN)
            {
                //Move to the Play screen
                set_next_state(eGameStates::STATE_PLAY);
            }
        }
    }
}

void clGameOverState::logic()
{
    //Calculate create texture for FPS
    calc_and_texture_FPS();
}

void clGameOverState::render()
{
    //Clear the window
    SDL_SetRenderDrawColor(gRenderer, 20, 20, 20, 255);  //almost black
    SDL_RenderClear(gRenderer);

    //Show the background
    mGameOverBackground.render(gRenderer, 0, 0);

    //Average FPS
    gFPSTextTexture.render(gRenderer, 2, 0);

    //Show the Game result
    mTextFinalPoints.render(gRenderer, (WINDOW_WIDTH - mTextFinalPoints.getWidth()) / 2, 260);
    mTextPlayAgain.render(gRenderer, (WINDOW_WIDTH - mTextPlayAgain.getWidth()) / 2, 600);
}