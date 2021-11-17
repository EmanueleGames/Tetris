#include "clTetromino.h"

Mix_Chunk* lRotation_sfx;
Mix_Chunk* lHardDrop_sfx;

clTetromino::clTetromino(eTetrominoTypes type)
{
	//Initialization
	mMatrix			= NULL;
	mMatrixMoved	= NULL;
	mDim			= 4;
	mX				= 0;
	mY				= 0;	
	mOrientation	= eOrientation::UP;
	mNextMove		= eTetrominoMoves::NO_MOVE;
	mStatus			= eTetrominoStatus::TETROMINO_WAITING;
	mColor			= { 0, 0, 0, 255 };
	alloc_matrices();

	if (type == eTetrominoTypes::UNDEFINED)
	{
		mType =(eTetrominoTypes) (rand() % 7);
	}
	else
		mType = type;

	switch (mType)
	{
	case eTetrominoTypes::TETROMINO_I:
		mX = 3;
		mY = -1;
		mDim = 4;
		mColor = COLOR_TETROMINO_I;
		init_matrices(0, 1, 1, 1, 2, 1, 3, 1);
		break;

	case eTetrominoTypes::TETROMINO_J:
		mX = 3;
		mY = 0;
		mDim = 3;
		mColor = COLOR_TETROMINO_J;
		init_matrices(0, 0, 0, 1, 1, 1, 2, 1);
		break;

	case eTetrominoTypes::TETROMINO_L:
		mX = 3;
		mY = 0;
		mDim = 3;
		mColor = COLOR_TETROMINO_L;
		init_matrices(0, 1, 1, 1, 2, 0, 2, 1);
		break;

	case eTetrominoTypes::TETROMINO_O:
		mX = 3;
		mY = -1;
		mDim = 4;
		mColor = COLOR_TETROMINO_O;
		init_matrices(1, 1, 1, 2, 2, 1, 2, 2);
		break;

	case eTetrominoTypes::TETROMINO_S:
		mX = 3;
		mY = 0;
		mDim = 3;
		mColor = COLOR_TETROMINO_S;
		init_matrices(0, 1, 1, 1, 1, 0, 2, 0);
		break;

	case eTetrominoTypes::TETROMINO_Z:
		mX = 3;
		mY = 0;
		mDim = 3;
		mColor = COLOR_TETROMINO_Z;
		init_matrices(0, 0, 1, 0, 1, 1, 2, 1);
		break;

	case eTetrominoTypes::TETROMINO_T:
		mX = 3;
		mY = 0;
		mDim = 3;
		mColor = COLOR_TETROMINO_T;
		init_matrices(0, 1, 1, 0, 1, 1, 2, 1);
		break;

	default:
		cout << "Wrong Tetromino type!" << endl;
		break;
	}
}

clTetromino::clTetromino(const clTetromino& tetromino)
{
	mDim = tetromino.mDim;
	mMatrix = NULL;
	mMatrixMoved = NULL;
	alloc_matrices();

	//Populate the Moved Matrix
	for (int i = 0; i < mDim; i++)
	{
		for (int j = 0; j < mDim; j++)
		{
			mMatrix[i][j] = tetromino.mMatrix[i][j];
			mMatrixMoved[i][j] = tetromino.mMatrixMoved[i][j];
		}
	}

	mX = tetromino.mX;
	mY = tetromino.mY;

	mOrientation = tetromino.mOrientation;
	mNextMove = tetromino.mNextMove;
	mType = tetromino.mType;
	mColor = tetromino.mColor;
	mStatus = tetromino.mStatus;
}

clTetromino::~clTetromino()
{
	freeTetromino();
}

