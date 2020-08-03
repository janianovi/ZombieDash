#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld* world)
        :GraphObject(imageID, startX, startY, dir, depth)
{
    alive = true;
    sWorld = world;
    isInfected = false;
    collision = false;
}
Actor::~Actor()
{}


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
    alive = false;
}

bool Actor::becomeInfected(bool infect)
{
    return isInfected = infect;
}


bool Actor::canInfected()
{
    return isInfected;
}


bool Actor::canCollide()
{
    return collision;
}
void Actor::setCollide(bool col)
{
    collision = col;
}


bool Actor::moveObject(Direction dir, double move)
{
    //moving function
    setDirection(dir);
    
    int x = getX();
    int y = getY();
    switch(dir)
    {
        case right:
            x += move;
            break;
        case up:
            y += move;
            break;
        case left:
            x -= move;
            break;
        case down:
            y -= move;
            break;
    }
    
    if (!world()->barrier(x, y, this))
    {
        moveTo(x, y);
        return true;
    }
    else
        return false;
}


// ********* Person ******** //

Person::Person(int imageID, int startX, int startY, Direction dir, int depth, StudentWorld* world):
    Actor(imageID, startX, startY, dir, 0, world)
{
    perInfected = false;
    numInfected = 0;
    becomeInfected(true);
}

void Person::doSomething()
{
}

bool Person::infectStatus()
{
    return perInfected;
}

void Person::ifInfected()
{
    perInfected = true;
}

int Person::countInfected()
{
    return numInfected;
}

void Person::incInfected()
{
    numInfected++;
}

void Person::vaccinated()
{
    perInfected = false;
    numInfected = 0;
    
}

Direction Person::follow(Actor* act)
{
    //calculating direction
    Direction first = right;
    Direction second = down;
    
    if(getX() < act->getX())
        first = right;
    else if (getX() > act->getX())
        first = left;
    
    //same row
    if (getY() == act->getY())
        return first;
    
    if(getY() < act->getY())
        second = up;
    else if (getY() > act->getY())
        second = down;
    
    //same column
    if(getX() == act->getX())
        return second;
    
    int r = rand();
    
    if (r < 5)
        return first;
    else
        return second;
}

// ******** Wall **********//

Wall::Wall(int startX, int startY, StudentWorld* world):Actor(IID_WALL, startX, startY, right, 0, world)
{}

Wall::~Wall()
{}

void Wall::doSomething()
{
}

// ********* Penelope ********* //

Penelope::Penelope(int startX, int startY, StudentWorld* world): Person(IID_PLAYER, startX * SPRITE_WIDTH, startY * SPRITE_HEIGHT, right, 0, world)
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

int Penelope::addLandmines()
{
    return landmines += 2;
}

int Penelope::addFlames()
{
    return flamethrowers += 5;
}

int Penelope::addVaccines()
{
    return vaccines += 1;
}



void Penelope::doSomething()
{
    //check infected
    if(infectStatus())
    {
        incInfected();
        if (countInfected() >= 500)
        {
            setDeath();
        }
    }
    
    if(!isAlive())
    {
        world()->playSound(SOUND_PLAYER_DIE);
    }
    
    if(!isAlive())
        return;
    
    //moving plan
    int ch;
    if (world()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                moveObject(left, 4);
                break;
            case KEY_PRESS_RIGHT:
                moveObject(right, 4);
                break;
            case KEY_PRESS_UP:
                moveObject(up, 4);
                break;
            case KEY_PRESS_DOWN:
                moveObject(down, 4);
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
                        
                        //would add new flame here
                    }
                }
                break;
            case KEY_PRESS_TAB:
                if (landmines > 0)
                {
                    landmines--;
                    
                    //would add new landmine here
                }
                break;
            case KEY_PRESS_ENTER:
                if (vaccines > 0)
                {
                    vaccines--;
                    //decrement vaccines
                    
                    vaccinated(); //change from infected to not
                }
                break;
                
        }
    }
}

//******** Exit ******//

Exit::Exit(int startX, int startY, StudentWorld* world):
    Actor(IID_EXIT, startX, startY, right, 1, world)
{
    setCollide(true);
}

