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
#define MAX_BULLET 5

extern Game* game;
extern list<Bullet*> player_bullets;

Ship::Ship(int _numLife, float x, float y, float size_torso, GLclampf r, GLclampf g, GLclampf b, float degree, int numBullet)
: _head(0, size_torso + sqrt(3)*size_torso/6 , size_torso, 180, r, g, b),
_torso(x, y, size_torso, 2*size_torso, degree, r, g, b),
_lwing(0, -size_torso, size_torso, 2*size_torso, -45, r, g, b),
_rwing(0, -size_torso, size_torso, 2*size_torso, 45, r, g, b),
_lcanon(0, -size_torso*2, size_torso, 2*size_torso, -135, r, g, b),
_rcanon(0, -size_torso*2, size_torso, 2*size_torso, 135, r, g, b)
{
    this->_numLife = _numLife;
    this->_baseScene = new GroupNode;
    this->_size_torso = size_torso;
    this->_numBullet = numBullet;
    this->_rotateDir = 1;

    this->_baseScene->addChild(this->_torso.groupNode());
    this->_torso.groupNode()->addToLast(this->_head.groupNode());
    this->_head.groupNode()->addSibling(this->_lwing.groupNode());
    this->_lwing.groupNode()->addToLast(this->_lcanon.groupNode());
    this->_lwing.groupNode()->addSibling(this->_rwing.groupNode());
    this->_rwing.groupNode()->addToLast(this->_rcanon.groupNode());

    /** TRT-1 */
    this->_lwing.pivot_rotate(0.0, size_torso, 0.0);
    this->_lcanon.pivot_rotate(0.0, size_torso, 0.0);
    this->_rwing.pivot_rotate(0.0, size_torso, 0.0);
    this->_rcanon.pivot_rotate(0.0, size_torso, 0.0);

}

void Ship::display()
{
    this->_baseScene->display();
}





/**
 * @brief bullet 과 Ship이 충돌했는지 체크
 * @brief bullet의 네 점중 하나라도 detection box 내부에 존재하는지 여부로 체크.
 * @param bullet
 * @return 충돌여부 (true/false)
 */

bool Ship::hit(Bullet* bullet)
{
    float x = _torso.x();
    float y = _torso.y();
    float len = _size_torso;
    bool isHit = false;

    vector<vector<float>> detection_box = {
            {x - 2.5f*len, y + len},
            {x + 2.5f*len, y + len},
            {x + 2.5f*len, y - len},
            {x - 2.5f*len, y - len}
    };

    vector<vector<float>> bullet_pos = bullet->getPosition();

    for (int i=0; i < 4; i++)
    {
        float x_b = bullet_pos[i][0];
        float y_b = bullet_pos[i][1];

        if (detection_box[0][0] > x_b || detection_box[0][1] < y_b)
            continue;
        if (detection_box[1][0] < x_b || detection_box[1][1] < y_b)
            continue;
        if (detection_box[2][0] < x_b || detection_box[2][1] > y_b)
            continue;
        if (detection_box[3][0] > x_b || detection_box[3][1] > y_b)
            continue;

        isHit = true;
        break;
    }
    return isHit;
}


/**
 * @brief 총알이 발사될 때 bullet 인스턴스 생성
 * @return Bullet*
 */

list<Bullet*> Ship::shot()
{
    GLclampf *color = _torso.color();
    vector<vector<float>> pos = _head.getPosition();
    float x = pos[0][0]/pos[0][3];
    float y = pos[0][1]/pos[0][3];
    int numBullet = _numBullet;
    float degree;

    list<Bullet*> result;

    if (numBullet % 2 != 0)
    {
        Bullet* b = new Bullet(x, y, 0.02, 0.02, 0, color);
        numBullet -= 1;
        result.push_back(b);
    }
    if (numBullet == 0)
        return result;

    degree = 90.0/(numBullet/2 + 1);

    for (int i = 1; i <= numBullet/2; i++){
        Bullet* b = new Bullet(0, 0, 0.02, 0.02, 0, color);
        RotationNode *r = new RotationNode(degree*i);
        TranslateNode *t = new TranslateNode(x, y, 0);
        r->addSibling(b->groupNode()->child());
        t->addSibling(r);
        b->groupNode()->addChild(t);
        result.push_back(b);
        /** Head 로 이동 T -> 발사각도로 좌표계 회전 R -> bullet 이동 y 움직임 T - */

        /** Rotate model frame coordinate to direction of bullet */
        b = new Bullet(0, 0, 0.02, 0.02, 0, color);
        r = new RotationNode(-degree*i);
        t = new TranslateNode(x, y, 0);
        r->addSibling(b->groupNode()->child());
        t->addSibling(r);
        b->groupNode()->addChild(t);
        result.push_back(b);

    }

    return result;

}

