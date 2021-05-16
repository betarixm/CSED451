#ifndef CSED451_STELLAR_H
#define CSED451_STELLAR_H

#include <array>

#include "Shape.h"
#include "Light.h"

using namespace std;

vector<GLclampf*> circleGradient(array<GLclampf, 3>& start, array<GLclampf, 3>& end);

void initStellar();

class Orb {
private:
    Sphere* _planet;
    Orb* _satellite = nullptr;
protected:
public:
    Orb(const char *map_path, const char *normal_path, float x, float y, float z, float length, GLclampf colorfv[]);

    Orb(const char *map_path, const char *normal_path, float x, float y, float z, float length, GLclampf colorfv[], Orb* satellite, float distance);

    void addSatellite(Orb* satellite, float distance);

    GroupNode* groupNode() const;

    Sphere* planet() const;
};

class Stellar {
private:
    GroupNode *_baseScene;
    TranslateNode *_baseTranslate;
    array<Orb*, 3> orbs = {nullptr, };
    array<float, 2> _distance;
    float _x, _y, _z, deg = 0;
    PointLight _light;

public:
    static vector<Stellar*> stellarVec;

    Stellar (const char *map_path[],  float x, float y, float z, array<float, 3>& length, array<float, 2>& distance, vector<GLclampf *> colorfv);

    void tick();

    void display(bool isBlack);
};


#endif //CSED451_STELLAR_H
