#include<iostream>
#include<memory>
#include <random>
#include <iostream>

using namespace std;

/*
---------------------------------------------------DESIGN PATTERN "FACTORY OBJECT"-------------------------------------------------


You have 5 types of enemies:
* robot
* manticore
* griffon
* ogre
* goblin

Each enemy has HP, attack damage and attack range. Each enemy activates his passive after being created. You want them to be able to damage the player, and vice versa. You also want enemies not be able to damage each other.
*/

enum EnemyType {ROBOT, MANTICORE, GRIFFON, OGRE, GOBLIN};

class Creature{
    protected:
        float hp;
        float atk_dmg;
        float atk_range;
    public:
    //pure virtual functions
        virtual void getDamaged(int damage) = 0;
        virtual void dealDamage(std::shared_ptr<Creature> target) = 0;
};

class Player : public Creature{
    public:
        Player(int hp){
            this->hp = hp;
            this->atk_dmg = 30;
            this->atk_range = 0;
        }

        int getHP(){
            return this->hp;
        }

        void getDamaged(int dmg) override{
            this->hp = this->hp - dmg;
        }

        void dealDamage(std::shared_ptr<Creature> target) override{
            target->getDamaged(atk_dmg);
        }
};

class Enemy : public Creature{
    protected:
        EnemyType type;
    public:
        //pure virtual functions
        virtual void passive() = 0;


        int random_number(int min, int max){
            return (std::rand() % (max - min)) + min;
        }

        void getDamaged(int dmg) override{
            this->hp = this->hp - dmg;
        }

        void dealDamage(std::shared_ptr<Creature> p) override{
            if(dynamic_pointer_cast<Player>(p)){
                p->getDamaged(atk_dmg);
            }
        }

        float getHP(){
            return this->hp;
        }

        float getAtkDmg(){
            return this->atk_dmg;
        }

        float getAtkRange(){
            return this->atk_range;
        }

        EnemyType getType(){
            return this->type;
        }
};

class Robot : public Enemy{
    public:
        Robot(){
            this->hp = random_number(100, 200);
            this->atk_dmg = random_number(10,20);
            this->atk_range = 0;
            this->type = EnemyType::ROBOT;
        }

        void passive() override{
            atk_dmg = atk_dmg + 10;
        }
};

class Manticore : public Enemy{
    public:
        Manticore(){
            this->hp = random_number(200, 300);
            this->atk_dmg = random_number(20,25);
            this->atk_range = 10;
            this->type = EnemyType::MANTICORE;
        }

        void passive() override{
            atk_range = atk_range + 10;
        }
};

class Griffon : public Enemy{
    public:
        Griffon(){
            this->hp = random_number(50, 200);
            this->atk_dmg = random_number(40,60);
            this->atk_range = random_number(10,30);
            this->type = EnemyType::GRIFFON;
        }

        void passive() override{
            hp = hp + 20;
        }
};

class Ogre : public Enemy{
    public:
        Ogre(){
            this->hp = random_number(400, 700);
            this->atk_dmg = 20;
            this->atk_range = 0;
            this->type = EnemyType::OGRE;
        }

        void passive() override{
            hp = hp + 50;
        }
};

class Goblin : public Enemy{
    public:
        Goblin(){
            this->hp = random_number(10, 30);
            this->atk_dmg = random_number(5, 10);
            this->atk_range = 0;
            this->type = EnemyType::GOBLIN;
        }

        void passive() override{
            hp = hp + 10;
            atk_dmg = atk_dmg + 10;
        }
};

class EnemyFactory{
    public:
        std::shared_ptr<Enemy> createEnemy(EnemyType type){
            std::shared_ptr<Enemy> enemy(nullptr);
            
            switch (type){
                case EnemyType::GOBLIN:
                {
                    std::shared_ptr<Goblin> temp(new Goblin());
                    enemy = dynamic_pointer_cast<Enemy>(temp);
                    break;
                }
                case EnemyType::GRIFFON:
                {
                    std::shared_ptr<Griffon> temp(new Griffon());
                    enemy = dynamic_pointer_cast<Enemy>(temp);
                    break;
                }
                case EnemyType::MANTICORE:
                {
                    std::shared_ptr<Manticore> temp(new Manticore());
                    enemy = dynamic_pointer_cast<Enemy>(temp);
                    break;
                }
                case EnemyType::OGRE:
                {
                    std::shared_ptr<Ogre> temp(new Ogre());
                    enemy = dynamic_pointer_cast<Enemy>(temp);
                    break;
                }
                case EnemyType::ROBOT:
                {
                    std::shared_ptr<Robot> temp(new Robot());
                    enemy = dynamic_pointer_cast<Enemy>(temp);
                    break;
                }
            }

            return enemy;
        }
};

class EnemyCreator{
    private:
        std::shared_ptr<EnemyFactory> factory;
    public:
        EnemyCreator(std::shared_ptr<EnemyFactory> factory){
            this->factory = factory;
        }

        std::shared_ptr<Enemy> getEnemy(EnemyType type){
            std::shared_ptr<Enemy> enemy;
            enemy = factory->createEnemy(type);
            enemy->passive();
            return enemy;
        }
};

void printStats(std::shared_ptr<Enemy> enemy){
    cout << "Enemy: " << enemy->getType() << ". HP: " << enemy->getHP() << ", attack damage: " << enemy->getAtkDmg() << ", attack range: " << enemy->getAtkRange() << endl;
    return ;
}



int main(){
    //create enemy factory
    std::shared_ptr<EnemyFactory> factory(new EnemyFactory());
    std::unique_ptr<EnemyCreator> creator(new EnemyCreator(factory));
    //create player
    std::shared_ptr<Player> player(new Player(100));
    //create 2 goblins and ogre
    std::shared_ptr<Enemy> goblin1 = creator->getEnemy(EnemyType::GOBLIN);
    printStats(goblin1);
    std::shared_ptr<Enemy> goblin2 = creator->getEnemy(EnemyType::GOBLIN);
    printStats(goblin2);
    std::shared_ptr<Enemy> ogre1 = creator->getEnemy(EnemyType::OGRE);
    printStats(ogre1);
    //deal damage to player
    goblin1->dealDamage(player);
    goblin2->dealDamage(player);
    ogre1->dealDamage(player);
    //show player's health
    cout << "Player HP: " << player->getHP() << endl;
    //player attacks goblin1
    player->dealDamage(goblin1);
    printStats(goblin1);
    //no damage dealt
    goblin2->dealDamage(goblin1);
    printStats(goblin1);

    return 0;
}