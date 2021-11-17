#pragma once

#include "constants.h"
#include "globals.h"

class clTimer
{
	public:
		//Initialization
		clTimer();

		//Timer funcions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's ticks
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

	private:
		//Ticks value when the timer started
		Uint32 mStartTicks;

		//Ticks value when the timer was paused
		Uint32 mPausedTicks;

		//Timer status
		bool mPaused;
		bool mStarted;
};