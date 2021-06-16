//
// Created by 민재 on 2021/04/02.
//

#ifndef CSED451_STELLAR_H
#define CSED451_STELLAR_H

#include <array>

#include "Tree.h"
#include "Shape.h"

using namespace std;

vector<GLclampf*> circleGradient(array<GLclampf, 3>& start, array<GLclampf, 3>& end);
void initStellar();

class Orb {
private:
    Sphere* _planet;
    Orb* _satellite = nullptr;
protected:
public:
    Orb(float x, float y, float z, float length, GLclampf colorfv[]);

    Orb(float x, float y, float z, float length, GLclampf colorfv[], Orb* satellite, float distance);

    void addSatellite(Orb* satellite, float distance);

    GroupNode* groupNode() const;

    Sphere* planet() const;
};

class Stellar {
private:
    GroupNode *_baseScene;
    TranslateNode *_baseTranslate;
    array<Orb*, 3> orbs = {nullptr, };

public:
    Stellar (float x, float y, float z, array<float, 3>& length, array<float, 2>& distance, vector<GLclampf *> colorfv);

    void tick();

    void display(bool isBlack);
};


#endif //CSED451_STELLAR_H
