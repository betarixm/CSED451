#ifndef CSED451_GAME_H
#define CSED451_GAME_H

#include "Ship.h"

#define PLAYER_DEGREE 0.0
#define ENEMY_DEGREE 180.0

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
    Game();

    ~Game();

    void spawnPlayer();

    void spawnEnemy();

    Item* tick();

    void displayInfo();

    void display();

    Player* player() const;

    Enemy* enemy() const;

    bool isGameOver() const;;

    bool isGameWin() const;
};


#endif //CSED451_GAME_H
