#include<iostream>
#include<memory>
#include<vector>

using namespace std;

/*
Create a series of buildings, which can activate their effects simultaneisly, and change the game state.
*/

enum building_types {HOUSE, CASTLE, MINE};

//game state, contains the supply of money obtained until now
class GameState{
    public:
        float money;
        GameState(){
            money = 0;
        }
        void increaseMoney(int money){
            this->money = money;
        }
};


//all kinds of possible buildings, each one has an effect on the GameState. Building is the abstract Observer class, which contains the method "activateEffect", that will subscribe, and fired as an event
class Building{
    protected:
        building_types type;
        float hp;
    public:
        Building(float hp) : hp(hp) {}
        virtual void activateEffect(std::shared_ptr<GameState> gs) = 0;
};

class House : public Building{
    public:
        House(float hp) : Building(hp) {}
        void activateEffect(std::shared_ptr<GameState> gs){
            gs->money += 10;
        }
};

class Castle : public Building{
    public:
        Castle(float hp) : Building(hp) {}
        void activateEffect(std::shared_ptr<GameState> gs){
            gs->money += 30;
        }
};

class Mine : public Building{
    public:
        Mine(float hp) : Building(hp) {}
        void activateEffect(std::shared_ptr<GameState> gs){
            gs->money += 60;
        }
};

//this is the observable class, from here you will add, remove, and notify observers
class Subject{
    private:
        std::vector<std::shared_ptr<Building>> observers;

    public:
        Subject(){
            observers = {};
        }


        void registerObserver(std::shared_ptr<Building> b){
            observers.push_back(b);
        }

        void unregisterObserver(std::shared_ptr<Building> b){
            for(int i=0; i<observers.size(); i++){
                if(b.get() == observers.at(i).get()){
                    observers.erase(observers.begin() + i);
                    break;
                }
            }
        }

        void notify(std::shared_ptr<GameState> gs){
            for(int i=0; i<observers.size(); i++){
                observers.at(i)->activateEffect(gs);
            }
        }
};


void printGameState(std::shared_ptr<GameState> gs){
    cout << "Money Supply: " << gs->money << endl;
}


int main(){
    std::shared_ptr<GameState> gs(new GameState());
    std::shared_ptr<Subject> observable(new Subject());


    std::shared_ptr<Building> house1(new House(40));
    observable->registerObserver(house1);
    std::shared_ptr<Building> castle1(new Castle(110));
    observable->registerObserver(castle1);
    std::shared_ptr<Building> mine1(new Mine(30));
    observable->registerObserver(mine1);
    observable->notify(gs);
    printGameState(gs);
    observable->unregisterObserver(house1);
    observable->notify(gs);
    printGameState(gs);
    observable->unregisterObserver(castle1);
    observable->notify(gs);
    printGameState(gs);
    observable->unregisterObserver(mine1);
    observable->notify(gs);
    printGameState(gs);

    return 0;
}