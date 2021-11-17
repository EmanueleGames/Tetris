#include "clStack.h"

clStack::clStack()
{
	mGrid = NULL;
	mGrid = new Block* [STACK_WIDTH];
	for (int i = 0; i < STACK_WIDTH; i++)
	{
		mGrid[i] = new Block[STACK_HEIGHT];
		for (int j = 0; j < STACK_HEIGHT; j++)
		{
			mGrid[i][j].free = true;
			mGrid[i][j].color = { 0, 0, 0, 255 };
		}
	}

	mGameOverIndex = STACK_HEIGHT-1;
}

clStack::~clStack()
{
	for (int i = 0; i < STACK_WIDTH; i++)
	{
		delete[] mGrid[i];
	}
	delete[] mGrid;
	mGrid = NULL;
}

//Handling functions
void clStack::render()
{
	int xPixel = 0, yPixel = 0;

	for (int i = 0; i < STACK_WIDTH; i++)
	{
		for (int j = 0; j < STACK_HEIGHT; j++)
		{
			if (!mGrid[i][j].free)
			{
				xPixel = GRID_LEFT_MARGIN + (i * BLOCK_DIM);
				yPixel = GRID_TOP_MARGIN + (j * BLOCK_DIM);
				//mBlockTexture.setColorModulation(mGrid[i][j].color.r, mGrid[i][j].color.g, mGrid[i][j].color.b);
				//mBlockTexture.render(gRenderer, xPixel, yPixel);
				gBlockTexture.setColorModulation(mGrid[i][j].color.r, mGrid[i][j].color.g, mGrid[i][j].color.b);
				gBlockTexture.render(gRenderer, xPixel, yPixel);
			}
		}
	}
}

//Stauts Funcions
bool clStack::isBlockFree(int x, int y) const
{
	return mGrid[x][y].free;
}

void clStack::setBlock(int x, int y, SDL_Color color)
{
	mGrid[x][y].free = false;
	mGrid[x][y].color = color;
}

//Utility functions
int clStack::checkForLines()
{
	int n_lines_completed = 0;
	
	bool line_complete;
	for (int j = 0; j < STACK_HEIGHT; j++)
	{
		line_complete = true;
		for (int i = 0; i < STACK_WIDTH && line_complete; i++)
		{
			if (mGrid[i][j].free)
				line_complete = false;
		}
		if (line_complete)
		{
			deleteLine(j);
			n_lines_completed++;
		}
	}
	return n_lines_completed;
}

void clStack::deleteLine(const int lineNumber)
{
	for (int j = lineNumber; j >= 0; j--)
	{
		for (int i = 0; i < STACK_WIDTH; i++)
		{
			if (j != 0)
			{
				mGrid[i][j].free = mGrid[i][j-1].free;
				mGrid[i][j].color = mGrid[i][j-1].color;
			}
			else
			{
				mGrid[i][j].free = true;
				mGrid[i][j].color = { 0, 0, 0, 255 };
			}
		}
	}
}

bool clStack::animatingGameOver()
{
	if (mGameOverIndex >= 0)
	{
		for (int i = 0; i < STACK_WIDTH; i++)
		{
			mGrid[i][mGameOverIndex].color = textColorWhite;
		}
		mGameOverIndex--;
		return true;
	}

	return false;
}