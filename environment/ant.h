#ifndef ANT_H
#define ANT_H
#include "organism.h"
class Ant:public Organism{
 public:
 Ant():Organism(){return ;};
 void move(Organism***& world,int**& ui_world);
 void breeding(Organism***& world,int**& ui_world);
 ~Ant() {};
};
#endif