//
// Created by 최은수 on 2021/03/04.
//
#include<iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <GL/glut.h>
#include <list>

#include "Shape.h"
#include <random>
#include <cmath>

using namespace std;

#ifndef CSED451_SHIP_H
#define CSED451_SHIP_H

#endif //CSED451_SHIP_H

#define MAX_WINDOW 1.0

typedef Square Bullet;

/* list<Bullet*> Enemy_bullets, Player_bullets */

class Ship {
protected:
    int _numLife;
    Triangle glObject;

public:
    Ship(int _numLife, float x, float y, float length, GLclampf r, GLclampf g, GLclampf b, float degree): glObject(x, y, length, r, g, b)
    {
        this->_numLife = _numLife;
        this->glObject.setDegree(degree);
    }
    ~Ship(){};
    bool hit(Bullet*); /* check whether ship hits bullet */
    Bullet* shot(); /* make bullet instance */
    float dotOverline(vector<float> dot, vector<float> A1, vector<float>A2);
    vector<vector<float>> getPosition();
    void display();
    void display(float rotateDeg);

    int numLife() const{
        return this->_numLife;
    }

};

class Player : public Ship {
public:
    Player(int _numLife, float x, float y, float length, GLclampf r, GLclampf g, GLclampf b, float degree): Ship(_numLife, x, y, length, r, g, b, degree){};
    void checkHit(list<Bullet*>*);
    Bullet* keyHandler(char key);
};


class Enemy : public Ship {
public:
    Enemy(int _numLife, float x, float y, float length, GLclampf r, GLclampf g, GLclampf b, float degree): Ship(_numLife, x, y, length, r, g, b, degree){};
    void randomMoveHandler();
    void checkHit(list<Bullet*>*);

};



