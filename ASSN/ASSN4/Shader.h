#ifndef CSED451_SHADER_H
#define CSED451_SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/mat4x4.hpp>

#define GET_UNIFORM(ID, KEY) glGetUniformLocation(ID, KEY)

using namespace std;

class Shader {
private:
    GLuint _sid;
public:
    Shader(char *vShaderPath, char *fShaderPath);

    void use() const;

    void uniform4v(const string &key, float x, float y, float z, float w) const;

    void uniform4m(const string &key, const glm::mat4 &value) const;

    void uniform3v(const string &key, float x, float y, float z) const;

    void uniform3v(const string &key, const glm::vec3 &value) const;

    void uniform1f(const string &key, float x) const;

    void uniform1i(const string &key, int x) const;
};

void initShader();

#endif //CSED451_SHADER_H
