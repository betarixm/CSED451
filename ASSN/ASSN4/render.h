#ifndef CSED451_RENDER_H
#define CSED451_RENDER_H

#include <deque>

#include <glm/mat4x4.hpp>

#include "Game.h"
#include "Shape.h"
#include "Ship.h"
#include "Shader.h"
#include "Stellar.h"

void lookAt(float x, float y, int _frontCamera);

void microRenderScene(bool isBlack);

void renderScene();

#endif //CSED451_RENDER_H
