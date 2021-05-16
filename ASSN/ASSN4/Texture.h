//
// Created by 최은수 on 2021/05/16.
//

#ifndef CSED451_TEXTURE_H
#define CSED451_TEXTURE_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/mat4x4.hpp>

#include "stb_image.h"


class Texture {
private:
    GLuint _tid;

public:
    Texture(const char *path);

    GLuint getTid()
    {
        return _tid;
    }
};


#endif //CSED451_TEXTURE_H
