#include<iostream>
#include<string>

using namespace std;

/*
PlayerController controls Player. Player can be a skeleton or a mower. Skeleton and mower shares the same movement logic, but they have different abilities.
Skeleton and Mower moves on different kind of platforms. Each platform defines a type of movement, and accepts only some inputs.

There are two types of platforms:

* PlatformRotate: can move only left or right, rotates 90 degrees in that direction
* PlatformForward: can only go back and forward, moves by speed
*/
class IMoveBehavior;

class PlayerModel{
    public:
        int speed;
        float rotation;
        IMoveBehavior *moveBehavior = nullptr;

        PlayerModel();

        void move();
        virtual void ability() = 0;
};

class IMoveBehavior{
    public:
        PlayerModel *player = nullptr;

        virtual void move() = 0;
};

class PlatformForwardBehavior : public IMoveBehavior{
    public:

        PlatformForwardBehavior(PlayerModel *pl){
            this->player = pl;
        }

        void move() override{
            string key;
            while(key != "exit"){
                cout << "Listening for forward platform inputs" << endl;
                cin >> key;
                if(key == "w"){
                    cout << "Moving " << player->speed << " meters forward";
                }else if(key == "s"){
                    cout << "Moving " << player->speed << " meters backward";
                }
            }
        }
};

class PlatformRotateBehavior : public IMoveBehavior{
    public:

        PlatformRotateBehavior(PlayerModel *pl){
            this->player = pl;
        }

        void move() override{
            string key;
            while(key != "exit"){
                cout << "Listening for rotate platform inputs" << endl;
                cin >> key;
                if(key == "a"){
                    cout << "Rotating " << player->rotation << " degrees left";
                }else if(key == "d"){
                    cout << "Rotating " << player->rotation << " degrees right";
                }
            }
        }
};


class Skeleton : public PlayerModel{
    public:
        Skeleton() : PlayerModel(){
            speed = 1;
            rotation = 0;
        }

        void ability() override{
            cout << "Skeleton ability" << endl;
        }
};

class Mower : public PlayerModel{
    public:
        Mower() : PlayerModel(){
            speed = 3;
            rotation = 0;
        }

        void ability() override{
            cout << "Mower ability" << endl;
        }
};

class PlayerController{
    public:

        int energy = 0;

        PlayerModel *player = nullptr;

        PlayerController(int energy){
            this->energy = energy;
            if(this->energy > 50){
                player = new Mower();
            }else{
                player = new Skeleton();
            }
        }

        void forwardPlatformMovement(){
            player->moveBehavior = new PlatformForwardBehavior(player);
            player->move();
        }

        void rotatePlatformMovement(){
            player->moveBehavior = new PlatformRotateBehavior(player);
            player->move();
        }


};


// PlayerModel definition
PlayerModel::PlayerModel(){
    //default moveBehavior
    moveBehavior = new PlatformForwardBehavior(this);
}

void PlayerModel::move(){
    moveBehavior->move();
}

int main(){
    PlayerController *pc = new PlayerController(100);
    
    pc->forwardPlatformMovement();
    pc->rotatePlatformMovement();
    pc->forwardPlatformMovement();
    return 0;
}