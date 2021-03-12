#ifndef CSED451_GAME_H
#define CSED451_GAME_H

#include "Ship.h"

#define PLAYER_DEGREE 180.0
#define ENEMY_DEGREE 0.0

class Game {
private:
    int numPlayer = 1, numEnemy = 5;
    int numLifePlayer = 3, numLifeEnemy = 1;
    Player* _player = nullptr;
    Enemy* _enemy = nullptr;

protected:
public:
    Game(){
        this->spawnPlayer();
        this->spawnEnemy();
    }

    void spawnPlayer(){
        if(this->numPlayer--){
            delete this->_player;
            this->_player = new Player(this->numLifePlayer, 0.75, -0.75, 0.3, 0, 0, 1, PLAYER_DEGREE);
        }
    }

    void spawnEnemy(){
        if(this->numEnemy--){
            delete this->_enemy;
            this->_enemy = new Enemy(this->numLifeEnemy, -0.75, 0.75, 0.3, 1, 1, 1, ENEMY_DEGREE);
            this->numLifeEnemy++;
        }
    }

    void tick() {
        if(_enemy->numLife() == 0){
            if(this->numEnemy){
                this->spawnEnemy();
            } else {
                delete this->_enemy;
                delete this->_player;


                // TODO: Game Win;
            }
        }

        if(_player->numLife() == 0){
            if(this->numPlayer){
                this->spawnPlayer();
            } else {
                delete this->_enemy;
                delete this->_player;

                // TODO: Game Over;
            }
        }
    }

    void display() {
        if(this->_player){_player->display();}
        if(this->_enemy){_enemy->display();}
    }

    Player* player() const{
        return this->_player;
    }

    Enemy* enemy() const{
        return this->_enemy;
    }
};


#endif //CSED451_GAME_H
