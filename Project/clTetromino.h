#pragma once

#include <time.h>

#include "constants.h"
#include "globals.h"
#include "clStack.h"

enum class eTetrominoTypes
{
	TETROMINO_I, //0
	TETROMINO_J, //1
	TETROMINO_L, //2
	TETROMINO_O, //3
	TETROMINO_S, //4
	TETROMINO_Z, //5
	TETROMINO_T, //6
	UNDEFINED,   //7
};
enum class eTetrominoStatus
{
	TETROMINO_WAITING, //0
	TETROMINO_ACTIVE,  //1
	TETROMINO_PLACED,  //2
};
enum class eTetrominoMoves
{
	NO_MOVE,      //0
	MOVE_LEFT,    //1
	MOVE_RIGHT,   //2
	MOVE_DOWN,    //3
	MOVE_BOTTOM,  //4
	ROTATE_LEFT,  //5
	ROTATE_RIGHT, //6
};
enum class eOrientation
{
	UP,     //0 - spawn orientation
	RIGHT,  //1 - after 1 rotation clockwise
	DOWN,   //2 - after 2 rotation from spawning orientation
	LEFT,   //3 - after 1 rotation counterclockwise
};

const SDL_Color COLOR_TETROMINO_I = { 0, 255, 255, 255 }; //cyan
const SDL_Color COLOR_TETROMINO_J = { 0, 0, 255, 255 };   //blue
const SDL_Color COLOR_TETROMINO_L = { 255, 127, 0, 255 }; //orange
const SDL_Color COLOR_TETROMINO_O = { 255, 255, 0, 255 }; //yellow
const SDL_Color COLOR_TETROMINO_S = { 0, 255, 0, 255 };   //green
const SDL_Color COLOR_TETROMINO_Z = { 255, 0, 0, 255 };	  //red
const SDL_Color COLOR_TETROMINO_T = { 128, 0, 128, 255 }; //purple

extern Mix_Chunk* lRotation_sfx;
extern Mix_Chunk* lHardDrop_sfx;

class clTetromino
{
	public:

		clTetromino(eTetrominoTypes type = eTetrominoTypes::UNDEFINED);
		clTetromino(const clTetromino& tetromino);				//copy constructor (for heap data)
		~clTetromino();
		clTetromino& operator=(const clTetromino& tetromino);	//assign operator overload (for heap data)

		//Handling functions
		void handleEvent(SDL_Event& event);
		void move(clStack& stack);
		void render();

		//Get and Set funcion
		bool isPlaced();
		bool activate();
		bool validPosition(const clStack& stack);
		void drop(clStack& stack);
		void addToStack(clStack& stack);
		void freeTetromino();

	private:
		
		int mDim;			  //matrix dimension
		bool** mMatrix;		  //matrix containing the blocks
		bool** mMatrixMoved;  //matrix after the rotation

		int mX; //matrix X position relative to the stack
		int mY; //matrix Y position relative to the stack

		eOrientation     mOrientation;  //Tetromino Orientation
		eTetrominoMoves  mNextMove;		//Tetromino stored Move
		eTetrominoTypes  mType;			//Tetromino Type
		SDL_Color		 mColor;		//Tetromino Color
		eTetrominoStatus mStatus;		//Tetromino Status

		//Tetromino movements
		void translate(int offsetX, int offsetY);
		void tryRotateClockwise(const clStack& stack);
		void tryRotateAntiClockwise(const clStack& stack);
		bool tryKicks(const clStack& grid);
		void confirmMove();
		void cancelMove();

		//Matrices utility functions
		void alloc_matrices();
		void dealloc_matrices();
		void init_matrices(const int x1, const int y1, const int x2, const int y2,
						   const int x3, const int y3, const int x4, const int y4);
};

