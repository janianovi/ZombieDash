#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <list>
#include <vector>
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
    string levelOne = "level01.txt";
    Level lev(assetPath());
    lev.loadLevel(levelOne);
    for (int i = 0; i < LEVEL_HEIGHT; i++)
    {
        for (int j = 0; j < LEVEL_WIDTH; j++)
        {
            Level::MazeEntry ge = lev.getContentsOf(i, j);
            switch(ge)
            {
                case Level::player:
                    player = new Penelope(this, SPRITE_WIDTH * j, SPRITE_HEIGHT * i);
                    break;
                case Level::wall:
                    actorSet.push_back(new Wall(this, SPRITE_WIDTH * j, SPRITE_HEIGHT * i));
                    break;
                default:
                    break;
                
            }
        }
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    player->doSomething();
    list<Actor*>::iterator it;
    for (it = actorSet.begin(); it != actorSet.end(); it++)
    {
        if (!(*it)->death())
            (*it)->doSomething();
    }
    
    //decLives();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it = actorSet.begin();
    
    while (it != actorSet.end())
    {
        delete (*it);
        it = actorSet.erase(it);
    }
    
    if (player != nullptr)
        delete player;
}

bool StudentWorld::checkPlace(int startx, int starty)
{
    list<Actor*>::iterator it = actorSet.begin();
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









