#ifndef CSED451_HELPER_H
#define CSED451_HELPER_H

#include <fstream>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

using namespace std;

glm::vec3 getVector3f(ifstream& f);
glm::vec2 getVector2f(ifstream& f);

#endif //CSED451_HELPER_H
