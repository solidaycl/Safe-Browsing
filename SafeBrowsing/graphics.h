#pragma once

extern void drawWindow(int width, int height);
extern void destroyWindow();
extern void drawImg(std::string imgPath, int x, int y, int size);
extern void updateScreen();
extern void drawLives(int lives);
extern void drawScore(int score);