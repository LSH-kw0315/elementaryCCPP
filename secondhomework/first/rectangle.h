#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "figure.h"

    class Rectangle:public Figure{
        public:
            Rectangle();
            Rectangle(int width,int height);
            Rectangle(int width,int height,int widthCenter,int heightCenter);
            Rectangle(int widht,int height,int* center);
            Rectangle(Rectangle& src);
            void draw();
            void erase();

        
    };


#endif