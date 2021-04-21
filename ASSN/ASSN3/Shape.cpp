#include "Shape.h"
#define PI 3.141592
#include <iostream>
using namespace std;

/// @class Shape

Shape::Shape(float x, float y, float z, float deg, GLenum mode, GLclampf r, GLclampf g, GLclampf b)
{

    GLclampf colorfv[3];
    colorfv[R] = r;
    colorfv[G] = g;
    colorfv[B] = b;

    this->_translation = new TranslateNode(x, y, 0);
    this->_rotation = new RotationNode(deg);
    this->_vertex = new VertexNode(nullptr, mode, colorfv);

    this->_group = new GroupNode;

    /** Group < T R V > */
    this->_group->addChild(this->_translation);
    this->_translation->addSibling(this->_rotation);
    this->_rotation->addSibling(this->_vertex);

}

Shape::Shape(float x, float y, float z, float deg, GLenum mode, GLclampf colorfv[])
{
    this->_translation = new TranslateNode(x, y, 0);
    this->_rotation = new RotationNode(deg);
    this->_vertex = new VertexNode(nullptr, mode, colorfv);

    this->_group = new GroupNode;
    /** Group < T R V > */
    this->_group->addChild(this->_translation);
    this->_translation->addSibling(this->_rotation);
    this->_rotation->addSibling(this->_vertex);
}

Shape::Shape(float x, float y, float z, float deg, float x_r, float y_r, float z_r, GLenum mode, GLclampf r, GLclampf g,
             GLclampf b)
{
    GLclampf colorfv[3];
    colorfv[R] = r;
    colorfv[G] = g;
    colorfv[B] = b;

    this->_translation = new TranslateNode(x, y, 0);
    this->_rotation = new RotationNode(deg, x_r, y_r, z_r);
    this->_vertex = new VertexNode(nullptr, mode, colorfv);

    this->_group = new GroupNode;
    /** Group < T R V > */
    this->_group->addChild(this->_translation);
    this->_translation->addSibling(this->_rotation);
    this->_rotation->addSibling(this->_vertex);
}

/**
 * @brief 도형 기본 출력자
 * @param rotateDeg 회전 각도
 * @param mat 기본 정점 벡터 행렬 ((x,y), (x,y), ...)
 * @param mode OpenGL 출력 모드
 */
void Shape::display() {
    this->_group->display();
}

/**
 * @brief dx, dy만큼을 기존 좌표에 더해서 도형 이동
 * @param dx 더할 x 좌표의 크기
 * @param dy 더할 y 좌표의 크기
 */
void Shape::move(float dx, float dy) {
    this->_translation->move(dx, dy, 0);
}

/**
 * @brief ddeg만큼을 기존 각도에 더해서 회전
 * @param ddeg 더할 각도 (not radian)
 */
void Shape::rotate(float ddeg) {
    this->_rotation->rotate(ddeg);
}

/**
 * @brief 도형 degree 설정
 * @param deg 도형의 degree로 설정 할 값
 */
void Shape::setDegree(float deg) {
    this->_rotation->set(deg);
}

/**
 * @brief 도형 색깔 수정
 * @param dColorfv 색깔 변동량을 담고 있는 배열
 */
void Shape::mutateColor(const GLclampf *dColorfv) {
    this->_vertex->mutateColor(dColorfv[R], dColorfv[G], dColorfv[B]);
}

/**
 * @brief 중심 x 좌표
 */
float Shape::x() const {
    return this->_translation->delta()[0];
}

/**
 * @brief 중심 y 좌표
 */
float Shape::y() const {
    return this->_translation->delta()[1];
}


/**
 * @brief 도형 색상
 * @return 색상 배열 주소 반환
 */
GLclampf *Shape::color() {
    return this->_vertex->color();
}

/**
 * @brief 도형 회전 각도
 */
float Shape::degree() const {
    return this->_rotation->degree();
}

void Shape::setVertex(std::vector<std::vector<float>> *mat)
{
    this->_vertex->set(mat);
}

GroupNode* Shape::groupNode()
{
    return this->_group;
}

