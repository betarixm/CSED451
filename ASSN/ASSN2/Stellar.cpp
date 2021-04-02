//
// Created by 민재 on 2021/04/02.
//

#include "Stellar.h"

extern vector<Stellar*> stellar_vec;

Orb::Orb(float x, float y, float length, vector<GLclampf*> &colorfv) {
    this->_planet = new GradientCircle(x, y, length, 0.0f, colorfv);
}

Orb::Orb(float x, float y, float length, vector<GLclampf*> &colorfv, Orb *satellite, float distance) {
    this->_planet = new GradientCircle(x, y, length, 0.0f, colorfv);
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

GradientCircle *Orb::planet() const {
    return this->_planet;
}

Stellar::Stellar(float x, float y, array<float, 3> &length, array<float, 2> &distance,
                 array<vector<GLclampf*>, 3> &colorfv) {
    this->_baseScene = new GroupNode;
    this->_baseTranslate = new TranslateNode;

    for(int i = 0; i < 3; i++){
        this->orbs[i] = new Orb(0, 0, length[i], colorfv[i]);
    }

    for(int i = 1; i < 3; i++) {
        this->orbs[2 - i]->addSatellite(this->orbs[2 - i + 1], distance[2 - i]);
    }

    this->_baseTranslate->move(x, y, 0.2);

    this->_baseScene->addChild(this->_baseTranslate);
    this->_baseTranslate->addChild(this->orbs[0]->groupNode());
}

void Stellar::tick() {
    for(int i = 0; i < 3; i++){
        this->orbs[i]->planet()->rotate(1.0f);
    }
}

void Stellar::display() {
    this->_baseScene->display();
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

    array<array<vector<GLclampf*>, 3>, 2> colorfv = {
            array<vector<GLclampf*>, 3>{circleGradient(red, yellow), circleGradient(green, blue), circleGradient(grey, darkGrey)},
            array<vector<GLclampf*>, 3>{circleGradient(blue, white), circleGradient(orange, darkGrey), circleGradient(grey, purple)}
    };

    for(int i = 0; i < 2; i++){
        stellar_vec.push_back(new Stellar(-0.61f + (float)i, -0.5f + (float)i, length[i], distance[i], colorfv[i]));
    }
}