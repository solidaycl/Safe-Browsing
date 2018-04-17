#include "stdafx.h"
#include <string>
#include "graphics.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


SDL_Window* window;
SDL_Surface* screenSurface;
TTF_Font * font;
TTF_Font * wastedFont;

// store the dimensions of the window
int screenWidth;
int screenHeight;

SDL_Renderer * renderer;
SDL_Surface * image;
SDL_Texture * texture;

using namespace std;

// game characters
	// players
	SDL_Texture * texture_Chrome;
	SDL_Texture * texture_Chromium;
	SDL_Texture * texture_IE;
	SDL_Texture * texture_Firefox;
	SDL_Texture * texture_Opera;
	SDL_Texture * texture_Safari;

	// enemies
	SDL_Texture * texture_GreenVirus2;
	SDL_Texture * texture_GreenVirus;
	SDL_Texture * texture_Kaspersky;
	SDL_Texture * texture_Trojan;
	SDL_Texture * texture_Exclamation;

	// powerups 
	SDL_Texture * texture_Patch;
	SDL_Texture * texture_Update;

// sound effects
Mix_Music * powerupSound = NULL;
Mix_Music * enemyCollisionSound = NULL;
Mix_Music * gameOverSound = NULL;


// loads all images required by the game and 
// stores them as global fields
void loadImages() {
	SDL_Surface * img;

	/** load enemies **/
	// load the green virus
	img = IMG_Load("Resources/GreenVirus.png");
	texture_GreenVirus = SDL_CreateTextureFromSurface(renderer, img);
	   
	// load the green virus 2
	img = IMG_Load("Resources/GreenVirus2.png");
	texture_GreenVirus2 = SDL_CreateTextureFromSurface(renderer, img);

	// load the trojan 
	img = IMG_Load("Resources/Trojan.png");
	texture_Trojan = SDL_CreateTextureFromSurface(renderer, img);

	// load the Kaspersky 
	img = IMG_Load("Resources/Kaspersky.png");
	texture_Kaspersky = SDL_CreateTextureFromSurface(renderer, img);


	/** load powerups **/
	// load the Update 
	img = IMG_Load("Resources/Update.png");
	texture_Update = SDL_CreateTextureFromSurface(renderer, img);

	// load the Patch 
	img = IMG_Load("Resources/Patch.png");
	texture_Patch = SDL_CreateTextureFromSurface(renderer, img);

	/** load player characters **/
	// load chrome
	img = IMG_Load("Resources/Chrome.png");
	texture_Chrome = SDL_CreateTextureFromSurface(renderer, img);

	// load the Chromium 
	img = IMG_Load("Resources/Chromium.png");
	texture_Chromium = SDL_CreateTextureFromSurface(renderer, img);
	
	// load Firefox 
	img = IMG_Load("Resources/Firefox.png");
	texture_Firefox = SDL_CreateTextureFromSurface(renderer, img);
	
	// load IE
	img = IMG_Load("Resources/IE.png");
	texture_IE = SDL_CreateTextureFromSurface(renderer, img);

	// load IE
	img = IMG_Load("Resources/Opera.png");
	texture_Opera = SDL_CreateTextureFromSurface(renderer, img);

	// load Safari
	img = IMG_Load("Resources/Safari.png");
	texture_Safari = SDL_CreateTextureFromSurface(renderer, img);

	// load Exclamation for enemy collision
	img = IMG_Load("Resources/Exclamation.png");
	texture_Exclamation = SDL_CreateTextureFromSurface(renderer, img);

	SDL_FreeSurface(img);
}

// loads all game sound effects
void loadSoundEffects() {
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	powerupSound = Mix_LoadMUS("Resources/PowerUp.mp3");
	enemyCollisionSound = Mix_LoadMUS("Resources/EnemyCollision.mp3");
	gameOverSound = Mix_LoadMUS("Resources/GameOver.mp3");
}