clTetromino& clTetromino::operator=(const clTetromino& tetromino)
{
	// Avoid self assignment
	if (this == &tetromino)
		return *this;

	mDim = tetromino.mDim;
	if (mMatrix != NULL && mMatrixMoved != NULL)
	{
		dealloc_matrices();
	}
	mMatrix = NULL;
	mMatrixMoved = NULL;
	alloc_matrices();		// allocate resource in *this

	//Populate the Rotated Matrix
	for (int i = 0; i < mDim; i++)
	{
		for (int j = 0; j < mDim; j++)
		{
			mMatrix[i][j] = tetromino.mMatrix[i][j];
			mMatrixMoved[i][j] = tetromino.mMatrixMoved[i][j];
		}
	}

	mX = tetromino.mX;
	mY = tetromino.mY;

	mOrientation = tetromino.mOrientation;
	mNextMove = tetromino.mNextMove;
	mType = tetromino.mType;
	mColor = tetromino.mColor;
	mStatus = tetromino.mStatus;

	return *this;
}

void clTetromino::handleEvent(SDL_Event& event)
{
	//Key press
	if ((gEvent.type == SDL_KEYDOWN))
	{
		if (gEvent.key.keysym.sym == SDLK_LEFT)
		{
			mNextMove = eTetrominoMoves::MOVE_LEFT;
		}

		if (gEvent.key.keysym.sym == SDLK_RIGHT)
		{
			mNextMove = eTetrominoMoves::MOVE_RIGHT;
		}

		if (gEvent.key.keysym.sym == SDLK_DOWN)
		{
			mNextMove = eTetrominoMoves::MOVE_DOWN;
		}

		if (gEvent.key.keysym.sym == SDLK_SPACE)
		{
			mNextMove = eTetrominoMoves::MOVE_BOTTOM;
		}

		if (gEvent.key.keysym.sym == SDLK_z)
		{
			mNextMove = eTetrominoMoves::ROTATE_LEFT;
		}

		if (gEvent.key.keysym.sym == SDLK_x)
		{
			mNextMove = eTetrominoMoves::ROTATE_RIGHT;
		}
	}
}

void clTetromino::move(clStack& grid)
{
	if (mNextMove == eTetrominoMoves::MOVE_LEFT)
	{
		translate(-1, 0);
		if (!validPosition(grid))
		{
			translate(1, 0);
		}
	}
	if (mNextMove == eTetrominoMoves::MOVE_RIGHT)
	{
		translate(1, 0);
		if (!validPosition(grid))
		{
			translate(-1, 0);
		}
	}
	if (mNextMove == eTetrominoMoves::MOVE_DOWN)
	{
		translate(0, 1);
		//Check if the tetromino becomes part of the Stack
		if (!validPosition(grid))
		{
			translate(0, -1);
			//Add tetromino to the stack
			addToStack(grid);
			mStatus = eTetrominoStatus::TETROMINO_PLACED;
			Mix_PlayChannel(-1, lHardDrop_sfx, 0);
		}
	}
	if (mNextMove == eTetrominoMoves::MOVE_BOTTOM)
	{
		while (validPosition(grid))
		{
			translate(0, 1);
		}
	    translate(0, -1);
	    //Add tetromino to the stack
	    addToStack(grid);
	    mStatus = eTetrominoStatus::TETROMINO_PLACED;
		Mix_PlayChannel(-1, lHardDrop_sfx, 0);
	}
	if (mNextMove == eTetrominoMoves::ROTATE_LEFT)
	{
		tryRotateAntiClockwise(grid);
	}
	if (mNextMove == eTetrominoMoves::ROTATE_RIGHT)
	{
		tryRotateClockwise(grid);
	}
}

void clTetromino::render()
{

	int xPixel = 0, yPixel = 0;
	int left_margin = 0, top_margin = 0;

	if (mStatus == eTetrominoStatus::TETROMINO_ACTIVE)
	{
		left_margin = GRID_LEFT_MARGIN + (mX * BLOCK_DIM);
		top_margin = GRID_TOP_MARGIN + (mY * BLOCK_DIM);
	}
	else if (mStatus == eTetrominoStatus::TETROMINO_WAITING)
	{
		left_margin = MINI_GRID_LEFT_MARGIN;
		top_margin = MINI_GRID_TOP_MARGIN;
		if (mDim == 3) // in case of 3x3 Tetromino, we center it a bit in the mini grid
		{
			left_margin = left_margin + BLOCK_DIM;
			top_margin = top_margin + BLOCK_DIM;
		}
	}

	for (int i = 0; i < mDim; i++)
	{
		for (int j = 0; j < mDim; j++)
		{
			if (mMatrix[i][j] == true)
			{
				xPixel = left_margin + (i * BLOCK_DIM);
				yPixel = top_margin + (j * BLOCK_DIM);
				gBlockTexture.setColorModulation(mColor.r, mColor.g, mColor.b);
				gBlockTexture.render(gRenderer, xPixel, yPixel);
			}
		}
	}
}

