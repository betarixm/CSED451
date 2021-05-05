#ifndef CSED451_ASSN1_SHAPE_H
#define CSED451_ASSN1_SHAPE_H

#include "Node.h"
#include "Model.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <vector>

#define NUM_COLOR 3

#define RED 0
#define GREEN 1
#define BLUE 2

#define SQRT_3 1.732f
#define PI 3.141592

class Shape {
private:
    std::vector<float> _direction = {0, 0, 0};

protected:
    TranslateNode * _translation{};
    RotationNode * _rotation{};
    ScaleNode * _scale{};
    VertexNode * _vertex{};
    GroupNode * _group{}; /* Shape Group Node */

    std::vector<std::vector<float>> _modelFrame;

public:
    Shape() = default;

    Shape(float x, float y, float z, float deg, float sx, float sy, float sz,  GLenum mode, GLclampf r, GLclampf g, GLclampf b);

    Shape(float x, float y, float z, float deg, float sx, float sy, float sz, GLenum mode, GLclampf colorfv[]);


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

    void display(bool isBlack);

    void pivot_rotate(float dx, float dy, float dz);

    GroupNode* groupNode();

    std::vector<std::vector<float>> getPosition(int num_pos=1);

    void setDirection(float x, float y, float z);

    std::vector<float> direction();

};





class Object : public Shape {
private:
    std::vector<std::vector<float>> vertices;
    GLuint buffer = 0;
    Model _model;
public:
    Object(char* path, float x, float y, float z, float deg, GLclampf r, GLclampf g, GLclampf b);

    Object(char* path, float x, float y, float z, float deg, GLclampf colorfv[]);

    glm::vec3 max();
    glm::vec3 min();

};

class Grid : public Shape {
private:
    std::vector<std::vector<float>> vertices;
    GLuint buffer;

public:
    Grid(float width, float height, int row, int col, float x, float y, float z, float deg, GLclampf r, GLclampf g, GLclampf b);
};


class Sphere : public Shape {
private:
    static std::vector<std::vector<float>> vertices;
    static GLuint buffer; //VBO

public:
    Sphere(int lat, int lon, float radius, float x, float y, float z, float deg, GLclampf colorfv[]);

    Sphere(int lat, int lon, float radius, float x, float y, float z, float deg, GLclampf r, GLclampf g, GLclampf b);

    static void init();
};





#endif // CSED451_ASSN1_SHAPE_H