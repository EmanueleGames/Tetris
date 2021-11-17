#pragma once

#include "constants.h"
#include "globals.h"

struct Block
{
	bool	  free;
	SDL_Color color; //Tetromino Color
};

class clStack
{
	public:

		clStack();
		~clStack();

		//Stauts Funcions
		bool isBlockFree(int x, int y) const;
		void setBlock(int x, int y, SDL_Color color);
		int checkForLines();   //0 for no lines, 1,2,3,4 if 1-4 lines
		bool animatingGameOver();
		
		//Handling functions
		void render();

	private:

		Block** mGrid;		   //the Stack of Blocks
		int mGameOverIndex;

		//Utility
		void deleteLine(const int lineNumber);
};

