#include "Game.h"

extern Game *game;

extern char mode;
extern bool isHiddenLineRemoval;

void onSpecialKeyDown(int key, int x, int y);

void onSpecialKeyUp(int key, int x, int y);

void onKeyDown(unsigned char key, int x, int y);

void onKeyUp(unsigned char key, int x, int y);