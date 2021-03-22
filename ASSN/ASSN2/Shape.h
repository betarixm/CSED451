#ifndef CSED451_ASSN1_SHAPE_H
#define CSED451_ASSN1_SHAPE_H

#include <GL/glew.h>
#include <vector>

#define NUM_COLOR 3

#define R 0
#define G 1
#define B 2

#define SQRT_3 1.732f

class Shape {
private:
    float _x, _y, _length, _degree = 0;
    GLclampf _colorfv[3]{};

protected:
    void _display(float rotateDeg, std::vector<std::vector<float>> &mat, GLenum mode);

public:
    Shape(float x, float y, float length, GLclampf r, GLclampf g, GLclampf b);

    Shape(float x, float y, float length, const GLclampf colorfv[]);

    void move(float dx, float dy);

    void rotate(float ddeg);

    void setDegree(float deg);

    void mutateColor(GLclampf dr, GLclampf dg, GLclampf db) {
        this->_colorfv[R] += dr;
        this->_colorfv[G] += dg;
        this->_colorfv[B] += db;
    }

    void mutateColor(const GLclampf dColorfv[]);

    float x() const;

    float y() const;

    float length() const;

    GLclampf *color();

    float degree() const;
};

class Triangle : public Shape {
public:
    Triangle(float x, float y, float length, GLclampf r, GLclampf g, GLclampf b);

    Triangle(float x, float y, float length, const GLclampf colorfv[]);

    void display();

    void display(float rotateDeg);
};

class Square : public Shape {
public:
    Square(float x, float y, float length, GLclampf r, GLclampf g, GLclampf b);

    Square(float x, float y, float length, const GLclampf colorfv[]);

    void display();

    void display(float rotateDeg);
};

class Circle : public Shape {
public:
    Circle(float x, float y, float length, GLclampf r, GLclampf g, GLclampf b) : Shape(x, y, length, r, g, b) {};

    Circle(float x, float y, float length, const GLclampf colorfv[]) : Shape(x, y, length, colorfv) {};

    void display();

    void display(float rotateDeg);
};
#endif // CSED451_ASSN1_SHAPE_H