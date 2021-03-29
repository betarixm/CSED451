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
#include <array>

using namespace std;

#ifndef CSED451_SHIP_H
#define CSED451_SHIP_H

#endif //CSED451_SHIP_H

#define MAX_WINDOW 1.0

typedef Square Bullet;
typedef Square Item;

#define MAX_ROTATION 45


class Ship {
protected:
    int _numLife;
    int _numBullet;
    float _size_torso;
    GroupNode *_baseScene; /* Group Node */
    Triangle _head;
    Square _torso, _lwing, _rwing, _lcanon, _rcanon;

    float _rotateDir;

public:
    Ship(int _numLife, float x, float y, float size_torso, GLclampf r, GLclampf g, GLclampf b, float degree);

    ~Ship() {};

    void display();


    bool hit(Bullet*);
/* check whether ship hits bullet */

    list<Bullet*> shot();
/* make bullet instance */

    float dotOverline(vector<float> dot, vector<float> A1, vector<float>A2);
    vector<vector<float>> getPosition();

    int numLife() const{
        return this->_numLife;
    }

    void mutateColor(GLclampf r, GLclampf g, GLclampf b);

    void wingMove();

};

class Player : public Ship {
private:
    std::array<bool, 0xff> inputKey = {false, };
public:
    Player(int _numLife, float x, float y, float size_torso, GLclampf r, GLclampf g, GLclampf b, float degree)
    : Ship(_numLife, x, y, size_torso, r, g, b, degree){};

    void HitBullet(list<Bullet*>*);
    void HitItem(list<Item*>*);


    list<Bullet *>keyHandler();

    list<Bullet *> keyDown(unsigned char key){
        if(!(0 <= key && key < 0xff)){
            return {nullptr};
        }

        this->inputKey[key] = true;
        return this->keyHandler();
    }

    void keyUp(unsigned char key){
        if(!(0 <= key && key < 0xff)){
            return;
        }
        this->inputKey[key] = false;
        this->keyHandler();
    }
};


class Enemy : public Ship {
public:
    Enemy(int _numLife, float x, float y, float length, GLclampf r, GLclampf g, GLclampf b, float degree): Ship(_numLife, x, y, length, r, g, b, degree){};
    void randomMoveHandler();
    void checkHit(list<Bullet*>*);

};