bool clTetromino::isPlaced()
{
	return (mStatus == eTetrominoStatus::TETROMINO_PLACED);
}

bool clTetromino::activate()
{
	if (mStatus == eTetrominoStatus::TETROMINO_WAITING)
	{
		mStatus = eTetrominoStatus::TETROMINO_ACTIVE;
		return true;
	}
	return false;
}

void clTetromino::drop(clStack& grid)
{
	translate(0, 1);
	//Check if the tetromino becomes part of the Stack
	if (!validPosition(grid))
	{
		translate(0, -1);
		//Add tetromino to the stack
		addToStack(grid);
		mStatus = eTetrominoStatus::TETROMINO_PLACED;
		Mix_PlayChannel(-1, lHardDrop_sfx, 0);
	}
}

void clTetromino::freeTetromino()
{
	dealloc_matrices();
	mDim = 4;
	mX = 0;
	mY = 0;
	mOrientation = eOrientation::UP;
	mNextMove = eTetrominoMoves::NO_MOVE;
	mStatus = eTetrominoStatus::TETROMINO_WAITING;
	mColor = { 0, 0, 0, 255 };
}

void clTetromino::translate(const int offsetX, const int offsetY)
{
	mX = mX + offsetX;
	mY = mY + offsetY;

	mNextMove = eTetrominoMoves::NO_MOVE;
}

void clTetromino::tryRotateClockwise(const clStack& grid)
{

	//in case of O-Tetromino, rotation is pointless
	if (mType != eTetrominoTypes::TETROMINO_O)
	{
		int i, j;

		//Populate the Rotated Matrix
		for (i = 0; i < mDim; i++)
		{
			for (j = 0; j < mDim; j++)
			{
				mMatrixMoved[i][j] = mMatrix[j][mDim-1-i];
			}
		}

		//if the position is valid, mMatrix gets rotated
		if (validPosition(grid))
		{
			confirmMove();
			mOrientation = (eOrientation)(((int)mOrientation + 1) % 4);
			Mix_PlayChannel(-1, lRotation_sfx, 0);
		}
		else
		{
			if (tryKicks(grid))
			{
				confirmMove();
				mOrientation = (eOrientation)(((int)mOrientation + 1) % 4);
				Mix_PlayChannel(-1, lRotation_sfx, 0);
			}
			cancelMove();
		}
	}
	mNextMove = eTetrominoMoves::NO_MOVE;
}

void clTetromino::tryRotateAntiClockwise(const clStack& grid)
{
	//in case of O-Tetromino, rotation is pointless
	if (mType != eTetrominoTypes::TETROMINO_O)
	{
		int i, j;

		//Populate the Rotated Matrix
		for (i = 0; i < mDim; i++)
		{
			for (j = 0; j < mDim; j++)
			{
				mMatrixMoved[i][j] = mMatrix[mDim-1-j][i];
			}
		}

		//if the position is valid, mMatrix gets rotated
		if (validPosition(grid))
		{
			confirmMove();
			if (mOrientation == eOrientation::UP)
				mOrientation = eOrientation::LEFT;
			else
				mOrientation = (eOrientation)((int)mOrientation - 1);
			Mix_PlayChannel(-1, lRotation_sfx, 0);
		}
		else
		{
			if (tryKicks(grid))
			{
				confirmMove();
				if (mOrientation == eOrientation::UP)
					mOrientation = eOrientation::LEFT;
				else
					mOrientation = (eOrientation)((int)mOrientation - 1);
				Mix_PlayChannel(-1, lRotation_sfx, 0);
			}
			cancelMove();
		}
	}
	mNextMove = eTetrominoMoves::NO_MOVE;
}

