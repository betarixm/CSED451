#include <string>
#include "Game.h"
#include "Ship.h"

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
        this->_player = new Player(this->numLifePlayer, 0.60, -0.75, 0.1, 0, 0, 1, PLAYER_DEGREE);
        this->isPlayerDead = false;
    }
}

void Game::spawnEnemy() {
    if(isEnemyDead && this->numEnemy--){
        delete this->_enemy;

        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 0.75);

        this->_enemy = new Enemy(this->numLifeEnemy, 0, 0.75, 0.1, (GLclampf)dis(gen), (GLclampf)dis(gen), 1, ENEMY_DEGREE, this->numLifeEnemy);
        this->numLifeEnemy++;
        this->isEnemyDead = false;
    }
}

Item * Game::tick() {
    Item *item = nullptr;

    if(_enemy && _enemy->numLife() == 0){
        this->isEnemyDead = true;
        if(this->numEnemy){
            item = new Sphere(50, 50, 0.1, _enemy->x(), _enemy->y(), 0, 0, 1, 0, 0);
            this->spawnEnemy();
        } else {
            this->_isGameWin = true;
        }
    }

    if(_player && _player->numLife() == 0){
        this->isPlayerDead = true;
        if(this->numPlayer){
            this->spawnPlayer();
        } else {
            this->_isGameOver = true;
        }
    }
    return item;
}

void Game::displayInfo() {
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(_isGameOver || _isGameWin){
        std::string msg = _isGameWin ? "WIN!" : "GAME OVER! - ";
        float width = 0;

        if((!_isGameWin)){
            if((!this->_itemLog.empty()) && this->_itemLog.back() == ITEM_BOMB) {
                msg += "BOMB exploded! That was not item X(";
            } else {
                msg += "You're killed by the enemy X(";
            }
        }

        for(char i : msg){
            width += (float)glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, i);
        }

        glLoadIdentity();
        glTranslatef(-width / 800.0f, 0, 0);
        glColor3f(1, 0, 0);
        glRasterPos2f(0, 0);

        for(char i : msg) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);
        }
        return;
    }

    std::string strNumEnemy, strNumPlayer;
    std::string strLifeEnemy, strLifePlayer;

    std::string strMode;

    for(int i = 0; i < numEnemy; i++){ strNumEnemy += "O"; }
    for(int i = 0; i < NUM_ENEMY - numEnemy; i++){ strNumEnemy += "X"; }

    for(int i = 0; i < numPlayer; i++){ strNumPlayer += "O"; }
    for(int i = 0; i < NUM_PLAYER - numPlayer; i++){ strNumPlayer += "X"; }

    for(int i = 0; _enemy && (i < this->_enemy->numLife()); i++){ strLifeEnemy += "O"; }
    for(int i = 0; _enemy && (i < numLifeEnemy - 1 - this->_enemy->numLife()); i++){ strLifeEnemy += "X"; }

    for(int i = 0; _player && (i < this->_player->numLife()); i++){ strLifePlayer += "O"; }
    for(int i = 0; i < numLifePlayer - this->player()->numLife(); i++){strLifePlayer += "X";}


    if(mode == 'f') {
        strMode = "[ALL FAIL MODE]";
        glColor3f(1, 0, 0);
    } else if (mode == 'c') {
        strMode = "[ALL PASS MODE]";
        glColor3f(0, 1, 0);
    } else {
        strMode = "[NORMAL MODE]";
        glColor3f(0, 1, 1);
    }

    std::vector<std::string> strVec = {};

    strVec.push_back(strMode);

    strVec.emplace_back("      ENEMY: " + strNumEnemy);
    strVec.emplace_back("     PLAYER: " + strNumPlayer);
    strVec.emplace_back(" ENEMY LIFE: " + strLifeEnemy);
    strVec.emplace_back("PLAYER LIFE: " + strLifePlayer);

    strVec.emplace_back("");
    strVec.emplace_back("[ITEM LOG]");

    int counter = 0;
    for(auto & i : this->_itemLog) {
        string strItemType;

        if(i == ITEM_INCR) {
            strItemType = "Bullet INCR <MAX 5> :)";
        } else if (i == ITEM_DESC) {
            strItemType = "Bullet DESC <MIN 1> :(";
        } else if (i == ITEM_BOMB) {
            strItemType = "BOMB! X(";
        } else {
            strItemType = "UNKNOWN... Don't pwn-ing!";
        }

        strVec.emplace_back(to_string(counter++) + ": " + strItemType);
    }

    float strPosY = 0.95;

    for(auto & str : strVec){
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(-0.95, strPosY -= 0.05, -0.1);
        glScalef(1/3500.0, 1/3500.0, 1/3500.0);
        for(char character : str){
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, character);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void Game::display(bool isBlack) {
    if(!isPlayerDead && this->_player){
        _player->display(isBlack);
    }

    if(!isEnemyDead && this->_enemy){
        _enemy->display(isBlack);
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

void Game::newItemLog(char type) {
    this->_itemLog.push_back(type);
}
