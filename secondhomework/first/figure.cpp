#include "figure.h"

Figure::Figure(){
    width=5;
    height=4;
    centerPoint[0]=3;
    centerPoint[1]=2;
}
Figure::Figure(int width,int height){
    this->width=width;
    this->height=height;
    centerPoint[0]=3;
    centerPoint[1]=2;
}
Figure::Figure(int width,int height,int widthCenter,int heightCenter){
    this->width=width;
    this->height=height;
    centerPoint[0]=widthCenter;
    centerPoint[1]=heightCenter;
}
Figure::Figure(int width,int height,int* center){
    this->width=width;
    this->height=height;
    centerPoint[0]=(*center);
    centerPoint[1]=*(center+1);
}
Figure::Figure(Figure& src){
    width=src.width;
    height=src.height;
    centerPoint[0]=src.centerPoint[0];
    centerPoint[1]=src.centerPoint[1];
}
int Figure:: getWidth(){
    return width;
}
int Figure:: getHeight(){
    return height;
}
int* Figure:: getcenterPoint(){
    return centerPoint;
}
void Figure:: setWidth(int width){
    this->width= width;
}
void Figure:: setHeight(int height){
    this->height=height;
}
void Figure:: setcenterPoint(int width,int height){
    this->centerPoint[0]=width;
    this->centerPoint[1]=height;
}
void Figure:: setcenterPoint(int* center){
    this->centerPoint[0]=center[0];
    this->centerPoint[1]=center[1];
}
void Figure::center(){
    erase();
    for(int i=0;i<centerPoint[1];i++){
        cout<<"\n";
    }
    draw();
}
Figure::~Figure(){
    delete [] centerPoint;
    delete this;
}