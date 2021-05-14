#include "keyboard.h"
#include "Ship.h"

extern Game *game;
extern char mode;
extern bool isHiddenLineRemoval;
extern int frontCamera;

void onSpecialKeyDown(int key, int x, int y) {
    if (!game->player()) {
        return;
    }

    switch (key) {
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
        default:
            break;
    }
}

void onSpecialKeyUp(int key, int x, int y) {
    if (!game->player()) {
        return;
    }

    switch (key) {
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

void onKeyDown(unsigned char key, int x, int y) {
    list<Bullet *> bullets;


    switch (key) {
        case 32: /* space bar */
            if (!game->player()) { break; }
            game->player()->keyDown('S');
            break;
        case 'f':
            if (mode == 'f')
                mode = 'n';
            else
                mode = 'f';
            break;

        case 'c':
            if (mode == 'c')
                mode = 'n';
            else
                mode = 'c';
            break;

        case 'r':
            isHiddenLineRemoval = !isHiddenLineRemoval;
            break;

        case 'v':
            frontCamera = -1 * frontCamera;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

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