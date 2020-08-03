#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Level.h"
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
    bool checkCoordinates(double x1, double y1, double x2, double y2);
    bool barrier(double x, double y, Actor* act);
    bool overlap(int x1, int y1, int x2, int y2);
    void createActor(Level::MazeEntry ge, double x, double y);
    void createZombie(double x, double y);
    void createVomit(double x, double y, Direction dir);
    bool vomitInfected(double x, double y);
    void overlapVomit(double x, double y);
    double disSquare(double x1, double y1, double x2, double y2);
    double distanceToZombie(Zombie*& zomb, Citizen* food);
    double distanceToPerson(Person*& people, Actor* act);
    bool getLifeTick();
    void exitPerson(Exit* out);
    bool citizensRemain();
    Penelope* getPenelope();
    void completeLevel();
    void displayBar();
    

private:
    vector<Actor*> actorSet;
    bool finished;
    bool lifeTick;
};

#endif // STUDENTWORLD_H_
