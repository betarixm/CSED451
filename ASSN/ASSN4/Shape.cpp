#include "Shape.h"
#include "helper.h"
#define PI 3.141592

using namespace std;

Grid* grid;
Grid* boundary;


void initGrid() {
    boundary = new Grid("texture/background.jpeg", "texture/background.jpeg", 0.1f, 0.1f, 20, 24, 0, 0, -0.295f, 0, 0.0f, 0.5f, 1.0f);
    grid = new Grid("texture/background.jpeg","texture/background.jpeg", 0.1f, 0.1f, 30, 40, 0, 0, -0.3f, 0, 1.0f, 1.0f, 1.0f);

}
/// @class Shape
// static variable vertices initialization
// scale = length, model frame unit = 1

Shape::Shape(const char *path, const char *normal_path, float x, float y, float z, float deg, float sx, float sy, float sz, GLenum mode, GLclampf r, GLclampf g, GLclampf b)
            : texture(path), normalMap(normal_path)
{
    cout << "hi" << endl;

    GLclampf colorfv[3];
    colorfv[RED] = r;
    colorfv[GREEN] = g;
    colorfv[BLUE] = b;

    this->_translation = new TranslateNode(x, y, 0);
    this->_rotation = new RotationNode(deg);
    //this->_scale = new ScaleNode(sx, sy, sz);
    this->_vertex = new VertexNode(nullptr, mode, colorfv);

    this->_group = new GroupNode;

    /** Group < T R S V >  -> Group <TRV> */
    this->_group->addChild(this->_translation);
    this->_translation->addSibling(this->_rotation);
    this->_rotation->addSibling(this->_vertex);
    //this->_scale->addSibling(this->_vertex);

    this->_vertex->setTid(texture.getTid(), normalMap.getTid());

}

Shape::Shape(const char *path, const char *normal_path,float x, float y, float z, float deg, float sx, float sy, float sz, GLenum mode, GLclampf colorfv[])
            : texture(path), normalMap(normal_path)
{
    this->_translation = new TranslateNode(x, y, 0);
    this->_rotation = new RotationNode(deg);
    //this->_scale = new ScaleNode(sx, sy, sz);
    this->_vertex = new VertexNode(nullptr, mode, colorfv);

    this->_group = new GroupNode;
    /** Group < T R S V > */
    this->_group->addChild(this->_translation);
    this->_translation->addSibling(this->_rotation);
    this->_rotation->addSibling(this->_vertex);
    //this->_scale->addSibling(this->_vertex);

    this->_vertex->setTid(texture.getTid(), normalMap.getTid());
}

/**
 * @brief 도형 기본 출력자
 * @param rotateDeg 회전 각도
 * @param mat 기본 정점 벡터 행렬 ((x,y), (x,y), ...)
 * @param mode OpenGL 출력 모드
 */
void Shape::display(bool isBlack) {
    this->_group->display(isBlack);
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
    this->_vertex->mutateColor(dColorfv[RED], dColorfv[GREEN], dColorfv[BLUE]);
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
    auto* T = new TranslateNode(dx, dy, dz);
    auto * T_1 = new TranslateNode(-dx, -dy, -dz);
    this->_translation->addSibling(T);
    this->_rotation->addSibling(T_1);
}

