#ifndef CSED451_KEYBOARD_H
#define CSED451_KEYBOARD_H

#include "Game.h"

void onSpecialKeyDown(int key, int x, int y);

void onSpecialKeyUp(int key, int x, int y);

void onKeyDown(unsigned char key, int x, int y);

void onKeyUp(unsigned char key, int x, int y);

#endif //CSED451_KEYBOARD_H