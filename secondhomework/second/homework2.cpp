#include <iostream>
#include <Windows.h>
#include "organism.h"
#include "ant.h"
#include "doodlebug.h"
int main(){
    
    int cnt = 0, x = 0, y = 0;
    char input;
    Organism*** world=new Organism**[20];
    int** ui_world=new int*[20];
    srand((unsigned int)(time(NULL)));
    for(int i=0;i<20;i++){
        world[i]=new Organism*[20];
        ui_world[i]=new int[20];
        for(int j=0;j<20;j++){
            world[i][j] = nullptr;
            ui_world[i][j]=' ';
        }
    }

    for(int i=0;i<100;i++){
        do{
            x=rand()%20;
            y=rand()%20;
        }while(ui_world[x][y]!=' ');
        world[x][y]=new Ant();
        ui_world[x][y]='O';
    }

    for(int i=0;i<5;i++){
        do{
            x=rand()%20;
            y=rand()%20;
        }while(ui_world[x][y]!=' ');
        world[x][y]=new Doodlebug();
        ui_world[x][y]='X';
    }

    while(true){
        system("cls");
        for(int i=0;i<20;i++){
            for(int j=0;j<20;j++){
                cout<<(char)ui_world[i][j];
            }
            cout<<"\n";
        }
        Sleep(1000);
        input=(char)getchar();
        if(input=='\n'){
            for(int i=0;i<20;i++){
                for (int j = 0; j < 20; j++) {
                    if (ui_world[i][j] == 'O' || ui_world[i][j] == 'X') {
                        if (world[i][j]->getTurn() == cnt) {
                            world[i][j]->move(world, ui_world);
                        }
                    }
                }
            }
            cnt++;
        }
        
    }

}