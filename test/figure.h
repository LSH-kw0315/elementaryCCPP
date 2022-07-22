#ifndef FIGURE_H
#define FIGURE_H
#include <iostream>
using namespace std;
    class Figure{
        public:
            virtual void draw();
            virtual void erase();
            virtual void center();
    };

#endif