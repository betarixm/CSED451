#include "Shape.h"
#define PI 3.141592

/// @class Shape

Shape::Shape(float x, float y, float length, GLclampf r, GLclampf g, GLclampf b)
        : _x(x), _y(y), _length(length) {
    this->_colorfv[R] = r;
    this->_colorfv[G] = g;
    this->_colorfv[B] = b;
}

Shape::Shape(float x, float y, float length, const GLclampf *colorfv) : _x(x), _y(y), _length(length) {
    std::memcpy(this->_colorfv, colorfv, NUM_COLOR * sizeof(GLclampf));
}

/**
 * @brief 도형 기본 출력자
 * @param rotateDeg 회전 각도
 * @param mat 기본 정점 벡터 행렬 ((x,y), (x,y), ...)
 * @param mode OpenGL 출력 모드
 */
void Shape::_display(float rotateDeg, std::vector<std::vector<float>> &mat, GLenum mode) {
    float x = this->_x, y = this->_y;

    glPushMatrix();
    glLoadIdentity();

    glColor3fv(this->color());
    glTranslatef(x, y, 0.0f);
    glRotatef(rotateDeg, 0.0f, 0.0f, 1.0f);

    glBegin(mode);
    for(auto & i : mat){
        glVertex2f(i[0], i[1]);
    }
    glEnd();

    glFlush();

    glPopMatrix();
}

/**
 * @brief dx, dy만큼을 기존 좌표에 더해서 도형 이동
 * @param dx 더할 x 좌표의 크기
 * @param dy 더할 y 좌표의 크기
 */
void Shape::move(float dx, float dy) {
    this->_x += dx;
    this->_y += dy;
}

/**
 * @brief ddeg만큼을 기존 각도에 더해서 회전
 * @param ddeg 더할 각도 (not radian)
 */
void Shape::rotate(float ddeg) {
    this->_degree += ddeg;
}

/**
 * @brief 도형 degree 설정
 * @param deg 도형의 degree로 설정 할 값
 */
void Shape::setDegree(float deg) {
    this->_degree = deg;
}

/**
 * @brief 도형 색깔 수정
 * @param dColorfv 색깔 변동량을 담고 있는 배열
 */
void Shape::mutateColor(const GLclampf *dColorfv) {
    for (int i = 0; i < 3; i++) {
        this->_colorfv[i] = dColorfv[i];
    }
}

/**
 * @brief 중심 x 좌표
 */
float Shape::x() const {
    return this->_x;
}

/**
 * @brief 중심 y 좌표
 */
float Shape::y() const {
    return this->_y;
}

/**
 * @brief 한 변의 길이
 */
float Shape::length() const {
    return this->_length;
}

/**
 * @brief 도형 색상
 * @return 색상 배열 주소 반환
 */
GLclampf *Shape::color() {
    return this->_colorfv;
}

/**
 * @brief 도형 회전 각도
 */
float Shape::degree() const {
    return this->_degree;
}


/// @class Triangle @extends Shape

Triangle::Triangle(float x, float y, float length, GLclampf r, GLclampf g, GLclampf b) : Shape(x, y, length, r, g, b) {}

Triangle::Triangle(float x, float y, float length, const GLclampf *colorfv) : Shape(x, y, length, colorfv) {}

/**
 * @brief 도형 기본 회전 값으로 display
 */
void Triangle::display() {
    this->display(this->degree());
}

/**
 * @brief 회전 각도 임의 지정하여 display
 * @param rotateDeg 회전 각도 (not radian)
 */
void Triangle::display(float rotateDeg) {
    float x = this->x(), y = this->y(), length = this->length();

    std::vector<std::vector<float>> triangleMat = {
        {              0, -1 * length /      SQRT_3 },
        {     length / 2,      length / (2 * SQRT_3)},
        {-1 * length / 2,      length / (2 * SQRT_3)}
    };

    this->_display(rotateDeg, triangleMat, GL_TRIANGLES);
}


/// @class Square @extends Shape

Square::Square(float x, float y, float length, GLclampf r, GLclampf g, GLclampf b) : Shape(x, y, length, r, g, b) {}

Square::Square(float x, float y, float length, const GLclampf *colorfv) : Shape(x, y, length, colorfv) {}

/**
 * @brief 도형 기본 회전 값으로 display
 */
void Square::display() {
    this->display(this->degree());
}


/**
 * @brief 회전 각도 임의 지정하여 display
 * @param rotateDeg 회전 각도 (not radian)
 */
void Square::display(float rotateDeg) {
    float length = this->length();
    float unit = length / 2;

    std::vector<std::vector<float>> squareMat = {
            {-unit, unit},
            {unit,  unit},
            {unit,  -unit},
            {-unit, -unit}
    };

    this->_display(rotateDeg, squareMat, GL_QUADS);
}

void Circle::display() {
    this->display(this->degree());
}

void Circle::display(float rotateDeg) {
    std::vector<std::vector<float>> circleMat = {};

    for(int i = 0; i < 360; i++){
        std::vector<float> ele = {
                this->length() * (float)cos((float)i * PI / 180),
                this->length() * (float)sin((float)i * PI / 180)
        };
        circleMat.push_back(ele);
    }
    this->_display(rotateDeg, circleMat, GL_POLYGON);
}
