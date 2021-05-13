#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shader.h"
#include "Stellar.h"
#include "Ship.h"

#include "keyboard.h"
#include "timer.h"
#include "render.h"

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

void initGraphic()
{
    vector<vector<float>> vertices = {
            {0.1, 0.1, 0.0},
            {0.2, 0.2, 0.0},
            {0.0, 0.0, 0.0}
    };

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