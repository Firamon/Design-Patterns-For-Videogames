#include<memory>
#include<iostream>

using namespace std;

/*
I have upgradable buildings, each building has an effect on GameState. Buildings with multiple upgrades will apply multiple effects. Buildings:

HOUSE, that can be upgraded with more obstacles in the garden, or speeding up the thief that wants to rob the house 
*/

class GameState{
    private:
        int obstacles;
        float money;
        float speed;
    public:
        GameState(int obstacles, float money, float speed) : obstacles(obstacles), money(money), speed(speed) {}

        int getObstacles(){
            return this->obstacles;
        }
        float getMoney(){
            return this->money;
        }
        float getSpeed(){
            return this->speed;
        }

        void speedUp(int sp){
            this->speed += sp;
        }
        void increaseObstacle(int obstInc){
            this->obstacles += obstInc;
        }
        void decreseMoney(int money){
            this->money -= money;
        }

        void printGameState(){
            cout << "Obstacles: " << this->obstacles << ", Moneys: " << this->money << ", Speed: " << this->speed << endl;
        }

};

class Building{
    public:
        virtual void effect(std::shared_ptr<GameState> gs) = 0;
};

class House : public Building{
    void effect(std::shared_ptr<GameState> gs) override{
        gs->decreseMoney(20);
        gs->printGameState();
    }
};

class BuildingDecorator : public Building{
    protected:
        std::shared_ptr<Building> building;
    
    public:
        BuildingDecorator(std::shared_ptr<Building> b){
            this->building = b;
        }

        void effect(std::shared_ptr<GameState> gs) override{
            building->effect(gs);
        }
};

class SpeedUpgrade : public BuildingDecorator{
    public:
        SpeedUpgrade(std::shared_ptr<Building> b) : BuildingDecorator(b) {}

        void effect(std::shared_ptr<GameState> gs){
            gs->speedUp(10);
            gs->decreseMoney(5);
            BuildingDecorator::effect(gs);
        }
};

class ObstacleUpgrade : public BuildingDecorator{
    public:
        ObstacleUpgrade(std::shared_ptr<Building> b) : BuildingDecorator(b) {}

        void effect(std::shared_ptr<GameState> gs){
            gs->increaseObstacle(1);
            gs->decreseMoney(10);
            BuildingDecorator::effect(gs);
        }
};


int main(){
    std::shared_ptr<GameState> gs(new GameState(3, 100, 20));

    std::shared_ptr<Building> house1(new House());
    house1 = std::make_shared<SpeedUpgrade>(house1);
    house1 = std::make_shared<ObstacleUpgrade>(house1);
    house1 = std::make_shared<SpeedUpgrade>(house1);
    house1->effect(gs);



    return 0;
}