void Ship::mutateColor(GLclampf r, GLclampf g, GLclampf b)
{
    _head.mutateColor(r, g, b);
    _torso.mutateColor(r, g, b);
    _lwing.mutateColor(r, g, b);
    _lcanon.mutateColor(r, g, b);
    _rwing.mutateColor(r, g, b);
    _rcanon.mutateColor(r, g, b);
}

void Ship::wingMove()
{
    if (_rwing.degree() - 45 + 1 > MAX_ROTATION)
    {
        _rotateDir = -1.0;
    }
    if (_rcanon.degree() - 45 - 1 < 0)
    {
        _rotateDir = +1.0;
    }

    _rwing.rotate(_rotateDir);
    _rcanon.rotate(_rotateDir);
    _lwing.rotate(-_rotateDir);
    _lcanon.rotate(-_rotateDir);


}

/**
 * @brief _player 가 bullet에 맞았는지 체크 및 목숨/컬러 변
 * @brief game mode에 따라 나눠짐.
 * @parmas bullet_list - Bullet*를 담고 있는 list의 주소
 */

void Player::HitBullet(list<Bullet*>* bullet_list)
{
    list<Bullet*>::iterator itr;
    bool isHit;
    int damage = 1;
    Bullet* bullet;

    switch(mode) {
        case 'c':
/* if "c mode" _player don't die */
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
                    mutateColor(0, 0, -0.1 * damage);
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

void Player::HitItem(list<Item*>* items)
{
    bool isHit;
    char type = 4;
    list<Item*>::iterator itr;
    Item * item;

    srand(time(NULL));

    int item_type;

    itr = items->begin();
    while(itr != items->end()) {
        isHit = hit(*itr);
        if (isHit) {
            item_type = rand()%11;

            if (item_type == 0){
                /** Bomb : Game Over  10% */
                _numLife = 0;
                type = ITEM_BOMB;
            } else if (item_type >= 1 && item_type < 7) {
                /** addition Bullet  60% */
                if (_numBullet < MAX_BULLET) {
                    _numBullet += 1;
                }
                type = ITEM_INCR;
            } else if (item_type >= 7 && item_type <= 10) {
                /** delete one Bullet 40% */
                if (_numBullet > 1) {
                    _numBullet -= 1;
                }
                type = ITEM_DESC;
            }

            item = *itr;
            items->erase(itr++);
            game->newItemLog(type);
            delete (item);
        }
        else
            ++itr;
    }

}

/**
 * @brief 키보드 입력 되었을 때 handler
 * @parmas char(key) 눌린 키 종류 (space_bar, direction)
 */

list<Bullet *> Player::keyHandler()
{
    list<Bullet*> bullets;
    float dx = 0, dy = 0;

    float x, y;
    x = this->_torso.x();
    y = this->_torso.y();

    std::vector<std::vector<float>> pos = {
            {x, y+2*_size_torso},
            {x, y-2*_size_torso},
            {x-4*_size_torso, y},
            {x+4*_size_torso, y}
    };

    if(this->inputKey['S']){
        if (mode != 'f')
            bullets = shot();
        if(!bullets.empty()) {
            for(Bullet* ptr: bullets){
                player_bullets.push_back(ptr);
            }
        }
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

    for(int i=0; i < 4; i++)
    {
        float x = pos[i][0], y = pos[i][1];

        if ((dx != 0) && ((x+dx > 1.0) || (x+dx < -1.0)))
            return {};
        if ((dy != 0) && ((y+dy > 1.0) || (y+dy < -1.0)))
            return {};
    }
    _torso.move(dx, dy); /** move base object */
    return bullets;
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

    float x, y;
    /** it rotated 180 */
    x = this->_torso.x();

    std::vector<std::vector<float>> pos = {
            {x, y+2*_size_torso},
            {x, y-2*_size_torso},
            {x-4*_size_torso, y},
            {x+4*_size_torso, y}
    };

    for(int i=0; i < 4; i++)
    {
        float x = pos[i][0];

        if(x+dx > 1.0 || x+dx < -1.0 )
        {
            return;
        }
    }

    _torso.move(dx, 0);

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
        case 'c':
/** c mode일 때 적은 1대만 맞아도 죽음 */

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

