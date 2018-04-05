#include "stdafx.h"
#include <string>
#include "graphics.h"
#include <SDL.h>
#include <SDL_image.h>


SDL_Window* window;
SDL_Surface* screenSurface;

using namespace std;

void drawWindow(int width, int height)
{
	bool quit = false;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Surface * image = IMG_Load("Chrome.png");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);

	while (!quit)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		//SDL_Rect dstrect = { 5, 5, 320, 240 };
		//SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	IMG_Quit();
}

void clearScreen() {

}

void destroyWindow() {
	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
}

void drawImg(std::string imgPath, int x, int y, int size) {
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Surface* surface = IMG_Load(imgPath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	destination.w = size;
	destination.h = size;

	SDL_RenderCopy(renderer, texture, NULL, &destination);
	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);
}