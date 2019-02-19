#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include <sstream>

using namespace std;

class Penelope;
class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    bool checkPlace(int startx, int starty);
    bool boundary(Actor* act, int x, int y);

private:
    list<Actor*> actorSet;
    Penelope* player;
    
};

#endif // STUDENTWORLD_H_
