#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld* world)
        :GraphObject(imageID, startX, startY, dir, depth)
{
    dead = false;
    sWorld = world;
}
Actor::~Actor()
{}

bool Actor::death() 
{
    return dead;
}

StudentWorld* Actor::world()
{
    return sWorld;
}

void Actor::setDeath()
{
    dead = true;
}

Wall::Wall(StudentWorld* world, int startX, int startY):Actor(IID_WALL, startX, startY, right, 0, world)
{}

Wall::~Wall()
{}

void Wall::doSomething()
{
    
}

Penelope::Penelope(StudentWorld* world, int startX, int startY):Actor(IID_PLAYER, startX, startY, right, 0, world)
{
    isInfected = false;
    nInfected = 0;
    landmines = 0;
    flamethrowers = 0;
    vaccines = 0;
    
}

Penelope::~Penelope()
{
}

bool Penelope::infected()
{
    return isInfected;
}

int Penelope::countInfected()
{
    return nInfected;
}

int Penelope::getLandmines()
{
    return landmines;
}

int Penelope::getFlames()
{
    return flamethrowers;
}

int Penelope::getVaccines()
{
    return vaccines;
}

void Penelope::doSomething()
{
    if (death())
        return;
    
    if (infected())
    {
        if(countInfected() >= 500)
        {
            death();
            world()->playSound(SOUND_PLAYER_DIE);
            return;
        }
        else
            nInfected++;
    }
    
    int ch;
    if (world()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(world()->checkPlace(getX() - 4.0, getY()))    //check if penelope can move to new spot, left
                    moveTo(getX() - 4.0, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(world()->checkPlace(getX() + 4.0, getY())) // check if penelope can move to new spot, right
                    moveTo(getX() + 4.0, getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(world()->checkPlace(getX(), getY()+4.0)) //check if penelope can move to new spot
                    moveTo(getX(), getY()+4.0);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(world()->checkPlace(getX(), getY()-4.0))    //check if penelope can move to new spot
                    moveTo(getX(), getY()-4.0);
                break;
        }
    }
    return;
}







