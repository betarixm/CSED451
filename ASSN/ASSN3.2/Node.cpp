#include "Node.h"
using namespace std;

Node::Node() : _child(nullptr), _sibling(nullptr){}
Node::Node(Node *_child, Node *_sibling) : _child(_child), _sibling(_sibling) {}

void Node::display(bool isBlack) {
    //glPushMatrix();
    ModelView.emplace_back(1.0f);

    this->_display(isBlack);

    if(this->_child != nullptr) {
        this->_child->display(isBlack);
    }

    if(this->_sibling != nullptr) {
        this->_sibling->display(isBlack);
    }

    ModelView.pop_back();
    //glPopMatrix();
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

void GroupNode::display(bool isBlack)
{
    ModelView.emplace_back(0.1f);
    //glPushMatrix();

    if(this->_child != nullptr) {
        this->_child->display(isBlack);
    }

    ModelView.pop_back();
    //glPopMatrix();

    if(this->_sibling != nullptr) {
        this->_sibling->display(isBlack);
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


void RotationNode::_display(bool isBlack) {
    glm::mat4 R = glm::rotate(glm::mat4(1.0f), this->_degree, glm::vec3(_x, _y, _z));
    glm::mat4 myModelView = ModelView.back();
    ModelView.pop_back();
    ModelView.push_back(R * myModelView);

    //glRotatef(this->_degree, _x, _y, _z);
}

RotationNode::RotationNode(float _degree) : _x(0), _y(0), _z(1.0), _degree(_degree) {}
RotationNode::RotationNode(float _degree, float _x, float _y, float _z) : _x(_x), _y(_y), _z(_z), _degree(_degree) {}

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

void TranslateNode::_display(bool isBlack) {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(this->_dx, this->_dy, this->_dz));
    glm::mat4 myModelView = ModelView.back();
    ModelView.pop_back();
    ModelView.push_back(T * myModelView);

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

void VertexNode::_display(bool isBlack) {
    if(this->_vertices == nullptr) {
        return;
    }
    glm::mat4 modelView = glm::mat4(1.0f);

    for (auto & i : ModelView){
        modelView = i * modelView;
    }

    glm::mat4 P = glm::perspective(90.0f, 1.0f, 0.001f, 100.0f);

    GLint uniformModelView, uniformProjection, uniformColor;
    uniformModelView = glGetUniformLocation(myProgObj, "ModelView"); // in vertex shader
    glUniformMatrix4fv(uniformModelView, 1, GL_FALSE, &modelView[0][0]);

    uniformProjection = glGetUniformLocation(myProgObj, "Projection");
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, &P[0][0]);

    uniformColor = glGetUniformLocation(myProgObj, "uniformColor");
    if(isBlack) {
        glUniform4f(uniformColor, 0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        glUniform4f(uniformColor, _colorfv[0], _colorfv[1], _colorfv[2], 1.0f);
    }

    glDrawArrays(this->_mode, 0, _vertices->size());
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



ScaleNode::ScaleNode(float _sx, float _sy, float _sz) : _sx(_sx), _sy(_sy), _sz(_sz) {}

ScaleNode::ScaleNode(float _sx, float _sy, float _sz, Node* _child, Node* _sibling) : _sx(_sx), _sy(_sy), _sz(_sz), Node(_child, _sibling) {}

void ScaleNode::_display(bool isBlack) {
    ModelView.push_back(glm::mat4(1.0f));
    //glPushMatrix();

    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(this->_sx, this->_sy, this->_sz));
    glm::mat4 myModelView = ModelView.back();
    ModelView.pop_back();
    ModelView.push_back(S * myModelView);

    //glScalef(this->_sx, this->_sy, this->_sz);

}

void ScaleNode::set(float x, float y, float z)
{
    this->_sx = x;
    this->_sy = y;
    this->_sz = z;
}

void ScaleNode::resizing(float dx, float dy, float dz)
{
    this->_sx += dx;
    this->_sy += dy;
    this->_sz += dz;
}

std::vector<float> ScaleNode::delta(){
    return std::vector<float>{_sx, _sy, _sz};
}


