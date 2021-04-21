#ifndef CSED451_MODEL_H
#define CSED451_MODEL_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "helper.h"

class Model {
private:
    vector<unsigned int> _vertexIdx, _uvIdx, _normalIdx;
    vector<glm::vec3> _vertex;
    vector<glm::vec2> _uv;
    vector<glm::vec3> _normal;
    vector<glm::vec3> _vertexBuf;
    vector<glm::vec2> _uvBuf;
    vector<glm::vec3> _normalBuf;

    GLenum _mode = GL_TRIANGLES;

public:
    explicit Model(char* path);

    void display();

    void display(GLenum mode);

};


#endif //CSED451_MODEL_H