bool clTetromino::tryKicks(const clStack& grid)
{

	int x1 = 0, x2 = 0, x3 = 0, x4 = 0,
		y1 = 0, y2 = 0, y3 = 0, y4 = 0;

	// Rotation direction
	if (mNextMove == eTetrominoMoves::ROTATE_RIGHT)
	{
		// Type I tetromino ha different kicks
		if (mType == eTetrominoTypes::TETROMINO_I)
		{
			// Switch on current orientation
			switch (mOrientation)
			{
			case eOrientation::UP:
				x1 = -2; y1 =  0;
				x2 =  1; y2 =  0;
				x3 = -2; y3 = -1;
				x4 =  1; y4 =  2;
				break;
			case eOrientation::RIGHT:
				x1 = -1; y1 =  0;
				x2 =  2; y2 =  0;
				x3 = -1; y3 =  2;
				x4 =  2; y4 = -1;
				break;
			case eOrientation::DOWN:
				x1 =  2; y1 =  0;
				x2 = -1; y2 =  0;
				x3 =  2; y3 =  1;
				x4 = -1; y4 = -2;
				break;
			case eOrientation::LEFT:
				x1 =  1; y1 =  0;
				x2 = -2; y2 =  0;
				x3 =  1; y3 = -2;
				x4 = -2; y4 =  1;
				break;
			}
		}
		else  //all the other tetrominos share the same kicks
		{
			// Switch on current orientation
			switch (mOrientation)
			{
			case eOrientation::UP:
				x1 = -1; y1 =  0;
				x2 = -1; y2 =  1;
				x3 =  0; y3 = -2;
				x4 = -1; y4 = -2;
				break;
			case eOrientation::RIGHT:
				x1 = 1;	y1 =  0;
				x2 = 1; y2 = -1;
				x3 = 0; y3 =  2;
				x4 = 1; y4 =  2;
				break;
			case eOrientation::DOWN:
				x1 = 1;	y1 =  0;
				x2 = 1; y2 =  1;
				x3 = 0; y3 = -2;
				x4 = 1; y4 = -2;
				break;
			case eOrientation::LEFT:
				x1 = -1; y1 =  0;
				x2 = -1; y2 = -1;
				x3 =  0; y3 =  2;
				x4 = -1; y4 =  2;
				break;
			}
		}
	}
	else if (mNextMove == eTetrominoMoves::ROTATE_LEFT)
	{
		// Type I tetromino ha different kicks
		if (mType == eTetrominoTypes::TETROMINO_I)
		{
			// Switch on current orientation
			switch (mOrientation)
			{
			case eOrientation::UP:
				x1 = -1; y1 =  0;
				x2 =  2; y2 =  0;
				x3 = -1; y3 =  2;
				x4 =  2; y4 = -1;
				break;
			case eOrientation::RIGHT:
				x1 =  2; y1 = 0;
				x2 = -1; y2 = 0;
				x3 =  2; y3 = 1;
				x4 = -1; y4 = 2;
				break;
			case eOrientation::DOWN:
				x1 =  1; y1 =  0;
				x2 = -2; y2 =  0;
				x3 =  1; y3 = -2;
				x4 = -2; y4 =  1;
				break;
			case eOrientation::LEFT:
				x1 = -2; y1 =  0;
				x2 =  1; y2 =  0;
				x3 = -2; y3 = -1;
				x4 =  1; y4 =  2;
				break;
			}
		}
		else //all the other tetrominos share the same kicks
		{
			// Switch on current orientation
			switch (mOrientation)
			{
			case eOrientation::UP:
				x1 = 1;	y1 =  0;
				x2 = 1; y2 =  1;
				x3 = 0; y3 = -2;
				x4 = 1; y4 = -2;
				break;
			case eOrientation::RIGHT:
				x1 = 1;	y1 =  0;
				x2 = 1; y2 = -1;
				x3 = 0; y3 =  2;
				x4 = 1; y4 =  2;
				break;
			case eOrientation::DOWN:
				x1 = -1; y1 =  0;
				x2 = -1; y2 =  1;
				x3 =  0; y3 = -2;
				x4 = -1; y4 = -2;
				break;
			case eOrientation::LEFT:
				x1 = -1; y1 =  0;
				x2 = -1; y2 = -1;
				x3 =  0; y3 =  2;
				x4 = -1; y4 =  2;
				break;
			}
		}
	}

	// Switch on the current Orientation
	translate(x1, y1);
	if (!validPosition(grid))
		translate(-x1, -y1);
	else
		return true;

	translate(x2, y2);
	if (!validPosition(grid))
		translate(-x2, -y2);
	else
		return true;

	translate(x3, y3);
	if (!validPosition(grid))
		translate(-x3, -y3);
	else
		return true;

	translate(x4, y4);
	if (!validPosition(grid))
		translate(-x4, -y4);
	else
		return true;

	//rotation completely failed
	return false;
}

