//
// Created by 최은수 on 2021/03/04.
//

#include "Ship.h"

/**
 * declared in main.cpp
 * n : normal, f : fail, c : king-god
 */
extern char mode;
#define dist 0.05  /** ship 움직일때 거단위 */
#define PIE 3.1415926

void Ship::display()
{
    glObject.display();
}

void Ship::display(float rotateDeg)
{
    glObject.display(rotateDeg);
}
/**
 * 한 점 P가 두 점 AB 직선 내부에 있는지 확
 * @param dot 대상 점 P
 * @param A1  점 A
 * @param A2  점 B
 * @return 음수 or 양수값
 */
float Ship::dotOverline(vector<float> dot, vector<float> A1, vector<float>A2)
{
    float x = dot[0];
    float y = dot[1];
    float x1 = A1[0];
    float y1 = A1[1];
    float x2 = A2[0];
    float y2 = A2[1];

    float result;

    result = (x-x1)*(y1-y2)  - (y-y1)*(x1-x2);
    return result;
}

/**
 * x,y 내분점을 기준으로 나머지 3개의 vertex 좌표 계
 * @return 3점의 x,y 좌표 vector Matrix
 */
vector<vector<float>> Ship::getPosition()
{
    float x = glObject.x(), y = glObject.y(), length = glObject.length();
    float degree = glObject.degree() * PIE / 180;
    /**
     * rotate the point (a, b) -> (a', b') 기준 - (x, y)점
     * d := 회전각 theta
     * a' = x+ (a-x)cos(d) - (b-y)sin(d)
     * b' = y+ (a-x)sin(d) + (b-y)cos(d)
     */
    vector<vector<float>> pos = {
            {              0,     -1*length /      SQRT_3},
            {     length / 2,     length / (2 * SQRT_3)},
            {-1 * length / 2,     length / (2 * SQRT_3)}
    };

    vector<vector<float>> rotate = {
            {cos(degree), -1*sin(degree)},
            {sin(degree), cos(degree)}
    };

    vector<vector<float>> result = {{x,y}, {x,y}, {x,y}};

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<2; j++)
        {
            float sum = 0.0;
            for(int k=0; k<2; k++)
            {
                sum += pos[i][k]*rotate[k][j];
            }
            result[i][j] += sum;
        }
    }
    return result;
}


/* check whether ship hits bullet
 * if hit, return True else return False
 * check the vertex of bullet over the surface of ship */
/**
 * @brief bullet 과 Ship이 충돌했는지 체크
 * @brief bullet의 네 점중 하나라도 삼각형 내부에 존재하는지 여부로 체크.
 * @param bullet
 * @return 충돌여부 (true/false)
 */
bool Ship::hit(Bullet* bullet)
{
    float x_b = bullet->x(), y_b = bullet->y(), unit = (bullet->length())/2;
    bool result = false;

    vector<vector<float>> bullet_pos = {
            {x_b - unit, y_b + unit},
            {x_b + unit, y_b + unit},
            {x_b + unit, y_b - unit},
            {x_b - unit, y_b - unit}
    };

    vector<vector<float>> pos = getPosition();

    for(int i=0; i<4; i++)
    {
        if(dotOverline(pos[0], pos[1], pos[2]) * dotOverline(bullet_pos[i], pos[1], pos[2]) < 0)
            continue;
        if(dotOverline(pos[1], pos[0], pos[2]) * dotOverline(bullet_pos[i], pos[0], pos[2]) < 0)
            continue;
        if(dotOverline(pos[2], pos[0], pos[1]) * dotOverline(bullet_pos[i], pos[0], pos[1]) < 0)
            continue;
        result = true;
        break;
    }
    return result;

}

/**
 * @brief 총알이 발사될 때 bullet 인스턴스 생성
 * @return Bullet*
 */
Bullet* Ship::shot()
{
    GLclampf* color = glObject.color();
    float y = getPosition()[0][1];
    Bullet* bullet = new Bullet(glObject.x(), y, 0.01, 1, 1, 1);

    return bullet;

}
/**
 * @brief _player 가 bullet에 맞았는지 체크 및 목숨/컬러 변
 * @brief game mode에 따라 나눠짐.
 * @parmas bullet_list - Bullet*를 담고 있는 list의 주소
 */
void Player::checkHit(list<Bullet*>* bullet_list)
{
    list<Bullet*>::iterator itr;
    bool isHit;
    int damage = 1;
    Bullet* bullet;

    switch(mode) {
        case 'c':   /* if "c mode" _player don't die */
            return;
        case 'f':
            damage = _numLife;
        case 'n':
            itr = bullet_list->begin();
            while(itr != bullet_list->end())
            {
                isHit = hit(*itr);
                if (isHit) {
                    _numLife -= damage;
                    glObject.mutateColor(0, 0, -0.1 * damage);
                    bullet = *itr;
                    bullet_list->erase(itr++);
                    delete (bullet);
                }
                else
                    ++itr;

                /* 여러대 맞아도 목숨 다 깎이면 게임 종료 */
                if (_numLife == 0)
                    return;
            }
    }
}

/**
 * @brief 키보드 입력 되었을 때 handler
 * @parmas char(key) 눌린 키 종류 (space_bar, direction)
 */
Bullet * Player::keyHandler()
{
    Bullet* bullet = nullptr;
    float dx = 0, dy = 0;

    vector<vector<float>> pos = getPosition();

    if(this->inputKey['S']){
        if (mode != 'f')
            bullet = shot();
    }

    if(this->inputKey['U']){
        dy = dist;
    }

    if(this->inputKey['D']){
        dy = -1*dist;
    }

    if(this->inputKey['L']){
        dx = -1*dist;
    }

    if(this->inputKey['R']){
        dx = dist;
    }

    /** window 밖으로 넘어가는지 체크 */
    for(int i=0; i < 3; i++)
    {
        float x = pos[i][0], y = pos[i][1];

        if(x+dx > 1.0 || x+dx < -1.0 || y+dy > 1.0 || y+dy < -1.0)
        {
            return nullptr;
        }
    }

    glObject.move(dx, dy);
    return bullet;
}

/** 좌/우/멈춤 중 _enemy 움직임 랜덤 선
 */
void Enemy::randomMoveHandler()
{
    float dx =0;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(-1, 1);
    dx = dist*dis(gen);

    vector<vector<float>> pos = getPosition();
    for(int i=0; i < 3; i++)
    {
        float x = pos[i][0];
        if(x+dx > 1.0 || x+dx < -1.0 )
        {
            return;
        }
    }

    glObject.move(dx, 0);

}
/**
 * @brief _enemy 가 bullet에 맞았는지 체크 및 목숨/컬러 변
 * @brief game mode에 따라 나눠짐.
 * @parmas bullet_list - Bullet*를 담고 있는 list의 주소
 */
void Enemy::checkHit(list<Bullet*>* bullet_list)
{
    list<Bullet*>::iterator itr;
    bool isHit;
    int damage = 1;
    Bullet* bullet;

    switch(mode) {
        case 'c': /** c mode일 때 적은 1대만 맞아도 죽음 */
            damage = _numLife;
        default:
            itr = bullet_list->begin();
            while(itr != bullet_list->end())
            {
                isHit = hit(*itr);
                if (isHit) {
                    _numLife -= damage;
                    bullet = *itr;
                    bullet_list->erase(itr++);
                    delete (bullet);
                }
                else
                    ++itr;;

                /** 여러대 맞아도 목숨 다 깎이면 게임 종료 */
                if (_numLife == 0)
                    return;
            }
    }
}
