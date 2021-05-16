#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shader.h"
#include "Stellar.h"
#include "Light.h"

#include "keyboard.h"
#include "timer.h"
#include "render.h"

void initGraphic(){
    initShader();
    initGame();
    initGrid();
    initStellar();
    initLight();
}

int main(int argc, char **argv) {
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
    glutTimerFunc(1, timerDirLight, 0);
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // https://flex.phys.tohoku.ac.jp/texi/glut/glutStrokeCharacter.3xglut.html

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glewExperimental = GL_TRUE;
    glewInit();

    initGraphic();
    glutMainLoop();
}