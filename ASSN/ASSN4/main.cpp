#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Shader.h"
#include "Stellar.h"
#include "Ship.h"

#include "keyboard.h"
#include "timer.h"


char mode = 'n'; /* n : normal,  f: fail, c: king-god */
bool isHiddenLineRemoval = false;
int frontCamera = -1;
list<Bullet*> enemy_bullets;
list<Bullet*> player_bullets;
list<Item*> item_list;

vector<Stellar*> stellar_vec;

Game* game;
Grid* grid;
Grid* boundary;

Sphere *sphere;
Sphere *cube;

deque<glm::mat4> ModelView;
deque<glm::mat4> Projection;

Shader* shader;

void lookAt(float x, float y, int _frontCamera) {
    glm::mat4 P = glm::perspective(90.0f, 1.0f, 0.001f, 100.0f);
    glm::mat4 cur_p = Projection.back();
    Projection.pop_back();
    Projection.push_back(P*cur_p);
    
    glm::mat4 C = glm::lookAt(glm::vec3(x, y + _frontCamera*0.22, 0.15), glm::vec3(x, y+0.7, 0.1), glm::vec3(0, 0, 1));
    glm::mat4 cur_C = ModelView.back();
    ModelView.pop_back();
    ModelView.push_back(C*cur_C);
}

void microRenderScene(bool isBlack) {
    list<Bullet*>::iterator itr;
    
    ModelView = { glm::mat4(1.0f) };
    Projection = { glm::mat4(1.0f) };

    if(!(game->isGameOver() || game->isGameWin()) ) {
        lookAt(game->player()->x(), game->player()->y(), frontCamera);
        game->display(isBlack);
 
        grid->display(isBlack);
        
        boundary->display(isBlack);
        
        for(auto & i : stellar_vec) {
            i->display(isBlack);
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
    shader->use();

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

void initGraphic()
{
    vector<vector<float>> vertices = {
            {0.1, 0.1, 0.0},
            {0.2, 0.2, 0.0},
            {0.0, 0.0, 0.0}
    };

    // Uniform Matrix init

    game = new Game();
   
    boundary = new Grid(0.1f, 0.1f, 20, 24, 0, 0, -0.295f, 0, 0.0f, 0.5f, 1.0f);

    grid = new Grid(0.1f, 0.1f, 30, 40, 0, 0, -0.3f, 0, 1.0f, 1.0f, 1.0f);

    initStellar();
}


int main(int argc, char **argv) {
    shader = new Shader((char *)"Shader.vert", (char *)"Shader.frag");

    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glewExperimental = GL_TRUE;
    glewInit();

    initGraphic();
    glutMainLoop();
}