bool clTetromino::validPosition(const clStack& grid)
{
	for (int i = 0; i < mDim; i++)
	{
		for (int j = 0; j < mDim; j++)
		{
			if (mMatrixMoved[i][j]) //if the blocks is part of a tetromino
			{
				// We're trying to move outside the stack (left or right)
				if ( (mX+i) < 0 || (mX+i) >= STACK_WIDTH )
				{
					return false;
				}
				// The teromino is above the stack or has reached the its end
				if ( (mY + j) < 0 || (mY + j) >= STACK_HEIGHT)
				{
					return false;
				}
				// The tetromino overlaps with part of the stack
				if (!grid.isBlockFree(mX + i, mY + j))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void clTetromino::confirmMove()
{
	for (int i = 0; i < mDim; i++)
	{
		for (int j = 0; j < mDim; j++)
		{
			mMatrix[i][j] = mMatrixMoved[i][j];
		}
	}
}

void clTetromino::cancelMove()
{
	for (int i = 0; i < mDim; i++)
	{
		for (int j = 0; j < mDim; j++)
		{
			mMatrixMoved[i][j] = mMatrix[i][j];
		}
	}
}

void clTetromino::addToStack(clStack& stack)
{
	for (int i = 0; i < mDim; i++)
	{
		for (int j = 0; j < mDim; j++)
		{
			if (mMatrix[i][j]) //if the blocks is part of a tetromino
			{
				//move it to the stack
				stack.setBlock(mX + i, mY + j, mColor);
			}
		}
	}
}

void clTetromino::alloc_matrices()
{
	int i, j;

	mMatrix = new bool* [mDim];
	for (i = 0; i < mDim; i++)
	{
		mMatrix[i] = new bool[mDim];
		for (j = 0; j < mDim; j++)
		{
			mMatrix[i][j] = false;
		}
	}

	mMatrixMoved = new bool* [mDim];
	for (i = 0; i < mDim; i++)
	{
		mMatrixMoved[i] = new bool[mDim];
		for (j = 0; j < mDim; j++)
		{
			mMatrixMoved[i][j] = false;
		}
	}
}

void clTetromino::dealloc_matrices()
{
	if (mMatrix != NULL)
	{
		for (int i = 0; i < mDim; i++)
		{
			delete[] mMatrix[i];
		}
		delete[] mMatrix;
		mMatrix = NULL;
	}

	if (mMatrixMoved != NULL)
	{
		for (int i = 0; i < mDim; i++)
		{
			delete[] mMatrixMoved[i];
		}
		delete[] mMatrixMoved;
		mMatrixMoved = NULL;
	}

}

void clTetromino::init_matrices(const int x1, const int y1, const int x2, const int y2,
							    const int x3, const int y3, const int x4, const int y4)
{
	mMatrix[x1][y1] = true;
	mMatrix[x2][y2] = true;
	mMatrix[x3][y3] = true;
	mMatrix[x4][y4] = true;

	mMatrixMoved[x1][y1] = true;
	mMatrixMoved[x2][y2] = true;
	mMatrixMoved[x3][y3] = true;
	mMatrixMoved[x4][y4] = true;
}