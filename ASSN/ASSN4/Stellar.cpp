#include "Stellar.h"

vector<Stellar*> Stellar::stellarVec = {};

Orb::Orb(float x, float y, float z, float length, GLclampf colorfv[]) {
    this->_planet = new Sphere(20, 20, length, x, y, z, 0, colorfv);
}

Orb::Orb(float x, float y, float z, float length, GLclampf colorfv[], Orb *satellite, float distance) {
    this->_planet = new Sphere(20, 20, length, x, y, z, 0, colorfv);
    this->addSatellite(satellite, distance);
}

void Orb::addSatellite(Orb *satellite, float distance) {
    this->_satellite = satellite;

    if(this->_satellite) {
        this->_planet->groupNode()->addToLast(satellite->groupNode());
        this->_satellite->_planet->move(distance, 0);
    }
}

GroupNode *Orb::groupNode() const {
    return this->planet()->groupNode();
}

Sphere *Orb::planet() const {
    return this->_planet;
}

Stellar::Stellar(float x, float y, float z, array<float, 3> &length, array<float, 2> &distance, vector<GLclampf *> colorfv)
    : _x(x), _y(y), _z(z), _distance(distance), _light(glm::vec3(x + distance[0] + distance[1], y, z + 0.3f), 1.0f, 0.09f, 0.2f, glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(0.8f, 0.8f, 0.8f)) {
    this->_baseScene = new GroupNode;
    this->_baseTranslate = new TranslateNode;

    for(int i = 0; i < 3; i++){
        this->orbs[i] = new Orb(0, 0, 0, length[i], colorfv[i]);
    }

    for(int i = 1; i < 3; i++) {
        this->orbs[2 - i]->addSatellite(this->orbs[2 - i + 1], distance[2 - i]);
    }

    this->_baseTranslate->move(x, y, z);

    this->_baseScene->addChild(this->_baseTranslate);
    this->_baseTranslate->addChild(this->orbs[0]->groupNode());
}

void Stellar::tick() {
    for(int i = 0; i < 3; i++){
        this->orbs[i]->planet()->rotate(0.5f);
    }

    deg += 0.5f;
    if(deg > 360) { deg -= 360; }

    float rad = glm::radians(deg);

    _light.setPosition(glm::vec3(_x + cos(rad) * _distance[0] + cos(2*rad) * _distance[1], _y + sin(rad) * _distance[0] + sin(2*rad) * _distance[1], _z));
}

void Stellar::display(bool isBlack) {
    this->_baseScene->display(isBlack);
}

vector<GLclampf*> circleGradient(array<GLclampf, 3>& start, array<GLclampf, 3>& end) {
    float HALF_CIRCLE = 180;
    vector<GLclampf*> halfVec, resultVec;

    for(int i = 0; i < (int)HALF_CIRCLE; i++) {
        auto tmp = new array<GLclampf, 3>();
        for(int j = 0; j < 3; j++) {
            (*tmp)[j] = (end[j] - start[j]) / HALF_CIRCLE * (float)i + start[j];
        }
        halfVec.push_back(tmp->data());
    }

    resultVec = halfVec;

    for(auto i = halfVec.rbegin(); i != halfVec.rend(); i++){
        resultVec.push_back(*i);
    }

    return resultVec;
}

void initStellar() {
    int i;

    array<GLclampf, 3> red = {1, 0, 0};
    array<GLclampf, 3> green = {0, 1, 0};
    array<GLclampf, 3> blue = {0, 0, 1};
    array<GLclampf, 3> white = {1, 1, 1};
    array<GLclampf, 3> yellow = {1, 1, 0};
    array<GLclampf, 3> orange = {1, 0.5, 0};
    array<GLclampf, 3> grey = {0.5, 0.5, 0.5};
    array<GLclampf, 3> darkGrey = {0.15, 0.15, 0.15};
    array<GLclampf, 3> purple = {0.5, 0, 1};

    array<array<float, 3>, 2> length = {
            array<float, 3>{0.15, 0.07, 0.03},
            array<float, 3>{0.2, 0.10, 0.05},
    };

    array<array<float, 2>, 2> distance = {
            array<float, 2>{0.4, 0.15},
            array<float, 2>{0.5, 0.2}
    };

    vector<vector<GLclampf *>> color{
            {&red[0], &green[0], &blue[0]},
            {&yellow[0], &orange[0], &purple[0]}
    };


    for(i = 0; i < 2; i++){
        Stellar::stellarVec.push_back(new Stellar(-0.61f + (float)i, -0.5f + (float)i, 0.25f * (float)i, length[i], distance[i], color[i]));
    }
}