void Exit::doSomething()
{
    //allow citizens to exit
    world()->exitPerson(this);
    
    //complete level if all citizens escaped and Penelope is at exit
    if(world()->overlap(getX(), getY(), world()->getPenelope()->getX(), world()->getPenelope()->getY()))
    {
        if (!world()->citizensRemain())
            world()->completeLevel();
    }
}

//********* Citizen *********//

Citizen::Citizen(int startX, int startY, StudentWorld* world):
    Person(IID_CITIZEN, startX, startY, right, 0, world)
{
}

void Citizen::doSomething()
{
    //if infected
    if(infectStatus())
    {
        incInfected();
        if (countInfected() >= 500)
        {
            setDeath();
        }
    }
    //if dead, create a new zombie
    if(!isAlive())
    {
        world()->playSound(SOUND_ZOMBIE_BORN);
        world()->increaseScore(-1000);
        world()->createZombie(getX(), getY());
    }
    
    if(!isAlive())
        return;
    
    if(world()->getLifeTick())
        return;
    //play sound if infected
    if(countInfected() == 1)
        world()->playSound(SOUND_CITIZEN_INFECTED);
    
    Direction dir;
    Zombie* zomb;
    
    //calculate distance from person to citizen
    double distancePerson = world()->disSquare(getX(), getY(),
                                               world()->getPenelope()->getX(), world()->getPenelope()->getY());
    double distanceZombie = world()->distanceToZombie(zomb, this);
    
    //run from zombie or run to person
    if((distancePerson > distanceZombie) && (distancePerson <= 80))
    {
        moveObject(follow(world()->getPenelope()), 2);
    }
    else if ((runFromZombie(dir, zomb, distanceZombie)) && (distanceZombie <= 80))
    {
        moveObject(dir, 2);
    }
    else
        return;
    
}


bool Citizen::runFromZombie(Direction& dir, Zombie *zomb, double distance)
{
    double place = distance;
    double temp = 0;
    
    //checks boundaries and sets direction for citizens
    if((!world()->barrier(getX() + 2, getY(), this)))
    {
        if ((temp = world()->disSquare(getX() + 2, getY(), zomb->getX(), zomb->getY() > place))) //right
        {
            dir = right;
            place = temp;
        }
    }
    
    if((!world()->barrier(getX() - 2, getY(), this)))
    {
        if (((temp = world()->disSquare(getX() - 2, getY(), zomb->getX(), zomb->getY()) > place))) //left
        {
            dir = left;
            place = temp;
        }
    }
    
    if((!world()->barrier(getX(), getY() + 2, this)))
    {
        if ((temp = world()->disSquare(getX(), getY() + 2, zomb->getX(), zomb->getY() > place))) //up
        {
            dir = up;
            place = temp;
        }
    }
    
    if((!world()->barrier(getX(), getY() - 2, this)))
    {
        if (((temp = world()->disSquare(getX(), getY() - 2, zomb->getX(), zomb->getY()) > place))) //down
        {
            dir = down;
            place = temp;
        }
    }
    
    if (place != distance)
        return true;
    else
        return false;

}


//***** Zombie *****//

Zombie::Zombie(int startX, int startY, StudentWorld* world) : Actor(IID_ZOMBIE, startX, startY, right, 0, world)
{
    directDistance = 0;
}

void Zombie::setDirection(Direction dir)
{
    direct = dir;
}

void Zombie::doSomething()
{
}


void Zombie::movementPlan()
{
    //zombie movement
    
    int ch = randInt(1, 4);
    
    switch (ch)
    {
        case 1:  // move left
            setDirection(left);
            moveObject(left, 2);
            break;
        case 2: // move right
            setDirection(right);
            moveObject(right, 2);
            break;
        case 3:  // move up
            setDirection(up);
            moveObject(up, 2);
            break;
        case 4:  // move down
            setDirection(down);
            moveObject(down, 2);
            break;
    }
}

