#ifndef CSED451_HELPER_H
#define CSED451_HELPER_H

#include <fstream>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

using namespace std;

glm::vec3 getVector3f(ifstream& f);
glm::vec2 getVector2f(ifstream& f);
vector<float> vectorCompat(const vector<vector<float>>& vertices);
vector<float> vectorCompat(std::vector<std::vector<float>> &vertex, std::vector<std::vector<float>> &normal,
                           std::vector<std::vector<float>> &uv);

#endif //CSED451_HELPER_H
