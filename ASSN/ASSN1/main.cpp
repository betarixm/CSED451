#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <glm/vec3.hpp>
#include "Ship.h"

char mode = 'n'; /* n : normal,  f: fail, c: king-god */
static list<Bullet*> enemy_bullets;
static list<Bullet*> player_bullets;
static Player* player;
Enemy* enemy;

/**
 * @brief 우주선/총알 display by Double buffer
 */
void renderScene(void) {
    list<Bullet*>::iterator itr;

    glClear(GL_COLOR_BUFFER_BIT);
    player->display();
    enemy->display();

    for(itr = enemy_bullets.begin(); itr != enemy_bullets.end(); itr++)
        (*itr)->display();
    for(itr = player_bullets.begin(); itr != player_bullets.end(); itr++)
        (*itr)->display();


    glutSwapBuffers();
}
 /**
  * @breif 방향키 입력 핸들러
  * @params key 입력키 종류
  */
void specialKeyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            player->keyHandler('U');
            break;
        case GLUT_KEY_DOWN:
            player->keyHandler('D');
            break;
        case GLUT_KEY_LEFT:
            player->keyHandler('L');
            break;
        case GLUT_KEY_RIGHT:
            player->keyHandler('R');
            break;
    }
    glutPostRedisplay();
}
/**
 * @brief 스페이스바/f/c 입력 핸들
 * @param key 입력키 종
 */
void keyboard(unsigned char key, int x, int y)
{
    Bullet* bullet;

    switch(key){
        case 32: /* space bar */
            bullet = player->keyHandler('S');
            if(bullet != NULL)
                player_bullets.push_back(bullet);
            break;
        case 'f':
            if(mode == 'f')
                mode = 'n';
            else
                mode = 'f';
            break;

        case 'c':
            if(mode == 'c')
                mode = 'n';
            else
                mode = 'c';
            break;
    }
    glutPostRedisplay();
}
/**
 * @brief 일정초마다 총알 이동 및 충돌 체크
 */
void timerFunc2(int value)
{
    list<Bullet*>::iterator itr;
    Bullet* bullet;

    itr = enemy_bullets.begin();
    while(itr != enemy_bullets.end())
    {
        bullet = *itr;
        bullet->move(0, -0.1);  /** window 밖으로 나가는 것 방지 */
        if((*itr)->y() < -1.0)
        {
            enemy_bullets.erase(itr++);
            delete(bullet);
        }
        else
            ++itr;
    }

    itr = player_bullets.begin();
    while(itr != player_bullets.end())
    {
        bullet = *itr;
        bullet->move(0, +0.1);
        if((*itr)->y() > 1.0)           /** window 밖으로 나가는 것 방지 */
        {
            player_bullets.erase(itr++);
            delete(bullet);
        }
        else
            ++itr;
    }

    player->checkHit(&enemy_bullets);
    enemy->checkHit(&player_bullets);

    glutPostRedisplay();
    glutTimerFunc(30, timerFunc2, 1);
}
/**
 * @brief 일정초마다 enemy moving & 총알 발
 * @param value 7번에 1번씩 총알 발사를 위한 counter
 */
void timerFunc(int value)
{
    Bullet* new_bullet;

    enemy->randomMoveHandler();

    if(value == 0) {    /* 총알 발사 */
        new_bullet = enemy->shot();
        enemy_bullets.push_back(new_bullet);
    }

    glutPostRedisplay();
    glutTimerFunc(300, timerFunc, (value+1)%7);
    if(value==-1)
        glutTimerFunc(30, timerFunc2, 1);

}

int main(int argc, char **argv) {


    player = new Player(3, 0.75,-0.75, 0.3, 0, 0, 1, 180.0);
    enemy = new Enemy(1, -0.75, 0.75, 0.3, 1, 1, 1, 0);


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("beta&ches");
    glutDisplayFunc(renderScene);

    /** eunsue modified start*/
    glutSpecialFunc(specialKeyboard);
    glutTimerFunc(100, timerFunc, -1);
    glutKeyboardFunc(keyboard);
    /** eunsue modified end */

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glewInit();
    glutMainLoop();

}