void drawWindow(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	bool quit = false;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window * window = SDL_CreateWindow("Safe Browsing",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	// init the renderer
	renderer = SDL_CreateRenderer(window, -1, 0); 
	
	// set up the font
	TTF_Init();
	// load score and lives font
	font = TTF_OpenFont("Resources/Xlines.ttf", 100);
	// laod the GTA V pricedown font
	wastedFont = TTF_OpenFont("Resources/Wasted.ttf", 1000);
	
	// load game images 
	loadImages();
	
	// load sound effects
	loadSoundEffects();

}

// plays the power up sound
void playPowerupSound() {
	Mix_PlayMusic(powerupSound, 0);
}

void playEnemyCollisionSound() {
	Mix_PlayMusic(enemyCollisionSound, 0);
}

void playGameOverSound() {
	Mix_PlayMusic(gameOverSound, 0);
}

void displayEndGame() {
	SDL_Color color = { 255, 0, 0};

	string msg = "Wasted";

	SDL_Surface * surface = TTF_RenderText_Solid(wastedFont,
		msg.c_str(), color);
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect destination;
	destination.x = 0;
	destination.y = 0;
	destination.w = 150;
	destination.h = 40;

	SDL_RenderCopy(renderer, textTexture, NULL, /*&destination*/NULL);
	
	// clean up memory that was created
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(surface);

	// draw to the screen
	SDL_RenderPresent(renderer);
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

void drawPlayerSelectMessage() {
	SDL_Color color = { 255, 255, 255 };

	string msg1 = "Choose your browser by entering a number.";

	SDL_Surface * surface1 = TTF_RenderText_Solid(font,
		msg1.c_str(), color);
	SDL_Texture * textTexture1 = SDL_CreateTextureFromSurface(renderer, surface1);

	SDL_Rect destination1;
	destination1.x = 0;
	destination1.y = 250;
	destination1.w = screenWidth;
	destination1.h = 40;

	string msg2 = "1";

	SDL_Surface * surface2 = TTF_RenderText_Solid(font,
		msg2.c_str(), color);
	SDL_Texture * textTexture2 = SDL_CreateTextureFromSurface(renderer, surface2);

	SDL_Rect destination2;
	destination2.x = (screenWidth / 7) *1;
	destination2.y = (screenHeight / 2) + 60;
	destination2.w = 30;
	destination2.h = 50;

	string msg3 = "2";

	SDL_Surface * surface3 = TTF_RenderText_Solid(font,
		msg3.c_str(), color);
	SDL_Texture * textTexture3 = SDL_CreateTextureFromSurface(renderer, surface3);

	SDL_Rect destination3;
	destination3.x = (screenWidth / 7) * 2;
	destination3.y = (screenHeight / 2) + 60;
	destination3.w = 30;
	destination3.h = 50;

	string msg4 = "3";

	SDL_Surface * surface4 = TTF_RenderText_Solid(font,
		msg4.c_str(), color);
	SDL_Texture * textTexture4 = SDL_CreateTextureFromSurface(renderer, surface4);

	SDL_Rect destination4;
	destination4.x = (screenWidth / 7) * 3;
	destination4.y = (screenHeight / 2) + 60;
	destination4.w = 30;
	destination4.h = 50;

	string msg5 = "4";

	SDL_Surface * surface5 = TTF_RenderText_Solid(font,
		msg5.c_str(), color);
	SDL_Texture * textTexture5 = SDL_CreateTextureFromSurface(renderer, surface5);

	SDL_Rect destination5;
	destination5.x = (screenWidth / 7) * 4;
	destination5.y = (screenHeight / 2) + 60;
	destination5.w = 30;
	destination5.h = 50;

	string msg6 = "5";

	SDL_Surface * surface6 = TTF_RenderText_Solid(font,
		msg6.c_str(), color);
	SDL_Texture * textTexture6 = SDL_CreateTextureFromSurface(renderer, surface6);

	SDL_Rect destination6;
	destination6.x = (screenWidth / 7) * 5;
	destination6.y = (screenHeight / 2) + 60;
	destination6.w = 30;
	destination6.h = 50;

	string msg7 = "6";

	SDL_Surface * surface7 = TTF_RenderText_Solid(font,
		msg7.c_str(), color);
	SDL_Texture * textTexture7 = SDL_CreateTextureFromSurface(renderer, surface7);

	SDL_Rect destination7;
	destination7.x = (screenWidth / 7) * 6;
	destination7.y = (screenHeight / 2) + 60;
	destination7.w = 30;
	destination7.h = 50;

	SDL_RenderCopy(renderer, textTexture1, NULL, &destination1);
	SDL_RenderCopy(renderer, textTexture2, NULL, &destination2);
	SDL_RenderCopy(renderer, textTexture3, NULL, &destination3);
	SDL_RenderCopy(renderer, textTexture4, NULL, &destination4);
	SDL_RenderCopy(renderer, textTexture5, NULL, &destination5);
	SDL_RenderCopy(renderer, textTexture6, NULL, &destination6);
	SDL_RenderCopy(renderer, textTexture7, NULL, &destination7);

	SDL_DestroyTexture(textTexture1);
	SDL_DestroyTexture(textTexture2);
	SDL_DestroyTexture(textTexture3);
	SDL_DestroyTexture(textTexture4);
	SDL_DestroyTexture(textTexture5);
	SDL_DestroyTexture(textTexture6);
	SDL_DestroyTexture(textTexture7);

	SDL_FreeSurface(surface1);
	SDL_FreeSurface(surface2);
	SDL_FreeSurface(surface3);
	SDL_FreeSurface(surface4);
	SDL_FreeSurface(surface5);
	SDL_FreeSurface(surface6);
	SDL_FreeSurface(surface7);

}

void drawImg(std::string imgPath, int x, int y, int size) {

	SDL_Rect destination;
	destination.x = x;
	destination.y = y;
	destination.w = size;
	destination.h = size;
	
	// find the correct texture to render 
	if (!imgPath.compare("GreenVirus.png")) {
	SDL_RenderCopy(renderer, texture_GreenVirus, NULL, &destination);
	}
	else if (!imgPath.compare("Trojan.png")) {
	SDL_RenderCopy(renderer, texture_Trojan, NULL, &destination);
	}
	else if (!imgPath.compare("Kaspersky.png")) {
	SDL_RenderCopy(renderer, texture_Kaspersky, NULL, &destination);
	}
	else if (!imgPath.compare("GreenVirus2.png")) {
	SDL_RenderCopy(renderer, texture_GreenVirus2, NULL, &destination);
	}
	else if (!imgPath.compare("Update.png")) {
	SDL_RenderCopy(renderer, texture_Update, NULL, &destination);
	}
	else if (!imgPath.compare("Patch.png")) {
	SDL_RenderCopy(renderer, texture_Patch, NULL, &destination);
	}
	else if (!imgPath.compare("Chrome.png")) {
	SDL_RenderCopy(renderer, texture_Chrome, NULL, &destination);
	}
	else if (!imgPath.compare("Chromium.png")) {
	SDL_RenderCopy(renderer, texture_Chromium, NULL, &destination);
	}
	else if (!imgPath.compare("Firefox.png")) {
	SDL_RenderCopy(renderer, texture_Firefox, NULL, &destination);
	}
	else if (!imgPath.compare("IE.png")) {
	SDL_RenderCopy(renderer, texture_IE, NULL, &destination);
	}
	else if (!imgPath.compare("Opera.png")) {
	SDL_RenderCopy(renderer, texture_Opera, NULL, &destination);
	}
	else if (!imgPath.compare("Safari.png")) {
		SDL_RenderCopy(renderer, texture_Safari, NULL, &destination);
	}
	else if (!imgPath.compare("Exclamation.png")) {
		SDL_RenderCopy(renderer, texture_Exclamation, NULL, &destination);
	}
}

void updateScreen() {
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
	SDL_UpdateWindowSurface(window);
}

void drawExclamation(int x, int y) {
	drawImg("Exclamation.png", x, y, 50);
	SDL_RenderPresent(renderer);
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
