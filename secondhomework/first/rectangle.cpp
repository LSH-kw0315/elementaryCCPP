 #include "rectangle.h"
    Rectangle::Rectangle():Figure(){

    }
    Rectangle::Rectangle(int width,int height):Figure(width,height){
        
    }
    Rectangle::Rectangle(int width,int height,int centerWidth,int centerHeight):Figure(width,height,centerWidth,centerHeight){
        
    }
    Rectangle::Rectangle(int width,int height,int* center):Figure(width,height,center){
        
    }
    Rectangle::Rectangle(Rectangle& src):Figure(src){
        
    }

    void Rectangle:: draw(){
        int rwidth=getWidth();
        int rheight=getHeight();
        int widthCenter=*(getcenterPoint());
        for(int i=0;i<rheight;i++){
            for(int k=0;k<widthCenter;k++){
                cout<<" ";
            }
            for(int j=0;j<rwidth;j++){
                cout<<"*";
            }
            cout<<"\n";
        }
    }
    void Rectangle:: erase(){
        system("cls");
    }
