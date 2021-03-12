#ifndef CSED451_GAME_H
#define CSED451_GAME_H

#include "Ship.h"

#define PLAYER_DEGREE 180.0
#define ENEMY_DEGREE 0.0

#define NUM_PLAYER 1
#define NUM_ENEMY 5
#define LIFE_PLAYER 3

class Game {
private:
    int numPlayer = NUM_PLAYER, numEnemy = NUM_ENEMY;
    int numLifePlayer = LIFE_PLAYER, numLifeEnemy = 1;
    bool isPlayerDead = true, isEnemyDead = true, _isGameOver = false, _isGameWin = false;
    Player* _player = nullptr;
    Enemy* _enemy = nullptr;

protected:
public:
    Game(){
        this->spawnPlayer();
        this->spawnEnemy();
    }

    void spawnPlayer(){
        if(isPlayerDead && this->numPlayer--){
            delete this->_player;
            this->_player = new Player(this->numLifePlayer, 0.75, -0.75, 0.3, 0, 0, 1, PLAYER_DEGREE);
            this->isPlayerDead = false;
        }
    }

    void spawnEnemy(){
        if(isEnemyDead && this->numEnemy--){
            delete this->_enemy;

            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0, 0.75);

            this->_enemy = new Enemy(this->numLifeEnemy, -0.75, 0.75, 0.3, dis(gen), dis(gen), 1, ENEMY_DEGREE);
            this->numLifeEnemy++;
            this->isEnemyDead = false;
        }
    }

    void tick() {
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

    void displayInfo() {
        std::string strNumEnemy;
        std::string strNumPlayer;

        std::string strLifeEnemy;
        std::string strLifePlayer;

        for(int i = 0; i < numEnemy; i++){ strNumEnemy += "O"; }
        for(int i = 0; i < NUM_ENEMY - numEnemy; i++){ strNumEnemy += "X"; }

        for(int i = 0; i < numPlayer; i++){ strNumPlayer += "O"; }
        for(int i = 0; i < NUM_PLAYER - numPlayer; i++){ strNumPlayer += "X"; }

        for(int i = 0; i < this->_enemy->numLife(); i++){ strLifeEnemy += "O"; }
        for(int i = 0; i < numLifeEnemy - 1 - this->_enemy->numLife(); i++){ strLifeEnemy += "X"; }

        for(int i = 0; i < this->_player->numLife(); i++){ strLifePlayer += "O"; }
        for(int i = 0; i < numLifePlayer - this->player()->numLife(); i++){strLifePlayer += "X";}

        std::string info =
                " ENEMY: " + strNumEnemy + " / "
                + "PLAYER: " + strNumPlayer + " / "
                + " ENEMY LIFE: " + strLifeEnemy + " / "
                + "PLAYER LIFE: " + strLifePlayer;

        glColor3f(0, 1, 0);
        glRasterPos2f(-0.93, 0.9);

        for(char i : info){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);
        }
    }

    void display() {
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

    Player* player() const{
        return this->_player;
    }

    Enemy* enemy() const{
        return this->_enemy;
    }

    bool isGameOver() const {
        return this->_isGameOver;
    };

    bool isGameWin() const {
        return this->_isGameWin;
    }
};


#endif //CSED451_GAME_H
