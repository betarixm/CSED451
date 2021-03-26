#ifndef CSED451_NODE_H
#define CSED451_NODE_H

#include <GL/glew.h>
#include <vector>

class Node {
private:
    Node* _child = nullptr, *_sibling = nullptr;
    virtual void _display() = 0;
protected:
public:
    Node() = default;

    Node(Node* _child, Node* _sibling);;

    void display();

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
    void _display() override {}
public:
    GroupNode() = default;
};

class RotationNode: public Node {
private:
    float _degree = 0;
    void _display() override;
protected:
public:
    RotationNode() = default;
    explicit RotationNode(float _degree);
    RotationNode(float _degree, Node* _child, Node* _sibling);

    float degree() const;

    float set(float degree);

    float rotate(float delta);
};

class TranslateNode: public Node {
private:
    float _dx = 0, _dy = 0, _dz = 0;
    void _display() override;
protected:
public:
    TranslateNode() = default;
    TranslateNode(float _dx, float _dy, float _dz);
    TranslateNode(float _dx, float _dy, float _dz, Node* _child, Node* _sibling);

    void set(float x, float y, float z);

    void move(float dx, float dy, float dz);
};

class VertexNode: public Node {
private:
    std::vector<std::vector<float>>* _vertices = nullptr;
    GLenum _mode = GL_POLYGON;

    void _display() override;

protected:
public:
    VertexNode() = default;
    explicit VertexNode(std::vector<std::vector<float>>* _vertices, GLenum _mode);
    VertexNode(std::vector<std::vector<float>>* _vertices, GLenum _mode, Node* _child, Node* _sibling);

    void set(std::vector<std::vector<float>>* vertices);
};

#endif //CSED451_NODE_H