void Shape::pivot_rotate(float dx, float dy, float dz)
{
    TranslateNode * T = new TranslateNode(dx, dy, dz);
    auto * T_1 = new TranslateNode(-dx, -dy, -dz);
    this->_translation->addSibling(T);
    this->_rotation->addSibling(T_1);
}

std::vector<std::vector<float>> Shape::getPosition()
{
    std::vector<std::vector<float>> model_frame = *_vertex->vertices();
    float *modelView = _vertex->modelView();
    std::vector<std::vector<float>> view;
    int num_pos = model_frame.size();

    std::vector<std::vector<float>> result(num_pos, std::vector<float>(4, 0));


    for (int i=0; i<4; i++)
    {
        std::vector<float> temp;
        for (int j = 0; j <4; j++)
            temp.push_back(modelView[i+4*j]);
        view.push_back(temp);
    }

    for (int i=0; i<num_pos; i++)
    {
        /** change to Homogeneous coordinate */
        model_frame[i].push_back(0); //z
        model_frame[i].push_back(1); //w
    }


    for(int i=0; i<4; i++)
    {
        for(int j=0; j<num_pos; j++)
        {
            float sum = 0.0;
            for(int k=0; k<4; k++)
            {
                sum += view[i][k]*model_frame[j][k];
            }
            result[j][i] += sum;
        }
    }

    return result;
}



/// @class Triangle @extends Shape

Triangle::Triangle(float x, float y, float length, float deg, GLclampf r, GLclampf g, GLclampf b)
: Shape(x, y, 0, deg, GL_TRIANGLES, r, g, b)
{

    this->_modelFrame = {
            {              0, -1 * length /      SQRT_3 },
            {     length / 2,      length / (2 * SQRT_3)},
            {-1 * length / 2,      length / (2 * SQRT_3)}
    };

    this->setVertex(&(this->_modelFrame));
}


Triangle::Triangle(float x, float y, float length, float deg, GLclampf *colorfv)
: Shape(x, y, 0, deg, GL_TRIANGLES, colorfv)
{
    this->_modelFrame = {
            {              0, -1 * length /      SQRT_3 },
            {     length / 2,      length / (2 * SQRT_3)},
            {-1 * length / 2,      length / (2 * SQRT_3)}
    };

    this->setVertex(&(this->_modelFrame));
}

Triangle::Triangle(float x, float y, float length, float x_r, float y_r, float z_r, float deg, GLclampf r, GLclampf g, GLclampf b)
        : Shape(x, y, 0, deg, x_r, y_r, z_r, GL_TRIANGLES, r, g, b)
{
    this->_modelFrame = {
            {              0, -1 * length /      SQRT_3 },
            {     length / 2,      length / (2 * SQRT_3)},
            {-1 * length / 2,      length / (2 * SQRT_3)}
    };

    this->setVertex(&(this->_modelFrame));
}

/// @class Square @extends Shape

Square::Square(float x, float y, float width, float height, float deg, GLclampf r, GLclampf g, GLclampf b)
: Shape(x, y, 0, deg, GL_QUADS, r, g, b)
{
    this->_width = width;
    this->_height = height;
    float w_unit = width/2;
    float h_unit = height/2;
    this->_modelFrame = {
            {-w_unit, h_unit},
            {w_unit,  h_unit},
            {w_unit,  -h_unit},
            {-w_unit, -h_unit}
    };
    this->setVertex(&(this->_modelFrame));
}

Square::Square(float x, float y, float width, float height, float deg, GLclampf colorfv[])
: Shape(x, y, 0, deg, GL_QUADS, colorfv)
{
    this->_width = width;
    this->_height = height;
    float w_unit = width/2;
    float h_unit = height/2;
    this->_modelFrame = {
            {-w_unit, h_unit},
            {w_unit,  h_unit},
            {w_unit,  -h_unit},
            {-w_unit, -h_unit}
    };
    this->setVertex(&(this->_modelFrame));
}

