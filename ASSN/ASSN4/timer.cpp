#include "timer.h"

#include <cmath>
#include "Light.h"

extern Game *game;

extern list<Bullet *> enemy_bullets;
extern list<Bullet *> player_bullets;
extern list<Item *> item_list;

extern char mode;

/**
 * @brief 일정초마다 총알 이동 및 충돌 체크
 */
void timerBulletMoveHit(int value) {
    list<Bullet *>::iterator itr;
    Bullet *bullet;
    vector<float> dir;
    float x, y;

    itr = enemy_bullets.begin();
    while (itr != enemy_bullets.end()) {
        bullet = *itr;
        dir = bullet->direction();
        bullet->move(-1 * dir[0], -1 * dir[1]);  /** window 밖으로 나가는 것 방지 */
        x = bullet->x();
        y = bullet->y();

        if ((x > 1.0) || (x < -1.0)) {
            bullet->setDirection(-1 * dir[0], dir[1], 0);
        }
        if (y < -1.0) {
            enemy_bullets.erase(itr++);
            delete (bullet);
        } else
            ++itr;
    }

    itr = player_bullets.begin();
    while (itr != player_bullets.end()) {
        bullet = *itr;
        bullet->move(0, +0.1);

        dir = bullet->direction();
        bullet->move(dir[0], dir[1]);  /** window 밖으로 나가는 것 방지 */
        x = bullet->x();
        y = bullet->y();

        if ((x > 1.0) || (x < -1.0)) {
            bullet->setDirection(-1 * dir[0], dir[1], 0);
        }

        if (y > 1.0)           /** window 밖으로 나가는 것 방지 */
        {
            player_bullets.erase(itr++);
            delete (bullet);
        } else
            ++itr;
    }


    if (game->player()) {
        game->player()->HitBullet(&enemy_bullets);
        game->player()->HitItem(&item_list);
    }

    if (game->enemy()) {
        game->enemy()->checkHit(&player_bullets);
    }

    glutTimerFunc(30, timerBulletMoveHit, 1);
}

/**
 * @brief 일정초마다 _enemy moving & 총알 발
 * @param value 7번에 1번씩 총알 발사를 위한 counter
 */
void timerBulletEnemyShot(int value) {
    list<Bullet *>::iterator itr;

    if (game->enemy()) {
        list<Bullet *> new_bullet;

        game->enemy()->randomMoveHandler();

        if (value == 0) {    /* 총알 발사 */
            new_bullet = game->enemy()->shot();
            for (Bullet *ptr : new_bullet)
                enemy_bullets.push_back(ptr);
        }

    }

    Item *item;
    itr = item_list.begin();
    while (itr != item_list.end()) {
        item = *itr;
        item->move(0, -0.1);
        if ((*itr)->y() < -1.0)           /** window 밖으로 나가는 것 방지 */
        {
            item_list.erase(itr++);
            delete (item);
        } else
            ++itr;
    }

    glutTimerFunc(300, timerBulletEnemyShot, (value + 1) % 7);
    if (value == -1)
        glutTimerFunc(30, timerBulletMoveHit, 1);

}

void timerDefault(int value) {
    Item *new_item = game->tick();
    if (new_item != nullptr)
        item_list.push_back(new_item);

    glutTimerFunc(1, timerDefault, -1);
}

void timerRedisplay(int value) {
    glutPostRedisplay();
    glutTimerFunc(1, timerRedisplay, -1);
}

void timerStellar(int value) {
    for (auto &i : Stellar::stellarVec) {
        i->tick();
    }
    glutTimerFunc(50, timerStellar, -1);
}

void timerDirLight(int value) {
    float theta = ((float)abs((value % 360) - 180) - 180.0f) * (float)PI / 180.0f;
    DirectionalLight::list[0]->setDirection(glm::vec3(RADIUS_DIR_LIGHT * cos(theta), RADIUS_DIR_LIGHT * sin(theta), 0));
    glutTimerFunc(5, timerDirLight, value + 1);
}