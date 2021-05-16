#ifndef CSED451_SHIP_H
#define CSED451_SHIP_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <GL/glut.h>
#include <list>
#include <cstdlib>
#include <ctime>

#include "Shape.h"
#include "Game.h"

#include <random>
#include <cmath>
#include <array>

using namespace std;

typedef Sphere Bullet;
typedef Sphere Item;

class Ship {
private:
    char _path[20] = "model/ship.obj";
    const char *_map_path = "texture/ship.jpeg";
    const char *_bullet_path = "texture/bullet.jpeg";
protected:
    int _numLife;
    int _numBullet;
    float _size_torso;
    GroupNode *_baseScene; /* Group Node */
    Object _obj;

public:
    Ship(int _numLife, float x, float y, float size_torso, GLclampf r, GLclampf g, GLclampf b, float degree,
         int numBullet = 1);

    ~Ship() = default;

    void display(bool isBlack);

    bool hit(Bullet *);

/* check whether ship hits bullet */

    list<Bullet *> shot();

/* make bullet instance */

    float x() {
        return _obj.x();
    }

    float y() {
        return _obj.y();
    }

    int numLife() const {
        return this->_numLife;
    }

    void mutateColor(GLclampf r, GLclampf g, GLclampf b);

};

class Player : public Ship {
private:
    std::array<bool, 0xff> inputKey = {false,};
public:
    Player(int _numLife, float x, float y, float size_torso, GLclampf r, GLclampf g, GLclampf b, float degree)
            : Ship(_numLife, x, y, size_torso, r, g, b, degree) {};

    void HitBullet(list<Bullet *> *);

    void HitItem(list<Item *> *);


    list<Bullet *> keyHandler();

    list<Bullet *> keyDown(unsigned char key) {
        if (!(0 <= key && key < 0xff)) {
            return {nullptr};
        }

        this->inputKey[key] = true;
        return this->keyHandler();
    }

    void keyUp(unsigned char key) {
        if (!(0 <= key && key < 0xff)) {
            return;
        }
        this->inputKey[key] = false;
        this->keyHandler();
    }
};


class Enemy : public Ship {
public:
    Enemy(int _numLife, float x, float y, float length, GLclampf r, GLclampf g, GLclampf b, float degree,
          int numBullet = 1) : Ship(_numLife, x, y, length, r, g, b, degree, numBullet) {};

    void randomMoveHandler();

    void checkHit(list<Bullet *> *);

};

#endif //CSED451_SHIP_H
