#include "stdafx.h"
#include <string>
#include "graphics.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


SDL_Window* window;
SDL_Surface* screenSurface;
TTF_Font * font;

// store the dimensions of the window
int width;
int height;

SDL_Renderer * renderer;
SDL_Surface * image;
SDL_Texture * texture;

using namespace std;

SDL_Texture * texture_chrome;
SDL_Texture * texture_GreenVirus2;


void drawWindow(int width, int height)
{
	bool quit = false;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window * window = SDL_CreateWindow("Safe Browsing",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	// init the renderer
	renderer = SDL_CreateRenderer(window, -1, 0); 
	
	// set up the font
	TTF_Init();
	font = TTF_OpenFont("Resources/Xlines.ttf", 100);
	
	// load chrome
	SDL_Surface * img1 = IMG_Load("Resources/Chrome.png");
	texture_chrome = SDL_CreateTextureFromSurface(renderer, img1);
	SDL_FreeSurface(img1);

	// load the green virus 2
	SDL_Surface * img2 = IMG_Load("Resources/GreenVirus2.png");
	texture_GreenVirus2 = SDL_CreateTextureFromSurface(renderer, img2);
	SDL_FreeSurface(img2);

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

	// Quit TTF
	TTF_Quit();
}

// draws the number of lives left at the top of the screen
void drawLives(int lives) {
	SDL_Color color = { 255, 255, 255 };

	string msg = "Lives: " + std::to_string(lives);

	SDL_Surface * surface = TTF_RenderText_Solid(font,
		msg.c_str(), color);
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect destination;
	destination.x = 0;
	destination.y = 50;
	destination.w = 150; // the width of the screen
	destination.h = 40;

	SDL_RenderCopy(renderer, textTexture, NULL, &destination);
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(surface);
}

void drawScore(int score) {
	SDL_Color color = { 255, 255, 255 };

	// calculate the number of digits to determine how wide the bounding rectangle should be
	int fontWidth = 20; 
	int number = score;
	int digits = 0; while (number != 0) { number /= 10; digits++; }
	fontWidth *= digits;

	string msg = "Score: " + std::to_string(score);

	SDL_Surface * surface = TTF_RenderText_Solid(font,
		msg.c_str(), color);
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect destination;
	destination.x = 0;
	destination.y = 0;
	destination.w = 150 + fontWidth; // add the font width for each digit 
	destination.h = 40;

	SDL_RenderCopy(renderer, textTexture, NULL, &destination);
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(surface);
}

void drawImg(std::string imgPath, int x, int y, int size) {

	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	destination.w = size;
	destination.h = size;
	
	if (!imgPath.compare("Chrome.png")) {
	SDL_RenderCopy(renderer, texture_chrome, NULL, &destination);
	}
	else if (!imgPath.compare("GreenVirus2.png")) {
	SDL_RenderCopy(renderer, texture_GreenVirus2, NULL, &destination);
	}

}

void updateScreen() {
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
	SDL_UpdateWindowSurface(window);
}
