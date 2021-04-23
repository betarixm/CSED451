//
// Created by 최은수 on 2021/03/04.
//

#include "Ship.h"

/**
 * declared in main.cpp
 * n : normal, f : fail, c : king-god
 */
extern char mode;
extern int frontCamera;
#define dist 0.05  /** ship 움직일때 거단위 */
#define PIE 3.1415926
#define MAX_BULLET 5

extern Game *game;
extern list<Bullet *> player_bullets;

Ship::Ship(int _numLife, float x, float y, float size_torso, GLclampf r, GLclampf g, GLclampf b, float degree,
           int numBullet): _obj(_path, x, y, 0, degree, r, g, b) {
    this->_numLife = _numLife;
    this->_size_torso = size_torso;
    this->_numBullet = numBullet;

    this->_baseScene = new GroupNode;
    this->_baseScene->addChild(_obj.groupNode());
}

void Ship::display(bool isBlack) {
    this->_baseScene->display(isBlack);
}


/**
 * @brief bullet 과 Ship이 충돌했는지 체크
 * @brief bullet의 네 점중 하나라도 detection box 내부에 존재하는지 여부로 체크.
 * @param bullet
 * @return 충돌여부 (true/false)
 */

bool Ship::hit(Bullet *bullet) {
    float x = _obj.x();
    float y = _obj.y();
    glm::vec3 max, min;
    bool isHit = true;
    max = _obj.max();
    min = _obj.min();

    vector<vector<float>> detection_box = {
            {x + min[0], y + max[1]},
            {x + max[0], y + max[1]},
            {x + max[0], y + min[1]},
            {x + min[0], y + min[1]}
    };

    float x_b = bullet->x();
    float y_b = bullet->y();



    if (detection_box[0][0] > x_b || detection_box[0][1] < y_b)
        isHit = false;
    if (detection_box[1][0] < x_b || detection_box[1][1] < y_b)
        isHit = false;
    if (detection_box[2][0] < x_b || detection_box[2][1] > y_b)
        isHit = false;
    if (detection_box[3][0] > x_b || detection_box[3][1] > y_b)
        isHit = false;

    return isHit;
}


/**
 * @brief 총알이 발사될 때 bullet 인스턴스 생성
 * @return Bullet*
 */

list<Bullet *> Ship::shot() {
    GLclampf *color = _obj.color();

    float x = _obj.x();
    float y = _obj.y();
    int numBullet = _numBullet;
    float degree;

    list<Bullet *> result;

    if (numBullet % 2 != 0) {
        auto *b = new Sphere(50, 50, 0.01f, x, y, 0, 0, 1, 0, 0);
        b->setDirection(0, 0.1, 0);
        numBullet -= 1;
        result.push_back(b);
    }
    if (numBullet == 0)
        return result;

    degree = (90.0 / (numBullet / 2 + 1))*PIE/180;

    for (int i = 1; i <= numBullet / 2; i++) {
        auto *b = new Sphere(50, 50, 0.01, x, y, 0, 0, 1, 0, 0);
        b->setDirection(0.1*cos(degree*i), 0.1*sin(degree*i), 0);
        result.push_back(b);

        b = new Sphere(50, 50, 0.01, x, y, 0, 0, 1, 0, 0);
        b->setDirection(-0.1*cos(degree*i), 0.1*sin(degree*i), 0);
        result.push_back(b);
    }

    return result;

}

void Ship::mutateColor(GLclampf r, GLclampf g, GLclampf b) {
    // TODO:: Set Color
}

/**
 * @brief _player 가 bullet에 맞았는지 체크 및 목숨/컬러 변
 * @brief game mode에 따라 나눠짐.
 * @parmas bullet_list - Bullet*를 담고 있는 list의 주소
 */

void Player::HitBullet(list<Bullet *> *bullet_list) {
    list<Bullet *>::iterator itr;
    bool isHit;
    int damage = 1;
    Bullet *bullet;

    switch (mode) {
        case 'c':
/* if "c mode" _player don't die */
            return;
        case 'f':
            damage = _numLife;
        case 'n':
            itr = bullet_list->begin();
            while (itr != bullet_list->end()) {
                isHit = hit(*itr);
                if (isHit) {
                    _numLife -= damage;
                    mutateColor(0, 0, -0.1 * damage);
                    bullet = *itr;
                    bullet_list->erase(itr++);
                    delete (bullet);
                } else
                    ++itr;
/* 여러대 맞아도 목숨 다 깎이면 게임 종료 */
                if (_numLife == 0)
                    return;
            }
    }
}