std::vector<std::vector<float>> Shape::getPosition(int num_pos)
{
    std::vector<std::vector<float>> model_frame = *_vertex->vertices();
    float *modelView = _vertex->modelView();
    std::vector<std::vector<float>> view;

    std::vector<std::vector<float>> result(num_pos, std::vector<float>(4, 0));


    for (int i=0; i<4; i++)
    {
        std::vector<float> temp;
        temp.reserve(4);
        for (int j = 0; j <4; j++)
            temp.push_back(modelView[i+4*j]);
        view.push_back(temp);
    }

    for (int i=0; i<num_pos; i++)
    {
        /** change to Homogeneous coordinate */
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

void Shape::setDirection(float x, float y, float z)
{
    this->_direction[0] = x;
    this->_direction[1] = y;
    this->_direction[2] = z;
}

std::vector<float> Shape::direction()
{
    return this->_direction;
}

void Shape::setNumVertex(unsigned long num)
{
    this->_vertex->setNumVertex(num);
}



Object::Object(char *path, const char *map_path, const char *normal_path, float x, float y, float z, float deg, GLclampf r, GLclampf g, GLclampf b)
        : Shape(map_path, normal_path, x, y, z, deg, 1, 1, 1, GL_TRIANGLES, r, g, b), _model(path) {
    vector<float> objVertexBuffer = _model.compat();
    setNumVertex(objVertexBuffer.size()/9);
    this->setVertexArray(objVertexBuffer);
}


glm::vec3 Object::max()
{
    return _model.max();
}
glm::vec3 Object::min()
{
    return _model.min();
}



Grid::Grid(const char *map_path, const char *normal_path, float width, float height, int row, int col, float x, float y, float z, float deg, GLclampf r, GLclampf g,
           GLclampf b) : Shape(map_path, normal_path,  x, y, z, deg, 1, 1, 1, GL_LINES, r, g, b){
    float xAdj = - width * (float)col / 2.0f;
    float yAdj = - height * (float)row / 2.0f;

    vector<float> vertexBuffer{};

    for(int i = 0; i <= row; i++) {
        vertexBuffer.insert(vertexBuffer.end(), {
            0 + xAdj, height * (float)i + yAdj, z,
            0, 0, 1,
            0, (float)i / (float)row, 0
        });

        vertexBuffer.insert(vertexBuffer.end(), {
            width * (float)col + xAdj, height * (float)i + yAdj, z,
            0, 0, 1,
            1, (float)i / (float)row, 0
        });
    }

    for(int i = 0; i <= col; i++) {
        vertexBuffer.insert(vertexBuffer.end(), {
            width * (float)i + xAdj, 0 + yAdj, z,
            0, 0, 1,
            (float) i / (float) col, 0, 0
        });
        vertexBuffer.insert(vertexBuffer.end(), {
            width * (float)i + xAdj, height * (float)row + yAdj, z,
            0, 0, 1,
            (float) i / (float) col, 1, 0
        });
    }

    setNumVertex(vertexBuffer.size() / 9);
    this->setVertexArray(vertexBuffer);
}



Sphere::Sphere(const char *map_path, const char *normal_path, int lat, int lon, float radius, float x, float y, float z, float deg, GLclampf *colorfv)
        : Shape(map_path, normal_path, x, y, z, deg, radius, radius, radius, GL_TRIANGLE_STRIP, colorfv) {

    this->init(lat, lon, radius);
}

Sphere::Sphere(const char *map_path, const char *normal_path, int lat, int lon, float radius, float x, float y, float z, float deg, GLclampf r, GLclampf g, GLclampf b)
        : Shape(map_path, normal_path, x, y, z, deg, radius, radius, radius, GL_TRIANGLE_STRIP, r, g, b) {
    this->init(lat, lon, radius);
}

void Sphere::init(int lat, int lon, float radius){
    vector<float> vertexBuffer{};

    vector<vector<vector<float>>> vertexImm{};
    vector<vector<vector<float>>> normalImm{};
    vector<vector<vector<float>>> uvImm{};

    for (int a = 0; a < lat + 1; a++) {
        vector<vector<float>> vertexLatVec{};
        vector<vector<float>> normalLatVec{};
        vector<vector<float>> uvLatVec{};

        float phi = (float)a / (float)lat * (float)PI;
        float _z = radius * cos(phi);
        for (int o = 0; o < lon; o++) {
            float theta = (float)o / (float)lon * (float)PI * 2;
            float _r = radius * sin(phi);
            float dx = sin(phi)*cos(theta), dy = sin(phi)*sin(theta), dz = cos(phi);

            vertexLatVec.emplace_back(initializer_list<float>{_r * cos(theta), _r * sin(theta), _z});
            normalLatVec.emplace_back(initializer_list<float>{dx, dy, dz});

            float u, v;
            u = 0.5f + atan2(dx, dz)/(2*PI);
            v = 0.5f - asin(dy)/(PI);
            uvLatVec.emplace_back(initializer_list<float>{u, v, 0});
            //uvLatVec.emplace_back(initializer_list<float>{sin(theta) * cos(phi), sin(theta) * sin(phi), 0});
        }
        vertexImm.push_back(vertexLatVec);
        normalImm.push_back(normalLatVec);
        uvImm.push_back(uvLatVec);
    }

    for (int a = 0; a < lat; a++) {
        for (int o = 0; o < lon; o++) {
            vertexBuffer.insert(vertexBuffer.end(), {
                vertexImm[a][o][0], vertexImm[a][o][1], vertexImm[a][o][2],
                normalImm[a][o][0], normalImm[a][o][1], normalImm[a][o][2],
                uvImm[a][o][0], uvImm[a][o][1], uvImm[a][o][2],

                vertexImm[a+1][o][0], vertexImm[a+1][o][1], vertexImm[a+1][o][2],
                normalImm[a+1][o][0], normalImm[a+1][o][1], normalImm[a+1][o][2],
                uvImm[a+1][o][0], uvImm[a+1][o][1], uvImm[a+1][o][2]
            });
        }

        vertexBuffer.insert(vertexBuffer.end(), {
                vertexImm[a][0][0], vertexImm[a][0][1], vertexImm[a][0][2],
                normalImm[a][0][0], normalImm[a][0][1], normalImm[a][0][2],
                uvImm[a][0][0], uvImm[a][0][1], uvImm[a][0][2],
        });
    }

    vertexBuffer.insert(vertexBuffer.end(), {
            vertexImm[lat][0][0], vertexImm[lat][0][1], vertexImm[lat][0][2],
            normalImm[lat][0][0], normalImm[lat][0][1], normalImm[lat][0][2],
            uvImm[lat][0][0], uvImm[lat][0][1], uvImm[lat][0][2],
    });

    setNumVertex(vertexBuffer.size() / 9);
    setVertexArray(vertexBuffer);
}


void Shape::setVertexArray(std::vector<std::vector<float>> &vertex, std::vector<std::vector<float>> &normal,
                           std::vector<std::vector<float>> &uv)
{
    vector<float> vertexBuffer = vectorCompat(vertex, normal, uv);
    setVBO(vertexBuffer);
}

void Shape::setVBO(vector<float> &VBO) {
    //Bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // setting VBO
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(float) * (GLsizeiptr)VBO.size(), &VBO[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    this->_vertex->setVAO(VAO);
}

void Shape::setVertexArray(vector<float> &VBO) {
    setVBO(VBO);
}

