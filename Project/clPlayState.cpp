#include "clPlayState.h"

using namespace std;

clPlayState::clPlayState()
{
    //Load background textures
    if (!mBackground.createFromFile(gRenderer, "data/tetris_play_background.png"))
    {
        cout << "Background image loading failed!" << endl;
    }

    //Load block texture (only called once since static)
    if (!gBlockTexture.createFromFile(gRenderer, "data/tetromino_block.png"))
    {
        cout << "Block image loading failed!" << endl;
    }

    //Load music and sound effects
    mGameboyNostalgia = Mix_LoadMUS("data/tetris_gameboy.wav");
    if (mGameboyNostalgia == NULL)
    {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    else
    {
        Mix_VolumeMusic(2);
    }
    lRotation_sfx = Mix_LoadWAV("data/rotation_sfx.wav");
    if (lRotation_sfx == NULL)
    {
        cout << "Failed to rotation SFX! SDL_mixer Error: " << Mix_GetError() << endl;
    }
    else
    {
        Mix_VolumeChunk(lRotation_sfx, 20);
    }
    lHardDrop_sfx = Mix_LoadWAV("data/harddrop_sfx.wav");
    if (lHardDrop_sfx == NULL)
    {
        cout << "Failed to rotation SFX! SDL_mixer Error: " << Mix_GetError() << endl;
    }
    else
    {
        Mix_VolumeChunk(lHardDrop_sfx, 50);
    }

    mGameOver_sfx = Mix_LoadWAV("data/gameover_sfx.wav");
    if (mGameOver_sfx == NULL)
    {
        cout << "Failed to rotation SFX! SDL_mixer Error: " << Mix_GetError() << endl;
    }
    else
    {
        Mix_VolumeChunk(mGameOver_sfx, 20);
    }
 

    // Labels fo HUD
    string nextLabel = "NEXT";
    if (!mNextLabel.createFromText(gRenderer, gFontHUD, nextLabel.c_str(), textColorWhite))
    {
        cout << "Unable to render NEXT label texture!" << endl;
    }

    string levelLabel = "LEVEL";
    if (!mLevelLabel.createFromText(gRenderer, gFontHUD, levelLabel.c_str(), textColorWhite))
    {
        cout << "Unable to render LEVEL label texture!" << endl;
    }

    string linesLabel = "LINES";
    if (!mLinesLabel.createFromText(gRenderer, gFontHUD, linesLabel.c_str(), textColorWhite))
    {
        cout << "Unable to render LINES label texture!" << endl;
    }
    string pointsLabel = "SCORE";
    if (!mPointsLabel.createFromText(gRenderer, gFontHUD, pointsLabel.c_str(), textColorWhite))
    {
        cout << "Unable to render POINTS label texture!" << endl;
    }

    reset_game();
    mActiveTetromino.activate();
    Mix_PlayMusic(mGameboyNostalgia, -1); // -1 means repeat
}

clPlayState::~clPlayState()
{
    mBackground.freeTexture();
    gBlockTexture.freeTexture();

    Mix_FreeMusic(mGameboyNostalgia);
    Mix_FreeChunk(mGameOver_sfx);
    Mix_FreeChunk(lRotation_sfx);
    Mix_FreeChunk(lHardDrop_sfx);
    mGameboyNostalgia = NULL;
    mGameOver_sfx = NULL;
    lRotation_sfx = NULL;
    lHardDrop_sfx = NULL;
    
    mNextLabel.freeTexture();
    mLevelLabel.freeTexture();
    mLinesLabel.freeTexture();
    mPointsLabel.freeTexture();

}

void clPlayState::handle_events()
{
    //Polling for a new event
    while (SDL_PollEvent(&gEvent))
    {
        //User requests to exit
        if (gEvent.type == SDL_QUIT)
        {
            set_next_state(eGameStates::STATE_EXIT);
        }
        
        mActiveTetromino.handleEvent(gEvent);
    }
}

void clPlayState::logic()
{
    
    string stringLevel;
    string stringLines;
    string stringPoints;
    
    //Calculate create texture for FPS
    calc_and_texture_FPS();

    if (mGameOver)
    {
        //game over animation
        if (SDL_GetTicks() - mLastAnimationTime > ANIMATION_DELAY)
        {
            if (mGrid.animatingGameOver())
            {
                mLastAnimationTime = SDL_GetTicks();
            }
            else //animation over
            {
                SDL_Delay(500);
                set_next_state(eGameStates::STATE_GAME_OVER);
            }
        }
    }

    else
    {
        mActiveTetromino.move(mGrid);

        // check if the tetromino has to be dropped
        if (SDL_GetTicks() - mLastAnimationTime > gDropDelay)
        {
            mActiveTetromino.drop(mGrid);
            mLastAnimationTime = SDL_GetTicks(); //reset the autodrop delay
        }

        // If the current active tetromino had been placed...
        if (mActiveTetromino.isPlaced())
        {
            //reset the autodrop delay, to avoid the immediate drop of the waiting piece
            mLastAnimationTime = SDL_GetTicks(); 

            //check for completed lines and increase counter
            int lLines = 0;
            lLines = mGrid.checkForLines();
            if (lLines > 0)
            {
                gPoints = gPoints + (10 * (gLevel+1) * lLines * lLines);    //update points
                gLines = gLines + lLines;                                   //update total lines
                if (gLines >= LINES_FOR_LEVELUP && gLevel < MAX_LEVEL)      //check for level up
                {
                    gLevel = STARTING_LEVEL + (gLines / LINES_FOR_LEVELUP);  //increase level
                    gDropDelay = STARTING_DELAY - (SPEED_UP_VALUE * gLevel); //speed up the pieces
                }
            }

            // Remove the active one
            mActiveTetromino.freeTetromino();
            // Waiting becomes active
            mActiveTetromino = mWaitingTetromino;
            mActiveTetromino.activate();

            //game over check
            if (!mActiveTetromino.validPosition(mGrid))
            {
                mGameOver = true;
                mActiveTetromino.addToStack(mGrid);
                mLastAnimationTime = SDL_GetTicks();
                Mix_HaltMusic();
                Mix_PlayChannel(-1, mGameOver_sfx, 0);
            }

            // Generate a new waiting Piece
            mWaitingTetromino.freeTetromino();
            mWaitingTetromino = clTetromino();
        }

        //Update HUD Values
        stringLevel = std::to_string(gLevel);
        stringLines = std::to_string(gLines);
        stringPoints = std::to_string(gPoints);
        if (!mLevelNumber.createFromText(gRenderer, gFontHUD, stringLevel.c_str(), textColorWhite))
        {
            cout << "Unable to render Level Number texture!" << endl;
        }
        if (!mLinesNumber.createFromText(gRenderer, gFontHUD, stringLines.c_str(), textColorWhite))
        {
            cout << "Unable to render Lines Number texture!" << endl;
        }
        if (!mPointsNumber.createFromText(gRenderer, gFontHUD, stringPoints.c_str(), textColorWhite))
        {
            cout << "Unable to render Points Number texture!" << endl;
        }
    }
}

void clPlayState::render()
{
    //Clear the window
    SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);  //almost white
    SDL_RenderClear(gRenderer);

    //Show the background
    mBackground.render(gRenderer, 0, 0);

    mGrid.render();                 // grid stack
    mWaitingTetromino.render();     // waiting piece
    if(!mGameOver)
        mActiveTetromino.render();  // active piece
    
    // HUD Lables
    mNextLabel.render(gRenderer, 172, 230);
    mLevelLabel.render(gRenderer, 850, 140);
    mLinesLabel.render(gRenderer, 850, 320);
    mPointsLabel.render(gRenderer, 850, 480);    
    // HUD Values
    mLevelNumber.render(gRenderer, 850, 190);
    mLinesNumber.render(gRenderer, 850, 370);
    mPointsNumber.render(gRenderer, 850, 530);

    //Average FPS
    gFPSTextTexture.render(gRenderer, 2, 0);
}

void clPlayState::reset_game()
{

    gLevel = STARTING_LEVEL;
    gPoints = 0;
    gLines = 0;
    gDropDelay = STARTING_DELAY - (SPEED_UP_VALUE * STARTING_LEVEL);

    mGameOver           = false;
    mGridRow            = 0;
    mLastDropTime       = SDL_GetTicks();
    mLastAnimationTime  = 0;

    srand((unsigned int)time(NULL));
    
}