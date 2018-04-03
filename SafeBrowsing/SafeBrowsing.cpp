// SafeBrowsing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <SDL.h>
using namespace std;


// we dont have bools in assembly, so here we can define true and false using ints
//const int TRUE = 1;
//const int FALSE = 0;


// declare the size of all objects on the screen
const int OBJ_SIZE = 50;

// define the speed that the player moves
const int PLAYER_SPEED = 5;

// define the speed that the rows move downward
const int ROW_SPEED = 5;

// number of possible enemies or powerups that could appear in each row
// should be an even number
const int NUM_OBJS_PER_ROW = 10;

// define the screen sizes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



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

// the number of rows of sinking objects
const int NUM_ROWS = 10;


// declare the rows that hold the sinking game objects
static Row rows[NUM_ROWS] = {};

// a "bool" of whether the game is over or not really an int
int gameOver;

int lives, score, multiplier;

/*
Draws the game world including the borders,
current player position, and enemy position.
Need to hide cursor so it does not continuously
flash in the game world on every iteration of the
world draw.
*/
void drawWorld() {
	/*
	system("cls");//Clears the console so a new world is drawn every time
	for (int i = 0; i < SCREEN_WIDTH+2; i++)
	{
	cout << "#";//Top game border
	}
	cout << endl;
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
	for (int j = 0; j < SCREEN_WIDTH+2; j++) {
	if (j == 0) {
	cout << "#";//Left side border
	}
	else if (i == playerY && j == playerX) {
	cout << "O";//This is the player
	j++;
	}
	else if (i == enemyY && j == enemyX) {
	cout << "V";//The enemy
	j++;
	}
	if (j == SCREEN_WIDTH) {
	cout << "#";//Right side border
	}
	else {
	cout << " ";//empty space for moving around
	}
	}
	cout << endl;
	}
	for (int i = 0; i < SCREEN_WIDTH + 2; i++) {
	cout << "#";//Bottom border
	}
	//Printing score and lives below the game world
	cout << endl;
	cout << " Score = ";
	cout << score;
	cout << endl;
	cout << " Lives = ";
	cout << lives;
	*/
}

/*
Called when lives reach 0, displays final score.
*/
void drawEndGame() {
	system("cls");//clears console window
	cout << "GAME OVER!" << endl << "Score : " << score << endl << "Press enter to quit.";
}

/*
Controls player movement. One button press = 1 unit movement
*/
void playerMovement() {
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
}

/*
This is a helper method for the smoothPlayerMovement() method.
uses _kbhit() and _getch() to set a direction
*//*
void setDirection() {
if (_kbhit()) {
switch (_getch()) {
case 'a':
dir = LEFT;
break;
case 'd':
dir = RIGHT;
break;
case 'w':
dir = UP;
break;
case 's':
dir = DOWN;
break;
case ' ':
dir = STOP;
break;
case 'q'://Ends the game
gameOver = true;
break;
}
}
}
*/

/*
Controls player movement. 1 button press = moves in that direction continuously.
*//*
void smoothPlayerMovement() {
setDirection();
switch (dir) {
case LEFT:
if (playerX > 1) {
playerX--;
break;
}
case RIGHT:
if (playerX < width-1)
playerX++;
break;
case UP:
if (playerY > 0)
playerY--;
break;
case DOWN:
if (playerY < height-1)
playerY++;
break;
case STOP:
break;
}
}
*/


/*
Checks for collision between the enemy and the player and
the enemy and the bottom of the world and adjusts the score
and lives accordingly.

***Currently respawns the enemy at
the top of the world until a better method for despawning/respawning
is implemented***

*/
void detectCollisions() {
	/*
	// compare every game object in every row with te player


	....
	if (enemyY == height) {
	score += 10;
	enemyY = 0;
	enemyX = rand() % width;
	}
	if (enemyX == playerX && enemyY == playerY) {
	lives--;
	enemyY = 0;
	enemyX = rand() % width;
	}
	...
	*/
}

/*
Populates a row with enemies and powerups
sets the position to the top of the screen
The higher the difficulty, the more enemies that are likely to spawn
*/
void resetRow(int rowIndex, int difficulty) {
	rows[rowIndex].xOffset = 0;
	rows[rowIndex].yPos = 0;
	//rows[rowIndex].sinkingObjs = {};

}

/*
* Controls enemy and powerup movement. When a row reaches the bottom
* of the screen, it will call a method that sends it will call resetRow.
*/
void moveRowsDown() {
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
	Player player = {};
	player.imgPath = "/img/chrome.png";
	player.xPos = SCREEN_WIDTH / 2;
	player.yPos = SCREEN_HEIGHT / 2;

	// init the rows
	for (int i = 0; i < NUM_ROWS; i++) {
		rows[i] = Row{};
		resetRow(i, 0);
		rows[i].xOffset = 0;


	}

	score = 0;
	multiplier = 0;
	lives = 3;
}



int main(int argc, char* args[])
{
	// srand(time(NULL));//Seed for rand() method so a different number is generated
	initGameWorld();

	while (gameOver == FALSE) {
		//TODO: start timer for game loop here

		drawWorld();
		//COMMENT OUT EITHER playerMovement() or smoothPlayerMovement() based on preference
		playerMovement();
		//smoothPlayerMovement();
		moveRowsDown();
		detectCollisions();
		if (lives == 0) {
			gameOver = TRUE;
		}

		//TODO: check timer and sleep for target fps
	}
	drawEndGame();
	std::cin.ignore();
	return 0;
}

/*
int main(int argc, char* args[])
{
//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

//Initialize SDL
if (SDL_Init(SDL_INIT_VIDEO) < 0)
{
printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
}
else
{
//Create window
window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

//Destroy window
SDL_DestroyWindow(window);

//Quit SDL subsystems
SDL_Quit();

return 0;
}
*/


