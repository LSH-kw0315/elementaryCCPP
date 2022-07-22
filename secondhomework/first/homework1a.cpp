#include "figure.h"
#include "rectangle.h"
#include "triangle.h"
int main(){
    Triangle tri1(3,3);
    Triangle tri2(tri1);
    Triangle tri3(6,8,0,0);
    tri3.center();
    cout<<"\n\n";
    tri2.draw();
    Rectangle rect1(10,9,5,5);
    cout<<"\n\n";
    rect1.draw();
}