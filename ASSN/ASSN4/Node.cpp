#include "Node.h"
#include "Light.h"

#define PIE 3.141592
using namespace std;

extern Shader* shader;

deque<glm::mat4> ModelView;
deque<glm::mat4> Projection;

Node::Node() : _child(nullptr), _sibling(nullptr){}
Node::Node(Node *_child, Node *_sibling) : _child(_child), _sibling(_sibling) {}

void Node::display(bool isBlack) {

    ModelView.emplace_back(1.0f);

    this->_display(isBlack);

    if(this->_child != nullptr) {
        this->_child->display(isBlack);
    }

    if(this->_sibling != nullptr) {
        this->_sibling->display(isBlack);
    }

    ModelView.pop_back();
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


    if(this->_child != nullptr) {
        this->_child->display(isBlack);
    }

    ModelView.pop_back();


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
    glm::mat4 myModelView = ModelView.back();
    glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(this->_degree), glm::vec3(_x, _y, _z));
    ModelView.pop_back();
    ModelView.push_back(myModelView * R);

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
    glm::mat4 myModelView = ModelView.back();
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(this->_dx, this->_dy, this->_dz));
    ModelView.pop_back();
    ModelView.push_back(myModelView * T);

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
    glm::mat4 modelView = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    for (auto & i : ModelView){
        modelView = modelView * i;
    }

    for (auto& i : Projection) {
        projection = projection * i;
    }

    GLint uniformModelView, uniformProjection, uniformColor;

    shader->uniform4m("ModelView", modelView);
    shader->uniform4m("Projection", projection);
    shader->uniform3v("material.ambient",  1.0f, 0.5f, 0.31f);
    shader->uniform3v("material.diffuse",  1.0f, 0.5f, 0.31f);
    shader->uniform3v("material.specular", 0.5f, 0.5f, 0.5f);
    shader->uniform1f("material.shininess", 32.0f);

    DirectionalLight::list[0]->use(shader);

    if(isBlack) {
        shader->uniform4v("color", 0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        shader->uniform4v("color", _colorfv[0], _colorfv[1], _colorfv[2], 1.0f);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(_VAO);
    glDrawArrays(this->_mode, 0, _numVertex);
    glBindVertexArray(0);
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