Square::Square(float x, float y, float width, float height, float x_r, float y_r, float z_r, float deg, GLclampf r, GLclampf g, GLclampf b)
        : Shape(x, y, 0, deg, x_r, y_r, z_r, GL_QUADS, r, g, b)
{
    this->_width = width;
    this->_height = height;
    float w_unit = width/2;
    float h_unit = height/2;
    this->_modelFrame = {
            {-w_unit, h_unit},
            {w_unit,  h_unit},
            {w_unit,  -h_unit},
            {-w_unit, -h_unit}
    };
    this->setVertex(&(this->_modelFrame));
}

float Square::getWidth()
{
    return this->_width;
}

float Square::getHeight()
{
    return this->_height;
}


Circle::Circle(float x, float y, float length, float deg,  GLclampf r, GLclampf g, GLclampf b)
: Shape(x, y, 0, deg, GL_POLYGON, r, g, b)
{
    this->_modelFrame = {};

    for(int i = 0; i < 360; i++){
        std::vector<float> ele = {
                length * (float)cos((float)i * PI / 180),
                length * (float)sin((float)i * PI / 180)
        };
        this->_modelFrame.push_back(ele);
    }
    this->setVertex(&(this->_modelFrame));
}

Circle::Circle(float x, float y, float length, float deg, GLclampf colorfv[])
: Shape(x, y, 0, deg, GL_POLYGON, colorfv)
{
    this->_modelFrame = {};

    for(int i = 0; i < 360; i++){
        std::vector<float> ele = {
                length * (float)cos((float)i * PI / 180),
                length * (float)sin((float)i * PI / 180)
        };
        this->_modelFrame.push_back(ele);
    }
    this->setVertex(&(this->_modelFrame));
}

Circle::Circle(float x, float y, float length, float x_r, float y_r, float z_r, float deg,  GLclampf r, GLclampf g, GLclampf b)
        : Shape(x, y, 0, deg, x_r, y_r, z_r, GL_POLYGON, r, g, b)
{
    this->_modelFrame = {};

    for(int i = 0; i < 360; i++){
        std::vector<float> ele = {
                length * (float)cos((float)i * PI / 180),
                length * (float)sin((float)i * PI / 180)
        };
        this->_modelFrame.push_back(ele);
    }
    this->setVertex(&(this->_modelFrame));
}

GradientShape::GradientShape(float x, float y, float deg, GLenum mode, vector<GLclampf *> &colorfv) {
    this->_translation = new TranslateNode(x, y, 0);
    this->_rotation = new RotationNode(deg);
    this->_vertex = new GradientVertexNode(nullptr, mode, colorfv);

    this->_group = new GroupNode;
    /** Group < T R V > */
    this->_group->addChild(this->_translation);
    this->_translation->addSibling(this->_rotation);
    this->_rotation->addSibling(this->_vertex);
}

void GradientShape::setVertex(std::vector<std::vector<float>> *mat) {
    this->_vertex->set(mat);
}

GradientCircle::GradientCircle(float x, float y, float length, float deg, vector<GLclampf *> &colorfv)
        :GradientShape(x, y, deg, GL_POLYGON, colorfv) {
    this->_modelFrame = {};
    for(int i = 0; i < 360; i++){
        std::vector<float> ele = {
                length * (float)cos((float)i * PI / 180),
                length * (float)sin((float)i * PI / 180)
        };
        this->_modelFrame.push_back(ele);
    }
    this->setVertex(&(this->_modelFrame));
}

Object::Object(char *path, float x, float y, float z, float deg, GLclampf r, GLclampf g, GLclampf b)
        : Shape(x, y, z, deg, GL_TRIANGLES, r, g, b), _model(path) {
    this->_modelFrame = _model.compat();
    this->setVertex(&(this->_modelFrame));
}

Object::Object(char *path, float x, float y, float z, float deg, GLclampf *colorfv)
        : Shape(x, y, z, deg, GL_TRIANGLES, colorfv), _model(path) {
    this->_modelFrame = _model.compat();
    this->setVertex(&(this->_modelFrame));
}

Object::Object(char *path, float x, float y, float z, float x_r, float y_r, float z_r, float deg, GLclampf r,
               GLclampf g, GLclampf b)
        : Shape(x, y, z, deg, x_r, y_r, z_r, GL_TRIANGLES, r, g, b), _model(path) {
    this->_modelFrame = _model.compat();
    this->setVertex(&(this->_modelFrame));
}
