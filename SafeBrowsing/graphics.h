#pragma once

extern void drawWindow(int width, int height);
extern void destroyWindow();
extern void drawImg(std::string imgPath, int x, int y, int size);
extern void updateScreen();
extern void drawExclamation(int x, int y);
extern void drawLives(int lives);
extern void drawScore(int score);
extern void drawPlayerSelectMessage();

// functions for sound effects defined in the graphics.cpp file
extern void playPowerupSound();
extern void playEnemyCollisionSound();
extern void playGameOverSound();

extern void displayEndGame();