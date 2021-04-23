#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>

//#include "Game.h"
#include "Stellar.h"
#include "Ship.h"

char mode = 'n'; /* n : normal,  f: fail, c: king-god */
bool isHiddenLineRemoval = false;
int frontCamera = -1;
static list<Bullet*> enemy_bullets;
list<Bullet*> player_bullets;
static list<Item*> item_list;

vector<Stellar*> stellar_vec;

Game* game;
Grid* grid;
Grid* boundary;

Sphere *sphere;
Sphere *cube;

void microRenderScene(bool isBlack) {
    list<Bullet*>::iterator itr;
    game->display(isBlack);

    if(!(game->isGameOver() || game->isGameWin()) ) {
        grid->display(isBlack);
        for(auto & i : stellar_vec) {
            i->display(isBlack);
            boundary->display(isBlack);
        }
        for (itr = enemy_bullets.begin(); itr != enemy_bullets.end(); itr++)
            (*itr)->display(isBlack);
        for (itr = player_bullets.begin(); itr != player_bullets.end(); itr++)
            (*itr)->display(isBlack);
        for (itr = item_list.begin(); itr != item_list.end(); itr++)
            (*itr)->display(isBlack);
    }
}

/**
 * @brief 우주선/총알 display by Double buffer
 */
void renderScene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    microRenderScene(false);

    if(isHiddenLineRemoval) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 5.0);
        microRenderScene(true);
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    glutSwapBuffers();
}
/**
 * @breif 방향키 입력 핸들러
 * @params key 입력키 종류
 */
void onSpecialKeyDown(int key, int x, int y)
{
    if(!game->player()){
        return;
    }

    switch(key)
    {
        case GLUT_KEY_UP:
            game->player()->keyDown('U');
            break;
        case GLUT_KEY_DOWN:
            game->player()->keyDown('D');
            break;
        case GLUT_KEY_LEFT:
            game->player()->keyDown('L');
            break;
        case GLUT_KEY_RIGHT:
            game->player()->keyDown('R');
            break;
    }
}

void onSpecialKeyUp(int key, int x, int y) {
    if(!game->player()){
        return;
    }

    switch(key)
    {
        case GLUT_KEY_UP:
            game->player()->keyUp('U');
            break;
        case GLUT_KEY_DOWN:
            game->player()->keyUp('D');
            break;
        case GLUT_KEY_LEFT:
            game->player()->keyUp('L');
            break;
        case GLUT_KEY_RIGHT:
            game->player()->keyUp('R');
            break;
        default:
            break;
    }
}
/**
 * @brief 스페이스바/f/c 입력 핸들
 * @param key 입력키 종
 */
void onKeyDown(unsigned char key, int x, int y)
{
    list<Bullet*> bullets;


    switch(key){
        case 32: /* space bar */
            if(!game->player()){ break; }
            game->player()->keyDown('S');
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

        case 'r':
            isHiddenLineRemoval = !isHiddenLineRemoval;
            break;

        case 'v':
            frontCamera = -1*frontCamera;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            x = game->player()->x();
            y = game->player()->y();

            gluLookAt(x, y + frontCamera*0.05, 0.2, x, y+0.7, 0.1, 0, 0, 1);


        default:
            break;
    }
}

void onKeyUp(unsigned char key, int x, int y){
    if(key == 32) {
        key = 'S';
    }
    game->player()->keyUp(key);
}
/**
 * @brief 일정초마다 총알 이동 및 충돌 체크
 */
void timerBulletMoveHit(int value)
{
    list<Bullet*>::iterator itr;
    Bullet* bullet;
    vector<float> dir;
    float x, y;

    itr = enemy_bullets.begin();
    while(itr != enemy_bullets.end())
    {
        bullet = *itr;
        dir = bullet->direction();
        bullet->move(-1*dir[0], -1*dir[1]);  /** window 밖으로 나가는 것 방지 */
        x = bullet->x();
        y = bullet->y();

        if ((x > 1.0) || (x < -1.0))
        {
            bullet->setDirection(-1*dir[0], dir[1], 0);
        }
        if(y < -1.0)
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

        dir = bullet->direction();
        bullet->move(dir[0], dir[1]);  /** window 밖으로 나가는 것 방지 */
        x = bullet->x();
        y = bullet->y();

        if ((x > 1.0) || (x < -1.0))
        {
            bullet->setDirection(-1*dir[0], dir[1], 0);
        }

        if(y > 1.0)           /** window 밖으로 나가는 것 방지 */
        {
            player_bullets.erase(itr++);
            delete(bullet);
        }
        else
            ++itr;
    }


    if(game->player()){
        game->player()->HitBullet(&enemy_bullets);
        game->player()->HitItem(&item_list);
    }

    if(game->enemy()){
        game->enemy()->checkHit(&player_bullets);
    }

    glutTimerFunc(30, timerBulletMoveHit, 1);
}
/**
 * @brief 일정초마다 _enemy moving & 총알 발
 * @param value 7번에 1번씩 총알 발사를 위한 counter
 */
void timerBulletEnemyShot(int value)
{
    list<Bullet*>::iterator itr;

    if(game->enemy()){
        list<Bullet*> new_bullet;

        game->enemy()->randomMoveHandler();

        if(value == 0) {    /* 총알 발사 */
            new_bullet = game->enemy()->shot();
            for(Bullet* ptr : new_bullet)
                enemy_bullets.push_back(ptr);
        }

    }

    Item *item;
    itr = item_list.begin();
    while(itr != item_list.end())
    {
        item = *itr;
        item->move(0, -0.1);
        if((*itr)->y() < -1.0)           /** window 밖으로 나가는 것 방지 */
        {
            item_list.erase(itr++);
            delete(item);
        }
        else
            ++itr;
    }

    glutTimerFunc(300, timerBulletEnemyShot, (value + 1) % 7);
    if(value==-1)
        glutTimerFunc(30, timerBulletMoveHit, 1);

}

void timerDefault(int value){
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
    for(auto & i : stellar_vec){
        i->tick();
    }
    glutTimerFunc(1, timerStellar, -1);
}



int main(int argc, char **argv) {
    game = new Game();
    boundary = new Grid(0.1f, 0.1f, 20, 20, 0, 0, -0.295f, 0, 0.0f, 0.5f, 1.0f);
    grid = new Grid(0.1f, 0.1f, 30, 30, 0, 0, -0.3f, 0, 1.0f, 1.0f, 1.0f);
    initStellar();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 800);
    glutCreateWindow("beta&ches");
    glutDisplayFunc(renderScene);

    glutSpecialFunc(onSpecialKeyDown);
    glutSpecialUpFunc(onSpecialKeyUp);
    glutTimerFunc(1, timerDefault, -1);
    glutTimerFunc(100, timerBulletEnemyShot, -1);
    glutTimerFunc(1, timerRedisplay, -1);
    glutTimerFunc(1, timerStellar, -1);
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // https://flex.phys.tohoku.ac.jp/texi/glut/glutStrokeCharacter.3xglut.html

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::mat4x4 proj = glm::perspective(90.0f, 1.0f, 0.001f, 100.0f);
    glLoadMatrixf(glm::value_ptr(proj));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.60, -1.5, 0.2, 0.60, 0, 0.1, 0, 0, 1);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glewInit();
    glutMainLoop();

}