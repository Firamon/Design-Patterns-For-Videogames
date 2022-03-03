#include<iostream>
#include<vector>
#include<string>
#include <conio.h>

using namespace std;

/*
This pattern makes use of Composite, in order to manage states without a moltitude of nested if-else loops, which lesser the maintenability

You must manage 3 lanes system. The player can switch from a lane to another: 1 to 2, 2 to 3, 3 to 2, 2 to 1.
While switching lane won't be accepted new movement commands.

If the player rotates left while in lane1, he will rotate 90 degrees left
If the player rotates right while in lane3, he will rotate 90 degrees right
*/

enum class LanePosition {LEFT, MIDDLE, RIGHT};

class ILaneState{
    protected:
        LanePosition currentLanePosition;
    public:
        virtual void moveLeft() = 0;
        virtual void moveRight() = 0;
};

//forward defined this class, the declaration is after the definition of the children of ILaneState
class Player{
    private:
        string name;
    public:
        ILaneState *state;
        ILaneState *leftState;
        ILaneState *middleState;
        ILaneState *rightState;

        Player();
        void moveLeft();
        void moveRight();

};



class LeftLaneState : public ILaneState{
    private:
        Player *player = nullptr;
    public:
        LeftLaneState(Player *player){
            this->player = player;
            currentLanePosition = LanePosition::LEFT;
        }

        void moveLeft() override {
            cout << "You can't go left from left lane" << endl;
        }

        void moveRight() override {
            cout << "Going middle from left lane" << endl;
            player->state = player->middleState;
            
        }

};

class MiddleLaneState : public ILaneState{
    private:
        Player *player = nullptr;
    public:
        MiddleLaneState(Player *player){
            this->player = player;
            currentLanePosition = LanePosition::MIDDLE;
        }

        void moveLeft() override {
            cout << "Going left from middle lane" << endl;
            player->state = player->leftState;
        }

        void moveRight() override {
            cout << "Going right from middle lane" << endl;
            player->state = player->rightState;
        }

};

class RightLaneState : public ILaneState{
    private:
        Player *player = nullptr;
    public:
        RightLaneState(Player *player){
            this->player = player;
            currentLanePosition = LanePosition::RIGHT;
        }

        void moveLeft() override {
            cout << "Going middle from right lane" << endl;
            player->state = player->leftState;
        }

        void moveRight() override {
            cout << "You can't go right from right lane" << endl;
            player->state = player->rightState;
        }

};

Player::Player(){
    name = "player";
    leftState = new LeftLaneState(this);
    middleState = new MiddleLaneState(this);
    rightState = new RightLaneState(this);

    state = middleState;
}

void Player::moveLeft(){
    state->moveLeft();
}

void Player::moveRight(){
    state->moveRight();
}

void listenKeyboardWindows(Player* player){
    string key = "";
    while(key != "exit"){
        cin >> key;

        if(key == "a"){
            player->moveLeft();
        }else if(key == "d"){
            player->moveRight();
        }
    }
    return ;
}

int main(){
    Player *player = new Player();
    listenKeyboardWindows(player);
    


    return 0;
}
