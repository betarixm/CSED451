#include "Light.h"
vector<DirectionalLight *> DirectionalLight::list = {};
vector<PointLight *> PointLight::list = {};

void initLight() {
    auto dirLight = new DirectionalLight(glm::vec3(RADIUS_DIR_LIGHT, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
}

Light::Light(string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : _name(std::move(name)), _ambient(ambient), _diffuse(diffuse), _specular(specular) {}

void Light::_use(Shader *shader) {
    shader->uniform3v(_name + ".ambient", _ambient);
    shader->uniform3v(_name + ".diffuse", _diffuse);
    shader->uniform3v(_name + ".specular", _specular);
}

void Light::use(Shader *shader) {
    _use(shader);
}

void Light::setAmbient(glm::vec3 ambient) {
    _ambient = ambient;
}

void Light::setDiffuse(glm::vec3 diffuse) {
    _diffuse = diffuse;
}

void Light::setSpecular(glm::vec3 specular) {
    _specular = specular;
}

string Light::name() {
    return _name;
}

glm::vec3 Light::ambient() {
    return _ambient;
}

glm::vec3 Light::diffuse() {
    return _diffuse;
}

glm::vec3 Light::specular() {
    return _specular;
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : Light(string("directionalLight[") + to_string(list.size()) + string("]"),
                ambient, diffuse, specular), _direction(direction) {
    list.push_back(this);
}

void DirectionalLight::use(Shader *shader) {
    _use(shader);
    shader->uniform3v(_name + ".direction", _direction);
}

void DirectionalLight::setDirection(glm::vec3 direction) {
    _direction = direction;
}

glm::vec3 DirectionalLight::direction() {
    return _direction;
}

PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 ambient,
                       glm::vec3 diffuse, glm::vec3 specular)
        : Light(string("pointLight[") + to_string(list.size()) + string("]"),
                ambient, diffuse, specular), _position(position), _constant(constant), _linear(linear),
          _quadratic(quadratic) {
    list.push_back(this);
}

void PointLight::use(Shader *shader) {
    _use(shader);
    shader->uniform3v(_name + ".position", _position);
    shader->uniform1f(_name + ".constant", _constant);
    shader->uniform1f(_name + ".linear", _linear);
    shader->uniform1f(_name + ".quadratic", _quadratic);
}

glm::vec3 PointLight::position() {
    return _position;
}

float PointLight::constant() const {
    return _constant;
}

float PointLight::linear() const {
    return _linear;
}

float PointLight::quadratic() const {
    return _quadratic;
}

void PointLight::setPosition(glm::vec3 position) {
    _position = position;
}

void PointLight::setConstant(float constant) {
    _constant = constant;
}

void PointLight::setLinear(float linear) {
    _linear = linear;
}

void PointLight::setQuadratic(float quadratic) {
    _quadratic = quadratic;
}
