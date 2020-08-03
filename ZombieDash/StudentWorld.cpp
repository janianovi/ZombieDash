#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>
#include <sstream>
#include <iomanip>
#include <cassert>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    finished = false;
    lifeTick = true;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    //fill in level name
    ostringstream s;
    s.fill('0');
    s << "level" << setw(2) << getLevel() << ".txt";
    string levelText = s.str();
    
    //load levels
    string levels = levelText;
    Level lev(assetPath());
    Level::LoadResult newLevel = lev.loadLevel(levels);
    
    if (newLevel == Level::load_fail_bad_format)
    {
        cerr << "File " << levels << " was not formatted correctly" << endl;
    }
    else if (newLevel == Level::load_fail_file_not_found)
    {
        cerr << "File " << levels << " was not found" << endl;
    }
    else if (newLevel == Level::load_success)
    {
        cerr << "Loaded File " << levels << endl;
        
        // create all actors
        for(int i = 0; i < LEVEL_WIDTH; i++)
        {
            for(int j = 0; j < LEVEL_HEIGHT; j++)
                createActor(lev.getContentsOf(i, j), i, j);
        }
    }
    else
    {
        cerr << "Something went wrong" << endl;
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    
    for (int i= 0; i < actorSet.size(); i++)
    {
        if (actorSet[i]->isAlive())
        {
            
            //let all other actors move
            actorSet[i]->doSomething();
            
            //if Penelope is not alive
            if (!actorSet[0]->isAlive())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            
            
            // check if level is complete
            if (finished)
            {
                finished = false;
                playSound(SOUND_LEVEL_FINISHED);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    
    // remove actors
    for (vector<Actor*>::iterator it = actorSet.begin(); it != actorSet.end();)
    {
        if ((*it)->isAlive())
            it++;
        else
        {
            delete *it;
            it = actorSet.erase(it);
        }
    }

    //create stats bar
    displayBar();
    
    //check life tick
    if(getLifeTick())
        lifeTick= false;
    else
        lifeTick = true;


    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //delete all actors
    vector<Actor*>::iterator it = actorSet.begin();
    
    while (it != actorSet.end())
    {
        delete (*it);
        it = actorSet.erase(it);
    }
    
}

bool StudentWorld::barrier(double x, double y, Actor* act)
{
    //checks if objects do not overlap with one another
    for (int i = 0; i < actorSet.size(); i++)
    {
        if (act == actorSet[i])
            continue;
        if (checkCoordinates(x, y, actorSet[i]->getX(), actorSet[i]->getY()) && (!actorSet[i]->canCollide()))
            return true;
    }
    return false;
}

double StudentWorld::disSquare(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

bool StudentWorld::overlap(int x1, int y1, int x2, int y2)
{
    //calculates if object can be overlapped
    double distance = disSquare(x1, y1, x2, y2);
    return distance <= 10;
}


bool StudentWorld::checkCoordinates(double x1, double y1, double x2, double y2)
{
    //checks if sprites overlap
    if (x1 <= x2 + SPRITE_WIDTH - 1 && x1 + SPRITE_WIDTH - 1 >= x2 &&
        y1 <= y2 + SPRITE_HEIGHT - 1 && y1 + SPRITE_HEIGHT - 1 >= y2)
        return true;

    return false;
}

void StudentWorld::createActor(Level::MazeEntry ge, double x, double y)
{
    //creating actors in levels
    Actor* object = nullptr;
    switch (ge)
    {
        case Level::empty:
            break;
        case Level::player:
            actorSet.insert(actorSet.begin(), new Penelope(x, y, this));
            break;
        case Level::smart_zombie:
            object = new SmartZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            actorSet.push_back(object);
            break;
        case Level::dumb_zombie:
            object = new DumbZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            actorSet.push_back(object);
            break;
        case Level::citizen:
            object = new Citizen(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            actorSet.push_back(object);
            break;
        case Level::wall:
            object = new Wall(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            actorSet.push_back(object);
            break;
        case Level::exit:
            object = new Exit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            actorSet.push_back(object);
            break;
        case Level::pit:
            object = new Pit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            actorSet.push_back(object);
            break;
        case Level::vaccine_goodie:
            object = new Vaccine(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            actorSet.push_back(object);
            break;
        case Level::gas_can_goodie:
            object = new GasCan(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            actorSet.push_back(object);
            break;
        case Level::landmine_goodie:
            object = new LandmineGoodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y, this);
            actorSet.push_back(object);
            break;
        default:
            cerr << "Invalid" << endl;
    }
}


bool StudentWorld::vomitInfected(double x, double y)
{
    //checks if vomit can be made
    for (int i = 0; i < actorSet.size(); i++)
    {
        //only vomit on citizen or Penelope (people that can be infected)
        if (actorSet[i]->canInfected() && overlap(x, y, actorSet[i]->getX(), actorSet[i]->getY()))
            return true;
    }
    return false;
}

void StudentWorld::overlapVomit(double x, double y)
{
    //infects person if vomitted
    for (int i = 0; i < actorSet.size(); i++)
    {
        if((actorSet[i]->canInfected()) && (overlap(x, y, actorSet[i]->getX(), actorSet[i]->getY())))
            static_cast<Person*>(actorSet[i])->ifInfected();
    }
}

void StudentWorld::createZombie(double x, double y)
{
    //creating dumb and smart zombies
    if(rand() < 7)
    {
        actorSet.push_back(new DumbZombie(x, y, this));
    }
    else
        actorSet.push_back(new SmartZombie(x, y, this));
}

void StudentWorld::createVomit(double x, double y, Direction dir)
{
    //creating vomit
    actorSet.push_back(new Vomit(x, y, dir, this));
    playSound(SOUND_CITIZEN_INFECTED);
}



double StudentWorld::distanceToZombie(Zombie*& zomb, Citizen* food)
{
    //calculating distance to zombies
    double distance = 80;
    double temp;
    
    for(int i = 0; i < actorSet.size(); i++)
    {
        temp = disSquare(food->getX(), food->getY(), actorSet[i]->getX(), actorSet[i]->getY());
        if(actorSet[i]->canInfected() && (temp <= distance))
        {
            //set element to a zombie
            zomb = static_cast<Zombie*>(actorSet[i]);
            distance = temp;
        }
    }
    return distance;
}


double StudentWorld::distanceToPerson(Person*& people, Actor* act)
{
    //calculate distance to person
    double distance = 80;
    double temp;
    
    for(int i = 0; i < actorSet.size(); i++)
    {
        temp = disSquare(act->getX(), act->getY(), actorSet[i]->getX(), actorSet[i]->getY());
        if(actorSet[i]->canInfected() && (temp <= distance))
        {
            //set element to a person
            people = static_cast<Person*>(actorSet[i]);
            distance = temp;
        }
    }
    return distance;
}

void StudentWorld::exitPerson(Exit* out)
{
    //allow citizens to exit
    for (int i = 1; i < actorSet.size(); i++)
    {
        if (actorSet[i]->canInfected() && overlap(out->getX(), out->getY(), actorSet[i]->getX(), actorSet[i]->getY()))
        {
            playSound(SOUND_CITIZEN_SAVED);
            increaseScore(500);
            actorSet[i]->setDeath();
        }
    }
}

bool StudentWorld::citizensRemain()
{
    //check if citizens remain in level
    for (int i = 1; i < actorSet.size(); i++)
    {
        if(actorSet[i]->canInfected())
            return true;
    }
    return false;
}

Penelope* StudentWorld::getPenelope()
{
    //return Penelope
    return static_cast<Penelope*>(actorSet[0]);
}

bool StudentWorld::getLifeTick()
{
    return lifeTick;
}

void StudentWorld::completeLevel()
{
    //returns if level is finished
    finished = true;
}

void StudentWorld::displayBar()
{
    //updates the stat bar
    ostringstream bar;
    bar << "Score: ";
    bar.fill('0');
    bar << setw(6) << getScore() << "  ";                       // score
    bar << "Level: " << getLevel() << "  ";                     // level
    bar << "Lives: " << getLives() << "  ";                     // lives
    bar << "Vaccines: " << getPenelope()->getVaccines() << "  ";   // vaccines
    bar << "Flames: " << getPenelope()->getFlames() << "  ";       // flames
    bar << "Mines: " << getPenelope()->getLandmines() << "  ";     // landmines
    bar << "Infected: " << getPenelope()->countInfected();        // infection count
    
    setGameStatText(bar.str());
}






