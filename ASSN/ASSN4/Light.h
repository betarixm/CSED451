#ifndef CSED451_LIGHT_H
#define CSED451_LIGHT_H

#include <string>
#include <vector>
#include <utility>

#include <glm/vec3.hpp>

#include "Shader.h"

class Light {
private:
    glm::vec3 _ambient{};
    glm::vec3 _diffuse{};
    glm::vec3 _specular{};

protected:
    string _name;

    void _display(Shader &shader);

public:
    Light(string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    virtual void use(Shader &shader);

    void setAmbient(glm::vec3 ambient);

    void setDiffuse(glm::vec3 diffuse);

    void setSpecular(glm::vec3 specular);

    string name();

    glm::vec3 ambient();

    glm::vec3 diffuse();

    glm::vec3 specular();
};

class DirectionalLight : private Light {
private:
    glm::vec3 _direction{};
public:
    static vector<DirectionalLight *> list;

    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void use(Shader& shader) override;

    void setDirection(glm::vec3 direction);

    glm::vec3 direction();
};

class PointLight : private Light {
private:
    glm::vec3 _position{};

    float _constant;
    float _linear;
    float _quadratic;

public:
    static vector<PointLight*> list;

    PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    void use(Shader& shader) override;

    glm::vec3 position();

    float constant() const;

    float linear() const;

    float quadratic() const;

    void setPosition(glm::vec3 position);

    void setConstant(float constant);

    void setLinear(float linear);

    void setQuadratic(float quadratic);
};


#endif //CSED451_LIGHT_H
