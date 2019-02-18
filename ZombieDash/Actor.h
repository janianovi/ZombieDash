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
    StudentWorld* world();
    void setDeath();
    
private:
    StudentWorld* sWorld;
    bool dead;
    
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* world, int startX, int startY);
    virtual ~Wall();
    virtual void doSomething();
    

};

class Penelope : public Actor
{
public:
    Penelope(StudentWorld* world, int startX, int startY);
    virtual ~Penelope();
    virtual void doSomething();
    
    bool infected();
    int countInfected();
    int getLandmines();
    int getFlames();
    int getVaccines();
    

private:
    bool isInfected;
    int nInfected;
    int landmines;
    int flamethrowers;
    int vaccines;
    
    
};




#endif // ACTOR_H_
