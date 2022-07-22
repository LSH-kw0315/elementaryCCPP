#include "triangle.h"
    Triangle::Triangle():Figure(){

    }
    Triangle::Triangle(int width,int height):Figure(width,height){
        
    }
    Triangle::Triangle(int width,int height,int centerWidth,int centerHeight):Figure(width,height,centerWidth,centerHeight){
        
    }
    Triangle::Triangle(int width,int height,int* center):Figure(width,height,center){
        
    }
    Triangle::Triangle(Triangle& src):Figure(src){
        
    }
    void Triangle:: draw(){
        //직각삼각형을 그린다고 가정한다.
        int twidth=getWidth();
        int theight=getHeight();
        int widthCenter=*(getcenterPoint());
        for(int i=0;i<theight;i++){
            for(int k=0;k<widthCenter;k++){
                cout<<" ";
            }
            //높이가 더 큰 경우에 대한 분기
            if(theight>=twidth){//높이가 더 크거나 같을 때
                if(theight-i>twidth){//차례대로 나타낼 수 없는 범위에 대해서는 *하나로만 표기한다
                    cout<<"*\n";
                    continue;
                }
            }else if(theight<twidth){//폭이 더 클 때
                if(i>=theight-1){ //만약 높이의 끝에 도달했다면
                    for(int j=0;j<twidth;j++)
                        cout<<"*";
                    break;
                }
            }
            for(int j=0;j<i+1;j++){
                cout<<"*";
            }
            cout<<"\n";
        }
    }
    void Triangle:: erase(){
        system("cls");
    }
