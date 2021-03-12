//
// Created by 민재 on 2021/03/12.
//

#include "Game.h"

extern char mode;

Game::Game() {
    this->spawnPlayer();
    this->spawnEnemy();
}

Game::~Game() {
    delete this->_enemy;
    delete this->_player;
}

void Game::spawnPlayer() {
    if(isPlayerDead && this->numPlayer--){
        delete this->_player;
        this->_player = new Player(this->numLifePlayer, 0.75, -0.75, 0.3, 0, 0, 1, PLAYER_DEGREE);
        this->isPlayerDead = false;
    }
}

void Game::spawnEnemy() {
    if(isEnemyDead && this->numEnemy--){
        delete this->_enemy;

        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 0.75);

        this->_enemy = new Enemy(this->numLifeEnemy, 0, 0.75, 0.3, dis(gen), dis(gen), 1, ENEMY_DEGREE);
        this->numLifeEnemy++;
        this->isEnemyDead = false;
    }
}

void Game::tick() {
    if(_enemy->numLife() == 0){
        this->isEnemyDead = true;
        if(this->numEnemy){
            this->spawnEnemy();
        } else {
            this->_isGameWin = true;
        }
    }

    if(_player->numLife() == 0){
        this->isPlayerDead = true;
        if(this->numPlayer){
            this->spawnPlayer();
        } else {
            this->_isGameOver = true;
        }
    }
}

void Game::displayInfo() {
    std::string strNumEnemy, strNumPlayer;
    std::string strLifeEnemy, strLifePlayer;

    std::string strMode;

    for(int i = 0; i < numEnemy; i++){ strNumEnemy += "O"; }
    for(int i = 0; i < NUM_ENEMY - numEnemy; i++){ strNumEnemy += "X"; }

    for(int i = 0; i < numPlayer; i++){ strNumPlayer += "O"; }
    for(int i = 0; i < NUM_PLAYER - numPlayer; i++){ strNumPlayer += "X"; }

    for(int i = 0; i < this->_enemy->numLife(); i++){ strLifeEnemy += "O"; }
    for(int i = 0; i < numLifeEnemy - 1 - this->_enemy->numLife(); i++){ strLifeEnemy += "X"; }

    for(int i = 0; i < this->_player->numLife(); i++){ strLifePlayer += "O"; }
    for(int i = 0; i < numLifePlayer - this->player()->numLife(); i++){strLifePlayer += "X";}


    if(mode == 'f') {
        strMode = "ALL FALE MODE";
        glColor3f(1, 0, 0);
    } else if (mode == 'c') {
        strMode = "ALL PASS MODE";
        glColor3f(0, 1, 0);
    } else {
        strMode = "NORMAL MODE";
        glColor3f(0, 1, 1);
    }

    std::string strInfo = strMode + " / "
                          + "ENEMY: " + strNumEnemy + " / "
                          + "PLAYER: " + strNumPlayer + " / "
                          + "ENEMY LIFE: " + strLifeEnemy + " / "
                          + "PLAYER LIFE: " + strLifePlayer;

    glRasterPos2f(-0.93, 0.9);

    for(char i : strInfo){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);
    }
}

void Game::display() {
    if(_isGameOver || _isGameWin){
        std::string msg = _isGameWin ? "WIN!" : "GAME OVER!";

        glLoadIdentity();

        glColor3f(1, 0, 0);
        glRasterPos2f(0, 0);

        for(char i : msg) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);
        }
        return;
    }

    this->displayInfo();

    if(!isPlayerDead && this->_player){
        _player->display();
    }
    if(!isEnemyDead && this->_enemy){
        _enemy->display();
    }
}

Player *Game::player() const {
    return this->_player;
}

Enemy *Game::enemy() const {
    return this->_enemy;
}

bool Game::isGameOver() const {
    return this->_isGameOver;
}

bool Game::isGameWin() const {
    return this->_isGameWin;
}
