#include "stdafx.h"
#include <string>
#include "graphics.h"
#include <SDL.h>
#include <SDL_image.h>


SDL_Window* window;
SDL_Surface* screenSurface;

SDL_Renderer * renderer;
SDL_Surface * image;
SDL_Texture * texture;

using namespace std;

void drawWindow(int width, int height)
{
	bool quit = false;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window * window = SDL_CreateWindow("Safe Browsing",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	// init the renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

}

void clearScreen() {

}

void destroyWindow() {

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	// Quit IMG subsystems 
	IMG_Quit();
	
}

void drawImg(std::string imgPath, int x, int y, int size) {

	image = IMG_Load(imgPath.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	destination.w = size;
	destination.h = size;

	SDL_RenderCopy(renderer, texture, NULL, &destination);
}

void updateScreen() {
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
//	SDL_UpdateWindowSurface(window);
}
