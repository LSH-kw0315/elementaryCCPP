#ifndef DOODLEBUG_H
#define DOODLEBUG_H
#include "organism.h"
class Doodlebug:public Organism{
 public:
 Doodlebug():Organism(){hunger=0;};
 void move(Organism***& world,int**& ui_world);
 void breeding(Organism***& world,int**& ui_world);
 ~Doodlebug() {};
 private:
 int hunger;
};
#endif