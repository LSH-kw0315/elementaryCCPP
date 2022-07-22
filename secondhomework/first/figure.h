#ifndef FIGURE_H
#define FIGURE_H
#include <iostream>
using namespace std;
    class Figure{
        public:
            Figure();
            Figure(int width,int height);
            Figure(int width,int height,int widthCenter,int heightCenter);
            Figure(int widht,int height,int* center);
            Figure(Figure& src);
            virtual void draw()=0;
            virtual void erase()=0;
            virtual void center();
            int getWidth();
            int getHeight();
            int* getcenterPoint();
            void setWidth(int width);
            void setHeight(int height);
            void setcenterPoint(int width,int height);
            void setcenterPoint(int* center);
            virtual ~Figure();
        private:
            int width;
            int height;
            int centerPoint[2];
    };


#endif