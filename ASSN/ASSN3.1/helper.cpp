#include <string>
#include "helper.h"

glm::vec3 getVector3f(ifstream& f) {
    string a, b, c;
    f >> a >> b >> c;
    glm::vec3 result(stof(a), stof(b), stof(c));
    return result;
}

glm::vec2 getVector2f(ifstream& f) {
    string a, b;
    f >> a >> b;
    glm::vec2 result(stof(a), stof(b));
    return result;
}