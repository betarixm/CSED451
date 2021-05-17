#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#define NUM_SHADER 2

Shader *shader;
Shader *shaders[NUM_SHADER];
bool shader_index = 0;

void initShader() {
    shaders[0] = new Shader((char *)"shader/Shader.vert", (char *)"shader/Shader.frag");
    shaders[1] = new Shader((char *)"shader/Shader_g.vert", (char *)"shader/Shader_g.frag");

    shader = shaders[0];
    //default = Phong shading
}

Shader::Shader(char *vShaderPath, char *fShaderPath) {
    ifstream vShaderFile, fShaderFile;
    stringstream vShaderStream, fShaderStream;
    string vShaderString, fShaderString;

    int vShader, fShader;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vShaderPath);
        fShaderFile.open(fShaderPath);

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vShaderString = vShaderStream.str();
        fShaderString = fShaderStream.str();
    } catch(const ifstream::failure& e) {
        cout << "[-][Shader] File Read Failed" << endl;
    }

    const char* vShaderCode = vShaderString.c_str();
    const char* fShaderCode = fShaderString.c_str();

    int success;
    char infoLog[512];

    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderCode, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        std::cout << "[-][Shader] Vertex Shader Compilation Failed\n" << infoLog << std::endl;
    }

    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderCode, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        std::cout << "[-][Shader] Fragment Shader Compilation Failed!\n" << infoLog << std::endl;
    };

    _sid = glCreateProgram();
    glAttachShader(_sid, vShader);
    glAttachShader(_sid, fShader);

    glLinkProgram(_sid);
    glGetProgramiv(_sid, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(_sid, 512, NULL, infoLog);
        std::cout << "[-][Shader] Linking Failed!\n" << infoLog << std::endl;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void Shader::use() const {
    glUseProgram(_sid);
}

void Shader::uniform4v(const string &key, float x, float y, float z, float w) const {
    glUniform4f(GET_UNIFORM(_sid, key.c_str()), x, y, z, w);
}

void Shader::uniform4m(const string &key, const glm::mat4 &value) const {
    glUniformMatrix4fv(GET_UNIFORM(_sid, key.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::uniform3v(const string &key, float x, float y, float z) const {
    glUniform3f(GET_UNIFORM(_sid, key.c_str()), x, y, z);
}

void Shader::uniform3v(const string &key, const glm::vec3 &value) const {
    glUniform3fv(GET_UNIFORM(_sid, key.c_str()), 1, glm::value_ptr(value));
}

void Shader::uniform1f(const string &key, float x) const {
    glUniform1f(GET_UNIFORM(_sid, key.c_str()), x);
}

void Shader::uniform1i(const string &key, int x) const {
    glUniform1i(glGetUniformLocation(_sid, key.c_str()), x);
}