void Player::HitItem(list<Item *> *items) {
    bool isHit;
    char type = 4;
    list<Item *>::iterator itr;
    Item *item;

    srand(time(NULL));

    int item_type;

    itr = items->begin();
    while (itr != items->end()) {
        isHit = hit(*itr);
        if (isHit) {
            item_type = rand() % 11;

            if (item_type == 0) {
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
        } else
            ++itr;
    }

}

/**
 * @brief 키보드 입력 되었을 때 handler
 * @parmas char(key) 눌린 키 종류 (space_bar, direction)
 */

list<Bullet *> Player::keyHandler() {
    list<Bullet *> bullets;
    float dx = 0, dy = 0;

    float x, y;
    x = this->_obj.x();
    y = this->_obj.y();

    glm::vec3 max, min;
    bool isHit = true;
    max = _obj.max();
    min = _obj.min();

    std::vector<std::vector<float>> pos = {
            {x + min[0], y + max[1]},
            {x + max[0], y + max[1]},
            {x + max[0], y + min[1]},
            {x + min[0], y + min[1]}
    };

    if (this->inputKey['S']) {
        if (mode != 'f')
            bullets = shot();
        if (!bullets.empty()) {
            for (Bullet *ptr: bullets) {
                player_bullets.push_back(ptr);
            }
        }
    }

    if(this->inputKey['U'] || this->inputKey['D'] ||this->inputKey['L'] || this->inputKey['R'] ) {
        if (this->inputKey['U']) {
            dy = dist;
        }

        if (this->inputKey['D']) {
            dy = -1 * dist;
        }

        if (this->inputKey['L']) {
            dx = -1 * dist;
        }

        if (this->inputKey['R']) {
            dx = dist;
        }


/** window 밖으로 넘어가는지 체크 */

        for (int i = 0; i < 4; i++) {
            float x = pos[i][0], y = pos[i][1];

            if ((dx != 0) && ((x + dx > 1.0) || (x + dx < -1.0)))
                return {};
            if ((dy != 0) && ((y + dy > 1.0) || (y + dy < -1.0)))
                return {};
        }
        _obj.move(dx, dy); /** move base object */

        x = this->_obj.x();
        y = this->_obj.y();


        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(x, y + frontCamera * 0.05 - 0.5, 0.2, x, y+0.7, 0.1, 0, 0, 1);
    }
    return bullets;
}


/** 좌/우/멈춤 중 _enemy 움직임 랜덤 선
 */

void Enemy::randomMoveHandler() {
    float dx = 0;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(-1, 1);
    dx = dist * dis(gen);

    float x, y;
    /** it rotated 180 */
    x = this->_obj.x();
    y = this->_obj.y();

    std::vector<std::vector<float>> pos = {
            {x,                   y + 2 * _size_torso},
            {x,                   y - 2 * _size_torso},
            {x - 4 * _size_torso, y},
            {x + 4 * _size_torso, y}
    };

    for (int i = 0; i < 4; i++) {
        float x = pos[i][0];
        if (x + dx > 1.0 || x + dx < -1.0) {
            return;
        }
    }

    _obj.move(dx, 0);

}

/**
 * @brief _enemy 가 bullet에 맞았는지 체크 및 목숨/컬러 변
 * @brief game mode에 따라 나눠짐.
 * @parmas bullet_list - Bullet*를 담고 있는 list의 주소
 */

void Enemy::checkHit(list<Bullet *> *bullet_list) {
    list<Bullet *>::iterator itr;
    bool isHit;
    int damage = 1;
    Bullet *bullet;

    switch (mode) {
        case 'c':
/** c mode일 때 적은 1대만 맞아도 죽음 */

            damage = _numLife;
        default:
            itr = bullet_list->begin();
            while (itr != bullet_list->end()) {
                isHit = hit(*itr);
                if (isHit) {
                    _numLife -= damage;
                    bullet = *itr;
                    bullet_list->erase(itr++);
                    delete (bullet);
                } else
                    ++itr;;


/** 여러대 맞아도 목숨 다 깎이면 게임 종료 */

                if (_numLife == 0)
                    return;
            }
    }
}

