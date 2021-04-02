//
// Created by 민재 on 2021/04/02.
//

#include "Stellar.h"

Orb::Orb(float x, float y, float length, array<GLclampf, 3> &colorfv) {
    this->_planet = new Circle(x, y, length, 0.0f, colorfv.data());
}

Orb::Orb(float x, float y, float length, array<GLclampf, 3> &colorfv, Orb *satellite, float distance) {
    this->_planet = new Circle(x, y, length, 0.0f, colorfv.data());
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

Circle *Orb::planet() const {
    return this->_planet;
}

Stellar::Stellar(float x, float y, array<float, 3> &length, array<float, 2> &distance,
                 array<array<GLclampf, 3>, 3> &colorfv) {
    this->_baseScene = new GroupNode;
    this->_baseTranslate = new TranslateNode;

    for(int i = 0; i < 3; i++){
        this->orbs[i] = new Orb(0, 0, length[i], colorfv[i]);
    }

    for(int i = 1; i < 3; i++) {
        this->orbs[2 - i]->addSatellite(this->orbs[2 - i + 1], distance[i - 1]);
    }

    this->_baseTranslate->move(x, y, 1);

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
