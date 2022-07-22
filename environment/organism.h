#ifndef ORGANISM_H
#define ORGANISM_H
#include <cstdlib>
#include <ctime>
using namespace std;
class Organism{
 public:
 Organism() { turn = 0; };
 virtual void move(Organism***& world, int**& ui_world)=0;
 virtual void breeding(Organism***& world, int**& ui_world)=0;
 int getTurn(){return turn;}
 void plusTurn(){turn+=1;}
 virtual ~Organism(){};
 private:
 int turn;
};
#endif