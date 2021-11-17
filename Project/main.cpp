#include <iostream>

#include "constants.h"
#include "globals.h"
#include "clTexture.h"
#include "clTimer.h"
#include "clGameState.h"
#include "clTitleState.h"
#include "clPlayState.h"
#include "clGameOverState.h"


using namespace std;

int main(int argc, char* argv[])
{
    
    //SLD components initialization
    if (!SDL_initialize())
    {
        cout << "SDL initialization Failed!" << endl;
        return -1;
    }
    
    //Load game assets
    if (!load_game_data())
    {
        cout << "Failed to load media!" << endl;
        return -1;
    }

    //Game initialization
    if (!game_initialize())
    {
        cout << "Failed to initialize the game!" << endl;
        return -1;
    }

    //Start the FPS calculator timer
    gCalcFPStimer.start();
    
    //While the user hasn't quit
    while (gActualState != eGameStates::STATE_EXIT)
    {
        
        //Handle events on queue
        ptrCurrentState->handle_events();

        //Handle the state logic
        ptrCurrentState->logic();

        //Change state if needed
        change_state();

        //Render current state
        ptrCurrentState->render();
        
        //Switch front and back buffer
        SDL_RenderPresent(gRenderer);

        gCountedFrames++;
    }
    
    //Free game assets
    free_game_data();
    //Free memory and close SDL
    SDL_close();

    return 0;
    
}