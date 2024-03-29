#include "Node.h"
#include <iostream>
using namespace std;

Node::Node() : _child(nullptr), _sibling(nullptr){}
Node::Node(Node *_child, Node *_sibling) : _child(_child), _sibling(_sibling) {}

void Node::display() {
    glPushMatrix();

    this->_display();

    if(this->_child != nullptr) {
        this->_child->display();
    }

    if(this->_sibling != nullptr) {
        this->_sibling->display();
    }

    glPopMatrix();
}

Node *Node::child() const {
    return this->_child;
}

Node *Node::sibling() const {
    return this->_sibling;
}

Node *Node::addChild(Node *target) {
    Node *curChild = this->_child;
    this->_child = target;
    if (!target)
        target->addChild(curChild);

    return curChild;
}

Node *Node::addSibling(Node *target) {
    if (target == nullptr)
        return target;

    Node* prevSibling = this->_sibling;

    this->_sibling = target;
    target->addSibling(prevSibling);

    return prevSibling;
}

void GroupNode::display()
{
    glPushMatrix();

    if(this->_child != nullptr) {
        this->_child->display();
    }

    glPopMatrix();

    if(this->_sibling != nullptr) {
        this->_sibling->display();
    }
}

void GroupNode::addToLast(Node *target)
{
    Node *cur = this->_child;
    if (!cur)
    {
        this->_child = target;
    }
    else
    {
    for(; cur->sibling() != nullptr; cur=cur->sibling());
    cur->addChild(target);
    }
}


void RotationNode::_display() {
    glRotatef(this->_degree, _x, _y, _z);
}

RotationNode::RotationNode(float _degree) : _x(0), _y(0), _z(1.0), _degree(_degree) {}
RotationNode::RotationNode(float _degree, float _x, float _y, float _z) : _x(_x), _y(_y), _z(_z), _degree(_degree) {}

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

std::vector<float> TranslateNode::delta() {
    return std::vector<float>{_dx, _dy, _dz};
}

void VertexNode::_display() {
    if(this->_vertices == nullptr) {
        return;
    }

    glPushMatrix();
    glColor3fv(this->color());

    glBegin(this->_mode);
    for(auto & _vertex : *this->_vertices) {
        if(_vertex.size() == 2){
            glVertex2f(_vertex[0], _vertex[1]);
        } else if (_vertex.size() == 3) {
            glVertex3f(_vertex[0], _vertex[1], _vertex[2]);
        }
    }
    glEnd();
    glPopMatrix();

    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
}

VertexNode::VertexNode(std::vector<std::vector<float>> *_vertices, GLenum _mode, GLclampf colorfv[])
            : _vertices(_vertices), _mode(_mode)
            {
                if(colorfv) {
                    std::memcpy(this->_colorfv, colorfv, NUM_COLOR * sizeof(GLclampf));
                }
                for (int i = 0; i < 4; i++){
                    for (int j = 0; j < 4; j++)
                    {
                        modelview[i*4 + j] = (i==j);
                    }
                }
            }

VertexNode::VertexNode(std::vector<std::vector<float>> *_vertices, GLenum _mode, GLclampf colorfv[], Node *_child, Node *_sibling)
            : _vertices(_vertices), _mode(_mode), Node(_child, _sibling)
            {
                std::memcpy(this->_colorfv, colorfv, NUM_COLOR * sizeof(GLclampf));

                for (int i = 0; i < 4; i++){
                    for (int j = 0; j < 4; j++)
                    {
                        modelview[i*4 + j] = (i==j);
                    }
                }
            }


void VertexNode::set(std::vector<std::vector<float>> *vertices) {
    this->_vertices = vertices;
}

std::vector<std::vector<float>> *VertexNode::vertices() {
    return _vertices;
}

GLclampf *VertexNode::color() {
    return this->_colorfv;
}

float * VertexNode::modelView()
{
    return modelview;
}

GradientVertexNode::GradientVertexNode(std::vector<std::vector<float>> *_vertices, GLenum _mode,
                                       vector<GLclampf *> &colorfv)
        : VertexNode(_vertices, _mode, nullptr){
    this->_colorfv = colorfv;
}

void GradientVertexNode::_display() {
    if(this->_vertices == nullptr) {
        return;
    }

    unsigned long num_vertices = this->_vertices->size();
    unsigned long num_colors = num_vertices < this->_colorfv.size() ? num_vertices : this->_colorfv.size();
    num_vertices = num_vertices - num_vertices % num_colors;
    unsigned long interval = num_vertices / num_colors;

    glPushMatrix();

    glBegin(this->_mode);

    int counter = 0, idx = 0;
    for(auto & _vertex : *this->_vertices) {
        if(counter < num_vertices && counter % interval == 0) {
            glColor3fv(this->_colorfv[idx++]);
        }
        if(_vertex.size() == 2){
            glVertex2f(_vertex[0], _vertex[1]);
        } else if (_vertex.size() == 3) {
            glVertex3f(_vertex[0], _vertex[1], _vertex[2]);
        }
        counter++;
    }
    glEnd();
    glPopMatrix();

    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
}

void GradientVertexNode::set(std::vector<std::vector<float>> *vertices) {
    this->_vertices = vertices;
}
