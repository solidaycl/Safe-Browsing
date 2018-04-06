// SafeBrowsing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <math.h>
#include <SDL.h>
#include "graphics.h"
#include <stdio.h>
using namespace std;


// we dont have bools in assembly, so here we can define true and false using ints
//const int TRUE = 1;
//const int FALSE = 0;

// delcare some object types for the sinking objects
const int TYPE_ENEMY = 0; 
const int TYPE_POWERUP = 1;

// define the size of all objects on the screen
const int OBJ_SIZE = 50;

// define the margin between the rows 
const int ROW_MARGIN = 15;

// define the margin between the rows 
const int OBJ_MARGIN = 15;

// define the speed that the player moves
const int PLAYER_SPEED = 5;

// define the speed that the rows move downward
const int ROW_SPEED = 3;

// the number of rows of sinking objects
const int NUM_ROWS = 10;

// the amount of leeway to give to collisions
const int COLLISION_ERROR_MARGIN = 10;

// number of possible enemies or powerups that could appear in each row
// should be an even number
const int NUM_OBJS_PER_ROW = 10;

// define the screen sizes
int SCREEN_WIDTH = (OBJ_SIZE + OBJ_MARGIN) * (NUM_OBJS_PER_ROW/1.5);
int SCREEN_HEIGHT = (OBJ_SIZE + ROW_MARGIN) * NUM_ROWS;



// represents all other game objects aside from the player. This includes both powerups and enemies
struct SinkingObj {
	// path to img, could be malware, windowsUpdateImg, etc
	string imgPath;

	// a bool.. but since we dont have bools its an int
	// 0 means invisible and collision detection and rendering should not happen
	// 1 means visible and collision detection and rendering should happen
	int isVisible;

	// objType should be used to determine if the sinking Game object is a powerup or enemy
	// powerup = 1
	// enemy = 0
	int objType;

	// the position of the player in pixels. That is, the position of the center of the graphic in the view
	int xPos;
	// the amount of pixels this object is offset from the row
	int yOffset;
};

// holds the sinking game objects in a row for easy movement and destruction
struct Row {

	// the set possible places for the sinkingObject to spawn
	SinkingObj sinkingObjs[NUM_OBJS_PER_ROW] = {};

	// the positin of the row in pixels.
	int xOffset;
	// how far down the screen the row is.
	int yPos;
};

struct Player {
	// path to img, could be chrome, firefox, etc (all behave the same)
	string imgPath;
	// the positin of the player in pixels. That is, the position of the center of the graphic in the view
	int xPos;
	int yPos;
};

Player player;


// declare the rows that hold the sinking game objects
static Row rows[NUM_ROWS] = {};

// a "bool" of whether the game is over or not really an int
int gameOver;

int lives, score, multiplier;

// the difficulty, increases as the game goes on and determines the likelyhood of enemies spawning
int difficulty = 0;

/*
Controls player movement. One button press = 1 unit movement
*/


int horizontal = 0;
int vertical = 0;

void playerMovement() {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_a://Pressing the 'a' key moves the player left player speed
						horizontal = -1;
					break;
				case SDLK_d://Moves right player speed
						horizontal = 1;
					break;
				case SDLK_w://Moves up player speed
						vertical = -1;
					break;
				case SDLK_s://Moves down player speed
						vertical = 1;
					break;
				case 'q'://Ends the game
					gameOver = true;
					break;
			}
		}
		if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
				case SDLK_a:
					horizontal = 0;
					break;
				case SDLK_d:
					horizontal = 0;
					break;
				case SDLK_w:
					vertical = 0; 
					break;
				case SDLK_s: 
					vertical = 0; 
					break;
			}
		}
	}

	// set the player movement
	player.xPos += horizontal * PLAYER_SPEED;
	player.yPos += vertical * PLAYER_SPEED;
	
	// if the player has gone out of bounds, put them back in bounds
	if (player.xPos < OBJ_SIZE / 2)
		player.xPos = OBJ_SIZE / 2;
	if (player.xPos > SCREEN_WIDTH - OBJ_SIZE / 2)
		player.xPos = SCREEN_WIDTH - (OBJ_SIZE / 2);
	if (player.yPos < OBJ_SIZE / 2)
		player.yPos = OBJ_SIZE / 2;
	if (player.yPos > SCREEN_HEIGHT - OBJ_SIZE / 2)
		player.yPos = SCREEN_HEIGHT - (OBJ_SIZE / 2);
    /*
	if (_kbhit())//_kbhit() listens for any key being entered
	{
		switch (_getch())//_getch() captures which key is pressed
		{
		case 'a'://Pressing the 'a' key moves the player left player speed
			if (player.xPos > OBJ_SIZE / 2)
				player.xPos -= PLAYER_SPEED;
			break;
		case 'd'://Moves right player speed
			if (player.xPos < SCREEN_WIDTH - OBJ_SIZE / 2)
				player.xPos += PLAYER_SPEED;
			break;
		case 'w'://Moves up player speed (included for testing purposes)
			if (player.yPos > OBJ_SIZE / 2)
				player.yPos -= PLAYER_SPEED;
			break;
		case 's'://Moves down player speed (included for testing purposes)
			if (player.yPos < SCREEN_HEIGHT - OBJ_SIZE / 2)
				player.yPos += PLAYER_SPEED;
			break;
		case 'q'://Ends the game
			gameOver = true;
			break;
		}
	}
    */
}


