#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = actorSet.begin();
    
    while (it != actorSet.end())
    {
        delete (*it);
        it = actorSet.erase(it);
    }
    delete player;
}

bool StudentWorld::checkPlace(int startx, int starty)
{
    vector<Actor*>::iterator it = actorSet.begin();
    while(it != actorSet.end())
    {
        if(boundary(*it, startx, starty))
            return false;
        it++;
    }
    return true;
}

bool StudentWorld::boundary(Actor *act, int x, int y)
{
    if ((act->getX() <= x + SPRITE_WIDTH - 1) && (act->getX() + SPRITE_WIDTH - 1 >= x) && (act->getY() <= y + SPRITE_HEIGHT - 1)
        && (act->getY() + SPRITE_WIDTH - 1 >= y))
    {
        return true;
        
    }
    return false;
}








