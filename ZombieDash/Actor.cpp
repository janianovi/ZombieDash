#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld* world)
        :GraphObject(imageID, startX, startY, dir, depth)
{
    dead = false;
    alive = true;
    sWorld = world;
    isInfected = false;
    nInfected = 0;
}
Actor::~Actor()
{}

bool Actor::death() 
{
    return dead; 
}

bool Actor::isAlive()
{
    return alive;
}

StudentWorld* Actor::world()
{
    return sWorld;
}

void Actor::setDeath()
{
    dead = true;
}

bool Actor::becomeInfected(bool infect)
{
    return isInfected = infect;
}

int Actor::infectNumber(int count)
{
    return nInfected = count;
}

bool Actor::infected()
{
    return isInfected;
}

int Actor::countInfected()
{
    return nInfected;
}

void Actor::incInfected()
{
    nInfected++;
}

// ********* Person ******** //

Person::Person(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld* world):
    Actor(imageID, startX, startY, dir, 0, world)
{
    perInfected = false;
}

void Person::doSomething()
{
    
}

void Person::ifInfected()
{
    perInfected = true;
}

// ******** Wall **********//

Wall::Wall(StudentWorld* world, int startX, int startY):Actor(IID_WALL, startX, startY, right, 0, world)
{}

Wall::~Wall()
{}

void Wall::doSomething()
{
    
}

// ********* Penelope ********* //

Penelope::Penelope(StudentWorld* world, int startX, int startY): Person(IID_PLAYER, startX, startY, right, 0, world)
{
    landmines = 0;
    flamethrowers = 0;
    vaccines = 0;
    
}

Penelope::~Penelope()
{
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

int Penelope::addLandmines(int number)
{
    return landmines += number;
}

int Penelope::addFlames(int number)
{
    return flamethrowers += number;
}

int Penelope::addVaccines(int number)
{
    return vaccines += number;
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
            incInfected();
    }
    
    int ch;
    if (world()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(world()->checkPlace(getX() - 4, getY())) //left
                    moveTo(getX() - 4.0, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(world()->checkPlace(getX() + 4, getY())) //right
                    moveTo(getX() + 4.0, getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(world()->checkPlace(getX(), getY() + 4)) //up
                    moveTo(getX(), getY()+4.0);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(world()->checkPlace(getX(), getY() - 4))    //down
                    moveTo(getX(), getY()-4.0);
                break;
            case KEY_PRESS_SPACE:
                if (flamethrowers > 0)
                {
                    flamethrowers--;
                    world()->playSound(SOUND_PLAYER_FIRE);
                    
                    for(int i = 1; i < 4; i++)
                    {
                        double x;
                        double y;
                        
                        if (getDirection() == up)
                        {
                            x = getX();
                            y = getY() + (i * SPRITE_HEIGHT);
                        }
                        else if (getDirection() == down)
                        {
                            x = getX();
                            y = getY() - (i * SPRITE_HEIGHT);
                        }
                        else if (getDirection() == right)
                        {
                            x = getX() + (i * SPRITE_WIDTH);
                            y = getY();
                        }
                        else if (getDirection() == left)
                        {
                            x = getX() - (i * SPRITE_WIDTH);
                            y = getY();
                        }
                        
                        // ******** check overlap here
                    }
                }
                break;
            case KEY_PRESS_TAB:
                if (landmines > 0)
                {
                    landmines--;
                    
                    //world()->createActor
                }
                break;
            case KEY_PRESS_ENTER:
                if (vaccines > 0)
                {
                    vaccines--;
                    
                    
                    becomeInfected(false);
                    infectNumber(0);
                }
                break;
                
        }
    }
    return;
}

//******** Exit ******//

Exit::Exit(int startX, int startY, StudentWorld* world):
    Actor(IID_EXIT, startX, startY, right, 1, world)
{
    
}

void Exit::doSomething()
{
    
}

//********* Citizen *********//

Citizen::Citizen(int startX, int startY, StudentWorld* world):
    Person(IID_CITIZEN, startX, startY, right, 0, world)
{
    
}

void Citizen::doSomething()
{
    if(death())
        return;
    
    if(infected())
    {
        if(countInfected() >= 500)
        {
            setDeath();
            world()->playSound(SOUND_ZOMBIE_BORN);
            //create methods to decrease citizens + scoreing
        }
        
        if(randInt(1, 10) <= 3)
        {
            //createActor Zombie
            
        }
        else
        {
            //createActor
        }
    }
    
    
}

//******** Vomit ******//

Vomit::Vomit(int startX, int startY, Direction dir, StudentWorld* world) :
    Actor(IID_VOMIT, startX, startY, right, 0, world)
{
    
}

void Vomit::doSomething()
{
    
}

// ******* Flame ********//

Flame::Flame(int startX, int startY, StudentWorld* world):
    Actor(IID_FLAME, startX, startY, right, 0, world)
{}

void Flame::doSomething()
{
    
}

// ******* Pit ********//
Pit::Pit(int startX, int startY, StudentWorld* world) : Actor(IID_PIT, startX, startY, right, 0, world)
{}

void Pit::doSomething()
{
    
}

// ******* Goodies ********//
Goodies::Goodies(int imageID, int startX, int startY, StudentWorld* world) : Actor(imageID, startX, startY, right, 1, world)
{}

void Goodies::doSomething()
{
    
}

// ******* Vaccine ********//
Vaccine::Vaccine(int startX, int startY, StudentWorld* world) : Goodies(IID_VACCINE_GOODIE, startX, startY, world)
{}

// ******* Flamethrower ********//
GasCan::GasCan(int startX, int startY, StudentWorld* world) : Goodies(IID_GAS_CAN_GOODIE, startX, startY, world)
{}

// ******* Landmine ********//
LandmineGoodie::LandmineGoodie(int startX, int startY, StudentWorld* world) : Goodies(IID_LANDMINE_GOODIE, startX, startY, world)
{}






























