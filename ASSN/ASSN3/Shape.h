#ifndef CSED451_ASSN1_SHAPE_H
#define CSED451_ASSN1_SHAPE_H

#include "Node.h"
#include "Model.h"
#include <GL/glew.h>
#include <vector>

#define NUM_COLOR 3

#define R 0
#define G 1
#define B 2

#define SQRT_3 1.732f
#define PI 3.141592

class Shape {
private:

protected:
    TranslateNode * _translation{};
    RotationNode * _rotation{};
    VertexNode * _vertex{};
    GroupNode * _group{}; /* Shape Group Node */

    std::vector<std::vector<float>> _modelFrame;

public:
    Shape() = default;

    Shape(float x, float y, float z, float deg, GLenum mode, GLclampf r, GLclampf g, GLclampf b);

    Shape(float x, float y, float z, float deg, GLenum mode, GLclampf colorfv[]);

    Shape(float x, float y, float z, float deg, float x_r, float y_r, float z_r, GLenum mode, GLclampf r, GLclampf g,
          GLclampf b);

    void move(float dx, float dy);

    void rotate(float ddeg);

    void setDegree(float deg);

    void mutateColor(GLclampf dr, GLclampf dg, GLclampf db) {
        this->_vertex->mutateColor(dr, dg, db);
    }

    void mutateColor(const GLclampf dColorfv[]);

    float x() const;

    float y() const;

    virtual void setVertex(std::vector<std::vector<float>> *mat);

    GLclampf *color();

    float degree() const;

    void display();

    void pivot_rotate(float dx, float dy, float dz);

    GroupNode* groupNode();

    std::vector<std::vector<float>> getPosition();
};

class Triangle : public Shape {
public:
    Triangle(float x, float y, float length, float deg, GLclampf r, GLclampf g, GLclampf b);

    Triangle(float x, float y, float length, float deg, GLclampf colorfv[]);

    Triangle(float x, float y, float length, float x_r, float y_r, float z_r, float deg, GLclampf r, GLclampf g, GLclampf b);
};

class Square : public Shape {
private:
    float _width, _height;
public:
    Square(float x, float y, float width, float height, float deg, GLclampf r, GLclampf g, GLclampf b);

    Square(float x, float y, float width, float height, float deg, GLclampf colorfv[]);

    Square(float x, float y, float width, float height, float x_r, float y_r, float z_r, float deg, GLclampf r, GLclampf g, GLclampf b);

    float getWidth();

    float getHeight();

};

class Circle : public Shape {
public:
    Circle(float x, float y, float length, float deg, GLclampf r, GLclampf g, GLclampf b);

    Circle(float x, float y, float length, float deg, GLclampf colorfv[]);

    Circle(float x, float y, float length, float x_r, float y_r, float z_r, float deg, GLclampf r, GLclampf g, GLclampf b);

};

class Object : public Shape {
private:
    Model _model;
public:
    Object(char* path, float x, float y, float z, float deg, GLclampf r, GLclampf g, GLclampf b);

    Object(char* path, float x, float y, float z, float deg, GLclampf colorfv[]);

    Object(char* path, float x, float y, float z, float x_r, float y_r, float z_r, float deg, GLclampf r, GLclampf g, GLclampf b);
};
#endif // CSED451_ASSN1_SHAPE_H