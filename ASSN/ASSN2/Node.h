#ifndef CSED451_NODE_H
#define CSED451_NODE_H

#include <GL/glew.h>
#include <vector>
#include <utility>

#define NUM_COLOR 3

#define R 0
#define G 1
#define B 2

using namespace std;

class Node {
private:
protected:
    Node* _child = nullptr, *_sibling = nullptr;
public:
    Node();

    Node(Node* _child, Node* _sibling);

    virtual void _display() = 0;

    virtual void display();

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

    void _display() override {}

    virtual void display() override;

    void addToLast(Node *target);

};

class RotationNode: public Node {
private:
    float _degree = 0;
    float _x, _y, _z;
protected:
public:
    RotationNode() = default;
    explicit RotationNode(float _degree);
    RotationNode(float _degree, float _x, float _y, float _z);
    RotationNode(float _degree, Node* _child, Node* _sibling);

    void _display() override;

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

    void _display() override;

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
    float modelview[16]; /* 4x4 model view matrix */

public:
    VertexNode() = default;
    explicit VertexNode(std::vector<std::vector<float>>* _vertices, GLenum _mode, GLclampf colorfv[]);
    VertexNode(std::vector<std::vector<float>>* _vertices, GLenum _mode, GLclampf colorfv[], Node* _child, Node* _sibling);

    void _display() override;

    virtual void set(std::vector<std::vector<float>>* vertices);

    std::vector<std::vector<float>>* vertices();

    void mutateColor(GLclampf dr, GLclampf dg, GLclampf db) {
        this->_colorfv[R] += dr;
        this->_colorfv[G] += dg;
        this->_colorfv[B] += db;
    }
    GLclampf *color();

    float *modelView();
};

class GradientVertexNode: public VertexNode {
private:
    vector<GLclampf*> _colorfv;
public:
    GradientVertexNode() = default;
    GradientVertexNode(std::vector<std::vector<float>>* _vertices, GLenum _mode, vector<GLclampf*>& colorfv);;

    void _display() override;

    void set(std::vector<std::vector<float>> *vertices) override;

};

#endif //CSED451_NODE_H
