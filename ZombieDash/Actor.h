#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
using namespace std;

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld* world);
    virtual ~Actor();
    virtual void doSomething() = 0;
    bool death();
    bool isAlive();
    void setDeath();
    bool canInfected();
    bool becomeInfected(bool infect);
    int infectNumber(int count);
    StudentWorld* world();
    bool canCollide();
    void setCollide(bool col);
    bool moveObject(Direction dir, double move);
private:
    StudentWorld* sWorld;
    bool alive;
    bool isInfected;
    bool collision;
};

class Person: public Actor
{
public:
    Person(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld* world);
    virtual void doSomething();
    void ifInfected();
    bool infectStatus();
    int countInfected();
    void incInfected();
    void vaccinated();
    Direction follow(Actor* act);
private:
    bool perInfected;
    int numInfected;
};

class Wall : public Actor
{
public:
    Wall(int startX, int startY, StudentWorld* world);
    virtual ~Wall();
    virtual void doSomething();
};

class Penelope : public Person
{
public:
    Penelope(int startX, int startY, StudentWorld* world);
    virtual ~Penelope();
    virtual void doSomething();
    int getLandmines();
    int getFlames();
    int getVaccines();
    int addLandmines();
    int addFlames();
    int addVaccines();
private:
    int landmines;
    int flamethrowers;
    int vaccines;
    
};

class Exit: public Actor
{
public:
    Exit(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};

class Zombie: public Actor
{
public:
    Zombie(int startX, int startY, StudentWorld* world);
    virtual void doSomething() = 0;
    void movementPlan();
    void vomitSpot(double& vx, double& vy, double x, double y);
    bool goVomit();
    void setDirection(Direction dir);
private:
    Direction direct;
    int directDistance;
};
    
class Citizen: public Person
{
public:
    Citizen(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    bool runFromZombie(Direction& dir, Zombie* zomb, double distance);
};

    
class DumbZombie: public Zombie
{
public:
    DumbZombie(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};

class SmartZombie: public Zombie
{
public:
    SmartZombie(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};

class Vomit : public Actor
{
public:
    Vomit(int startX, int startY, Direction dir, StudentWorld* world);
    virtual void doSomething();
private:
    bool created;
};

class Pit: public Actor
{
public:
    Pit(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};

class Flame: public Actor
{
public:
    Flame(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};


class Landmine : public Actor
{
public:
    Landmine(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};



class Goodies: public Actor
{
public:
    Goodies(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething();
    bool pickUp();
};

class Vaccine : public Goodies
{
public:
    Vaccine(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};

class GasCan : public Goodies
{
public:
    GasCan(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};

class LandmineGoodie: public Goodies
{
public:
    LandmineGoodie(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};




#endif // ACTOR_H_
