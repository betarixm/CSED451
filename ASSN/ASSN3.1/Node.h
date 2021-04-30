#ifndef CSED451_NODE_H
#define CSED451_NODE_H

#include <GL/glew.h>
#include <vector>
#include <utility>
#include <stack>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define NUM_COLOR 3

#define RED 0
#define GREEN 1
#define BLUE 2

using namespace std;

extern stack<glm::mat4> ModelView;
extern stack<glm::mat4> Projection;

class Node {
private:
protected:
    Node* _child = nullptr, *_sibling = nullptr;
public:
    Node();

    Node(Node* _child, Node* _sibling);

    virtual void _display(bool isBlack) = 0;

    virtual void display(bool isBlack);

    Node* child() const;

    Node* sibling() const;

    Node* addChild(Node* target);

    Node* addSibling(Node* target);

    virtual ~Node() {
        delete this->_child;
        delete this->_sibling;
    }
};

class GroupNode: public Node {
private:

public:
    GroupNode() = default;

    void _display(bool isBlack) override {}

    virtual void display(bool isBlack) override;

    void addToLast(Node *target);

};

class RotationNode: public Node {
private:
    float _degree = 0;
    float _x = 0, _y = 0, _z = 0;
protected:
public:
    RotationNode() = default;
    explicit RotationNode(float _degree);
    RotationNode(float _degree, float _x, float _y, float _z);

    void _display(bool isBlack) override;

    float degree() const;

    float set(float degree);

    float rotate(float delta);
};

class TranslateNode: public Node {
private:
    float _dx = 0, _dy = 0, _dz = 0;
protected:
public:
    TranslateNode() = default;
    TranslateNode(float _dx, float _dy, float _dz);
    TranslateNode(float _dx, float _dy, float _dz, Node* _child, Node* _sibling);

    void _display(bool isBlack) override;

    void set(float x, float y, float z);

    void move(float dx, float dy, float dz);

    std::vector<float> delta();
};

class VertexNode: public Node {
private:
protected:
    std::vector<std::vector<float>>* _vertices = nullptr;
    GLclampf _colorfv[3]{};
    GLenum _mode = GL_POLYGON;
    float modelview[16]{}; /* 4x4 model view matrix */

public:
    VertexNode() = default;
    explicit VertexNode(std::vector<std::vector<float>>* _vertices, GLenum _mode, GLclampf colorfv[]);
    VertexNode(std::vector<std::vector<float>>* _vertices, GLenum _mode, GLclampf colorfv[], Node* _child, Node* _sibling);

    void _display(bool isBlack) override;

    virtual void set(std::vector<std::vector<float>>* vertices);

    std::vector<std::vector<float>>* vertices();

    void mutateColor(GLclampf dr, GLclampf dg, GLclampf db) {
        this->_colorfv[RED] += dr;
        this->_colorfv[GREEN] += dg;
        this->_colorfv[BLUE] += db;
    }
    GLclampf *color();

    float *modelView();
};



class ScaleNode: public Node {
private:
    float _sx = 1, _sy = 1, _sz = 1;
public:
    ScaleNode() = default;
    ScaleNode(float _sx, float _sy, float _sz);
    ScaleNode(float _sx, float _sy, float _sz, Node* _child, Node* _sibling);
    void _display(bool isBlack) override;
    void set(float x, float y, float z);
    void resizing(float dx, float dy, float dz);
    std::vector<float> delta();

};

#endif //CSED451_NODE_H