/*
Checks for collision between the enemy and the player and
the enemy and the bottom of the world and adjusts the score
and lives accordingly.
*/
void detectCollisions() {
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_OBJS_PER_ROW; j++) {

			// only detect collisions on visible objects
			if (rows[i].sinkingObjs[j].isVisible == TRUE) {

				// implement the distance formula to determine if there was a collision or not
				int tempX = rows[i].sinkingObjs[j].xPos + rows[i].xOffset;
				int tempY = rows[i].yPos + rows[i].sinkingObjs[j].yOffset;
				
				int distX = player.xPos - tempX;
				int distY = player.yPos - tempY;
			
				// this might be quite difficult to implement in assembly...
				int distance = (int)sqrt((distX * distX) + (distY * distY));
				
				if (distance < OBJ_SIZE-COLLISION_ERROR_MARGIN) {
				    // a collision happened! 
					// set the object to disappear
					rows[i].sinkingObjs[j].isVisible = FALSE;
					if (rows[i].sinkingObjs[j].objType == TYPE_ENEMY) {
						lives--;
					}
					else if (rows[i].sinkingObjs[j].objType == TYPE_POWERUP) {
						lives++;
					}
				}
			}
		}

	}

}

/*
Populates a row with enemies and powerups
sets the position to the top of the screen
The higher global int difficulty, the more enemies that are likely to spawn
*/
void resetRow(int rowIndex) {



	// TODO: use the global called difficulty in spawning algorithm
	rows[rowIndex].xOffset = 0; // offset the row, this should be calculated using perlin noise in the future
	rows[rowIndex].yPos = 0;
	for (int i = 0; i < NUM_OBJS_PER_ROW; i++) {
		int randomInt = rand() % 100 + 1;
		if (randomInt <= 20) {
			rows[rowIndex].sinkingObjs[i].imgPath = "GreenVirus2.png";
			rows[rowIndex].sinkingObjs[i].isVisible = TRUE;
			rows[rowIndex].sinkingObjs[i].objType = TYPE_ENEMY;
			rows[rowIndex].sinkingObjs[i].xPos = i * (OBJ_SIZE + OBJ_MARGIN);
		}
		else {
			rows[rowIndex].sinkingObjs[i].isVisible = FALSE;
		}
	}
	rows[rowIndex].sinkingObjs[NUM_OBJS_PER_ROW / 2].isVisible = FALSE; // set the middle row to be empty so there is always a passage
}

/*
* Controls enemy and powerup movement. When a row reaches the bottom
* of the screen, it will call a method that sends it will call resetRow.
*/
void moveRowsDown() {
	for (int i = 0; i < NUM_ROWS; i++) {
		rows[i].yPos += ROW_SPEED;
		
		if (rows[i].yPos >= SCREEN_HEIGHT) {
			resetRow(i);
		}
	}
	// for each row... move the row down
	// if row height is more than screen height, move back to top
}

/*
Initializes the game. Set where the player spawns.
Enemy spawns in a random spot at the top of the world.
*/
void initGameWorld() {

	gameOver = FALSE;

	// init the player
	player = {};
	player.imgPath = "Chrome.png";
	player.xPos = SCREEN_WIDTH / 2;
	player.yPos = SCREEN_HEIGHT / 2;

	// init the rows
	for (int i = 0; i < NUM_ROWS; i++) {
		rows[i] = Row{};
		resetRow(i);
		// sets the rows to be way above the screen at the start.
		rows[i].yPos = -1 * i * (OBJ_SIZE + ROW_MARGIN); // this might reverse the rows and could cause issues... needs testing
	}

	score = 0;
	multiplier = 0;
	lives = 3;
}

void drawWorld() {
	// draw the player 
	drawImg(player.imgPath, player.xPos, player.yPos, OBJ_SIZE);
	
	// loop over each enemies or powerup
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_OBJS_PER_ROW; j++) {
			// check if object is visible and should be drawn
			if (rows[i].sinkingObjs[j].isVisible == TRUE) {
				int tempX = rows[i].sinkingObjs[j].xPos + rows[i].xOffset;
				int tempY = rows[i].yPos + rows[i].sinkingObjs[j].yOffset;
				    // draw the enemy or powerup
				drawImg(rows[i].sinkingObjs[j].imgPath, tempX, tempY, OBJ_SIZE);
			}
		}
	}
}

int main(int argc, char* args[])
{
	
	// set the seed for this game
	srand(static_cast <unsigned int> (time(NULL)));

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	drawWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	initGameWorld();

	while (gameOver == FALSE) {
		frameStart = SDL_GetTicks(); 
		playerMovement();
		moveRowsDown();
		detectCollisions();
		drawWorld();
		if (lives == 0) {
		//	gameOver = TRUE; temporarily disable game end for testing
		}
		updateScreen();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	destroyWindow();
	std::cin.ignore();
	return 0;
}