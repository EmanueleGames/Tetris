#pragma once

#include "constants.h"
#include "globals.h"

//Base Class: Game State
class clGameState
{

    public:
        //Destructor
        virtual ~clGameState() {};
        //Game loop functions
        virtual void handle_events() = 0;
        virtual void logic() = 0;
        virtual void render() = 0;

};