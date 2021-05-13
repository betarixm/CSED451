#ifndef CSED451_RENDER_H
#define CSED451_RENDER_H

#include <deque>

#include <glm/mat4x4.hpp>

#include "Game.h"
#include "Shape.h"
#include "Ship.h"
#include "Shader.h"
#include "Stellar.h"

extern Game* game;
extern list<Bullet*> enemy_bullets;
extern list<Bullet*> player_bullets;
extern list<Item*> item_list;

extern Grid* grid;
extern Grid* boundary;

extern bool isHiddenLineRemoval;

extern Shader* shader;

void lookAt(float x, float y, int _frontCamera);

void microRenderScene(bool isBlack);

void renderScene();

#endif //CSED451_RENDER_H
