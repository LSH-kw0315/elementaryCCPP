#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "figure.h"
    class Triangle :public Figure{
        public:
            Triangle();
            Triangle(int width,int height);
            Triangle(int width,int height,int widthCenter,int heightCenter);
            Triangle(int widht,int height,int* center);
            Triangle(Triangle& src);
            void draw();
            void erase();

        
    };


#endif