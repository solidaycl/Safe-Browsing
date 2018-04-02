// SafeBrowsing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <SDL.h>
using namespace std;

bool gameOver;
const int width = 10;//Width of game world
const int height = 30;//Height of game world
int playerX, playerY, enemyX, enemyY, lives, score, multiplier;

//Used to set a direction for continuous movement (Used in setDirection() method)
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

/*
Draws the game world including the borders,
current player position, and enemy position.
Need to hide cursor so it does not continuously
flash in the game world on every iteration of the 
world draw.
*/
void drawWorld() {
	system("cls");//Clears the console so a new world is drawn every time
	for (int i = 0; i < width+2; i++)
	{
		cout << "#";//Top game border
	}
	cout << endl;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width+2; j++) {
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
			if (j == width) {
				cout << "#";//Right side border
			}
			else {
				cout << " ";//empty space for moving around
			}
		}
		cout << endl;
	}
	for (int i = 0; i < width + 2; i++) {
		cout << "#";//Bottom border
	}
	//Printing score and lives below the game world
	cout << endl;
	cout << "       Score = ";
	cout << score;
	cout << endl;
	cout << "       Lives = ";
	cout << lives;	
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
		case 'a'://Pressing the 'a' key moves the player left one unit
			if (playerX > 1)
				playerX--;
			break;
		case 'd'://Moves right one unit
			if (playerX < width - 1)
				playerX++;
			break;
		case 'w'://Moves up one unit (included for testing purposes)
			if (playerY > 0)
				playerY--;
			break;
		case 's'://Moves down one unit (included for testing purposes)
			if (playerY < height - 1)
				playerY++;
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
*/
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

/*
Controls player movement. 1 button press = moves in that direction continuously.
*/
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

/*
Controls enemy movement. As long as it is not at the bottom
of the screen, it will keep moving down.
*/
void enemyMovement() {
	while (enemyY <= height) {
		enemyY++;
		break;
	}
}

/*
Checks for collision between the enemy and the player and
the enemy and the bottom of the world and adjusts the score
and lives accordingly.  

***Currently respawns the enemy at
the top of the world until a better method for despawning/respawning
is implemented***

*/
void collision() {
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
}

/*
Initializes the game. Set where the player spawns.
Enemy spawns in a random spot at the top of the world.
*/
void startGame() {
	gameOver = false;
	playerX = width / 2;
	playerY = height -1;
	enemyX = rand() % width +1;
	enemyY = 0;
	score = 0;
	multiplier = 0;
	lives = 3;
}

/*
int main()
{
	srand(time(NULL));//Seed for rand() method so a different number is generated
	startGame();
	
	while (!gameOver) {
		drawWorld();
		//COMMENT OUT EITHER playerMovement() or smoothPlayerMovement() based on preference
		playerMovement();
		//smoothPlayerMovement();
		enemyMovement();
		collision();
		if (lives == 0) {
			gameOver = true;
		}
	}
	drawEndGame();
	std::cin.ignore();
    return 0;
}
*/


int main(int argc, char * argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		cout << "SDL initialization succeeded!";
	}

	cin.get();
	return 0;
}