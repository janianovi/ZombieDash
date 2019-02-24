#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld* world);
    
    virtual ~Actor();
    virtual void doSomething() = 0;
    
    bool death();
    bool isAlive();
    void setDeath();
    
    bool infected();
    bool becomeInfected(bool infect);
    int infectNumber(int count);
    int countInfected();
    void incInfected();
    StudentWorld* world();
    
    
private:
    StudentWorld* sWorld;
    bool dead;
    bool alive;
    bool isInfected;
    int nInfected;
    
};

class Person: public Actor
{
public:
    Person(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld* world);
    virtual void doSomething();
    void ifInfected();
    
private:
    bool perInfected;
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* world, int startX, int startY);
    virtual ~Wall();
    virtual void doSomething();
    

};

class Penelope : public Person
{
public:
    Penelope(StudentWorld* world, int startX, int startY);
    virtual ~Penelope();
    virtual void doSomething();

    int getLandmines();
    int getFlames();
    int getVaccines();
    int addLandmines(int number);
    int addFlames(int number);
    int addVaccines(int number);

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
    
class Citizen: public Person
{
public:
    Citizen(int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};


class Zombie: public Person
{
public:
    Zombie(int startX, int startY, StudentWorld* world);
    virtual void doSomething() = 0;
    int moving();
private:
    int movement;
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
    //bool isOnSafety();
private:
    //int m_safetyTicks;
};



class Goodies: public Actor
{
public:
    Goodies(int imageID, int startX, int startY, StudentWorld* world);
    virtual void doSomething();
};

class Vaccine : public Goodies
{
public:
    Vaccine(int startX, int startY, StudentWorld* world);
};

class GasCan : public Goodies
{
public:
    GasCan(int startX, int startY, StudentWorld* world);
};

class LandmineGoodie: public Goodies
{
public:
    LandmineGoodie(int startX, int startY, StudentWorld* world);
};







#endif // ACTOR_H_
