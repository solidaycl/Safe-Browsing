#include "stdafx.h"
#include "graphics.h"
#include <SDL.h>
#include <SDL_image.h>

SDL_Window* window;
SDL_Surface* screenSurface;

using namespace std;

void drawWindow(int width, int height)
{
	//The window we'll be rendering to
	window = NULL;

	//The surface contained by the window
	screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("Safe Browsing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			//Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			//Update the surface
			SDL_UpdateWindowSurface(window);

			//Wait 12 seconds
			SDL_Delay(12000);
		}
	}

}

void clearScreen() {

}

void drawGameObjects() {

}

void destroyWindow() {
	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
}


void drawImg(string imgPath, int x, int y, int size) {

	SDL_Surface* surface = IMG_Load(img);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	destination.w = size;
	destination.w = size;

	//SD_RenderCopy(renderer, texture, NULL, &destination);
}