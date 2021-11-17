#pragma once

#include "constants.h"
#include "globals.h"

using namespace std;

class clTexture
{
	public:
		//Constructor
		clTexture();
		//Destructor
		~clTexture();

		//Free memory
		void freeTexture();

		//Gets texture dimensions
		int getWidth();
		int getHeight();

		//Create a texture from an image file
		bool createFromFile(SDL_Renderer* renderer, string filename);
		//Create a texture from a rendered text
		bool createFromText(SDL_Renderer* renderer, TTF_Font* font, string text, SDL_Color textColor);

		//Set color modulation
		void setColorModulation(Uint8 red, Uint8 green, Uint8 blue);
		//Set blend mode
		void setBlendMode(SDL_BlendMode blend);
		//Set alpha modulation
		void setAlphaModulation(Uint8 alpha);

		//Renders the texture with specified parameters
		void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double rot_angle = 0.0,
			SDL_Point* rot_axis_point = NULL, SDL_RendererFlip flip_status = SDL_FLIP_NONE);

	private:
		//The texture in memory
		SDL_Texture* mTexture;

		//Texture dimensions
		int mWidth;
		int mHeight;
};