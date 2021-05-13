#ifndef CSED451_TIMER_H
#define CSED451_TIMER_H

#include "Game.h"
#include "Ship.h"
#include "Stellar.h"

extern Game *game;

extern list<Bullet *> enemy_bullets;
extern list<Bullet *> player_bullets;
extern list<Item *> item_list;

extern char mode;

void timerBulletMoveHit(int value);

void timerBulletEnemyShot(int value);

void timerDefault(int value);

void timerRedisplay(int value);

void timerStellar(int value);

#endif //CSED451_TIMER_H
