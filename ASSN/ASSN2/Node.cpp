#include "Node.h"

Node::Node(Node *_child, Node *_sibling) : _child(_child), _sibling(_sibling) {}

void Node::display() {
    glPushMatrix();

    this->_display();

    if(this->_child != nullptr) {
        this->_child->display();
    }

    glPopMatrix();

    if(this->_sibling != nullptr) {
        this->_sibling->display();
    }
}

Node *Node::child() const {
    return this->_child;
}

Node *Node::sibling() const {
    return this->_sibling;
}

Node *Node::addChild(Node *target) {
    Node* curSibling = this->_sibling;
    for(; curSibling != nullptr; curSibling = curSibling->_sibling) {
        if(curSibling->_sibling == nullptr){
            curSibling->_sibling = target;
            break;
        }
    }
    return curSibling;
}

Node *Node::addSibling(Node *target) {
    Node* prevSibling = this->_sibling;
    this->_sibling = target;
    return prevSibling;
}

void RotationNode::_display() {
    glRotatef(this->_degree, 0.0f, 0.0f, 1.0f);
}

RotationNode::RotationNode(float _degree) : _degree(_degree) {}

RotationNode::RotationNode(float _degree, Node *_child, Node *_sibling) : _degree(_degree), Node(_child, _sibling) {}

float RotationNode::degree() const {
    return this->_degree;
}

float RotationNode::set(float degree) {
    float prevDegree = this->_degree;
    this->_degree = degree;
    return prevDegree;
}

float RotationNode::rotate(float delta) {
    float prevDegree = this->_degree;
    this->_degree += delta;
    return prevDegree;
}

void TranslateNode::_display() {
    glTranslatef(this->_dx, this->_dy, this->_dz);
}

TranslateNode::TranslateNode(float _dx, float _dy, float _dz) : _dx(_dx), _dy(_dy), _dz(_dz) {}

TranslateNode::TranslateNode(float _dx, float _dy, float _dz, Node *_child, Node *_sibling) : _dx(_dx), _dy(_dy), _dz(_dz), Node(_child, _sibling) {}

void TranslateNode::set(float x, float y, float z) {
    this->_dx = x;
    this->_dy = y;
    this->_dz = z;
}

void TranslateNode::move(float dx, float dy, float dz) {
    this->_dx += dx;
    this->_dy += dy;
    this->_dz += dz;
}

void VertexNode::_display() {
    if(this->_vertices == nullptr) {
        return;
    }

    glBegin(this->_mode);
    for(auto & _vertex : *this->_vertices) {
        if(_vertex.size() == 2){
            glVertex2f(_vertex[0], _vertex[1]);
        } else if (_vertex.size() == 3) {
            glVertex3f(_vertex[0], _vertex[1], _vertex[2]);
        }
    }
    glEnd();
}

VertexNode::VertexNode(std::vector<std::vector<float>> *_vertices, GLenum _mode) : _vertices(_vertices), _mode(_mode) {}

VertexNode::VertexNode(std::vector<std::vector<float>> *_vertices, GLenum _mode, Node *_child, Node *_sibling) : _vertices(_vertices), _mode(_mode), Node(_child, _sibling) {}

void VertexNode::set(std::vector<std::vector<float>> *vertices) {
    this->_vertices = vertices;
}
