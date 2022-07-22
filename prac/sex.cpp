#include <iostream>
#include <typeinfo>
using namespace std;


int main()
{
 char** ui_world=new char*[20];
 for(int i=0;i<20;i++){
        ui_world[i]=new char[20];
        for(int j=0;j<20;j++){
            ui_world[i][j]='a';
            cout<<ui_world[i][j];
        }
    }
 return 0;
}