void Zombie::vomitSpot(double& vx, double& vy, double x, double y)
{
    //creating vomit location
    vx = x;
    vy = y;
    switch (getDirection())
    {
        case right:
            vx += SPRITE_WIDTH;
            break;
        case up:
            vy += SPRITE_HEIGHT;
            break;
        case left:
            vx -= SPRITE_WIDTH;
            break;
        case down:
            vy -= SPRITE_HEIGHT;
            break;
        default:
            break;
    }
}

bool Zombie::goVomit()
{
    //check if it can vomit
    double x = getX();
    double y = getY();
    vomitSpot(x, y, getX(), getY());
    
    if(world()->vomitInfected(x, y))
    {
        //create vomit
        world()->createVomit(x, y, direct);
        return true;
    }
    return false;
}


//***** DumbZombie *****//


DumbZombie::DumbZombie(int startX, int startY, StudentWorld* world) : Zombie(startX, startY, world)
{
}

void DumbZombie::doSomething()
{
    //if tick don't move
    if (world()->getLifeTick())
        return;
    
    //check vomit
    if(goVomit())
        return;
    else
        movementPlan();
}
    
//***** SmartZombie *****//


SmartZombie::SmartZombie(int startX, int startY, StudentWorld* world) : Zombie(startX, startY, world)
{
}

void SmartZombie::doSomething() {
    //if tick, don't move
    if (world()->getLifeTick())
        return;
    
    // check vomit
    if (goVomit())
        return;
    else
        movementPlan();

}


//******** Vomit ******//

Vomit::Vomit(int startX, int startY, Direction dir, StudentWorld* world) :
    Actor(IID_VOMIT, startX, startY, right, 0, world)
{
    created = true;
    setCollide(true);
}

void Vomit::doSomething()
{
    if(!created)
    {
        setDeath();
        return;
    }
    
    
    created = false;
    
    //infect person
    world()->overlapVomit(getX(), getY());
    
}

// ******* Flame ********//

Flame::Flame(int startX, int startY, StudentWorld* world):
    Actor(IID_FLAME, startX, startY, right, 0, world)
{
    setCollide(true);
}

void Flame::doSomething()
{
    if(!isAlive())
        return;
    
   
}

// ******* Landmine ********//

Landmine::Landmine(int startX, int startY, StudentWorld* world): Actor(IID_LANDMINE, startX, startY, right, 1, world)
{
    setCollide(true);
}
void Landmine::doSomething()
{
    if (!isAlive())
        return;
}

// ******* Pit ********//
Pit::Pit(int startX, int startY, StudentWorld* world) : Actor(IID_PIT, startX, startY, right, 0, world)
{
}

void Pit::doSomething()
{
    
}

// ******* Goodies ********//
Goodies::Goodies(int imageID, int startX, int startY, StudentWorld* world) : Actor(imageID, startX, startY, right, 1, world)
{
    setCollide(true);
}

void Goodies::doSomething()
{
    if (isAlive())
        return;
    
}

bool Goodies::pickUp()
{
    //checking overlap with goodies, plays sound and increase score if picked up
    if (world()->overlap(getX(), getY(), world()->getPenelope()->getX(), world()->getPenelope()->getY()))
    {
        world()->increaseScore(50);
        setDeath();
        world()->playSound(SOUND_GOT_GOODIE);
        return true;
    }
    return false;
}

// ******* Vaccine ********//
Vaccine::Vaccine(int startX, int startY, StudentWorld* world) : Goodies(IID_VACCINE_GOODIE, startX, startY, world)
{
}

void Vaccine::doSomething()
{
    if(pickUp())
        world()->getPenelope()->addVaccines();
}

// ******* Flamethrower ********//
GasCan::GasCan(int startX, int startY, StudentWorld* world) : Goodies(IID_GAS_CAN_GOODIE, startX, startY, world)
{
}

void GasCan::doSomething()
{
    if(pickUp())
        world()->getPenelope()->addFlames();
}

// ******* Landmine Goodie ********//
LandmineGoodie::LandmineGoodie(int startX, int startY, StudentWorld* world) : Goodies(IID_LANDMINE_GOODIE, startX, startY, world)
{

}

void LandmineGoodie::doSomething()
{
    if(pickUp())
        world()->getPenelope()->addLandmines();
}





