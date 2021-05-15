#include "render.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "Ship.h"

extern Game* game;
extern list<Bullet*> enemy_bullets;
extern list<Bullet*> player_bullets;
extern list<Item*> item_list;
extern Grid* grid;
extern Grid* boundary;
extern deque<glm::mat4> ModelView;
extern deque<glm::mat4> Projection;
extern Shader* shader;

bool isHiddenLineRemoval = false;
int frontCamera = -1;

void lookAt(float x, float y, int _frontCamera) {
    glm::vec3 viewPos = glm::vec3(x, y + _frontCamera*0.22, 0.15);
    glm::mat4 P = glm::perspective(90.0f, 1.0f, 0.001f, 100.0f);
    glm::mat4 cur_p = Projection.back();
    Projection.pop_back();
    Projection.push_back(P*cur_p);

    glm::mat4 C = glm::lookAt(viewPos, glm::vec3(x, y+0.7, 0.1), glm::vec3(0, 0, 1));
    glm::mat4 cur_C = ModelView.back();
    shader->uniform4m("View", C);
    shader->uniform3v("viewPos", viewPos);
    // ModelView.pop_back();
    // ModelView.push_back(C*cur_C);
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

        for(auto & i : Stellar::stellarVec) {
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