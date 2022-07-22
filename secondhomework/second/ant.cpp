#include "ant.h"
#include <iostream>
void Ant::move(Organism***& world,int**& ui_world){
    
   plusTurn();
   //이동 부분
   int target_x=-1;
   int target_y=-1;
   for(int i=0;i<20;i++){
       for(int j=0;j<20;j++){
           if(world[i][j]==this){
               target_x=i;
               target_y=j;
               break;
           }
       }
   }
   if (target_x < 0 || target_y < 0) {
       return;
   }
        int temp[2]={target_x,target_y};
        int n;
        int cnt = 0;
        while(true) {
            n = rand() % 4;
            if ((n == 0 && temp[0] - 1 >= 0 && ui_world[temp[0] - 1][temp[1]] == ' ') || (n == 1 && temp[0] + 1 <= 19 && ui_world[temp[0] + 1][temp[1]] == ' ') || (n == 2 && temp[1] - 1 >= 0 && ui_world[temp[0]][temp[1] - 1] == ' ') || (n == 3 && temp[1] + 1 <= 19 && ui_world[temp[0]][temp[1] + 1] == ' ')) {
                break;
            }
            cnt++;
            if (cnt >= 20) {
                n = 5;
                break;
            }
            
        }
        switch (n) {
        case 0:
            if (temp[0] - 1 >= 0 && ui_world[temp[0]-1][temp[1]]==' ') {
                ui_world[temp[0] - 1][temp[1]] = 'O';

                world[temp[0]-1][temp[1]] = this;
                ui_world[temp[0]][temp[1]] = ' ';

                world[temp[0]][temp[1]] = nullptr;
                break;
            }
        case 1:
            if (temp[0] + 1 <= 19 && ui_world[temp[0] + 1][temp[1]] == ' ') {
                ui_world[temp[0] + 1][temp[1]] = 'O';

                world[temp[0] + 1][temp[1]] = this;
                ui_world[temp[0]][temp[1]] = ' ';
  
                world[temp[0]][temp[1]] = nullptr;
                break;
            }
        case 2:
            if (temp[1] - 1 >= 0 && ui_world[temp[0]][temp[1]-1] == ' ') {
                ui_world[temp[0]][temp[1]-1] = 'O';

                world[temp[0]][temp[1]-1] = this;
                ui_world[temp[0]][temp[1]] = ' ';
          
                world[temp[0]][temp[1]] = nullptr;
                break;
            }
        case 3:
            if (temp[1] + 1 <= 19 && ui_world[temp[0]][temp[1]+1] == ' ') {
                ui_world[temp[0]][temp[1]+1] = 'O';
        
                world[temp[0]][temp[1]+1] = this;
                ui_world[temp[0]][temp[1]] = ' ';
  
                world[temp[0]][temp[1]] = nullptr;
                break;
            }
        default:
            break;
        }     
   //번식 판단
   if(getTurn()%3==0 && getTurn()>0){
       breeding(world,ui_world);
    }
}   
void Ant::breeding(Organism***& world, int**& ui_world) {

    int target_x = -1;
    int target_y = -1;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (world[i][j] == this) {
                target_x = i;
                target_y = j;
                break;
            }
        }
    }
    if (target_x < 0 || target_y < 0) {
        return;
    }
        int temp[2]={target_x,target_y};
        int n;
        int cnt = 0;
        while(true) {
            n = rand() % 4;
            if ((n == 0 && temp[0] - 1 >= 0 && ui_world[temp[0] - 1][temp[1]] == ' ') || (n == 1 && temp[0] + 1 <= 19 && ui_world[temp[0] + 1][temp[1]] == ' ') || (n == 2 && temp[1] - 1 >= 0 && ui_world[temp[0]][temp[1] - 1] == ' ') || (n == 3 && temp[1] + 1 <= 19 && ui_world[temp[0]][temp[1] + 1] == ' ')) {
                break;
            }
            cnt++;
            if (cnt >= 20) {
                n = 5;
                break;
            }
        } 
        switch (n) {
        case 0:
         
                ui_world[temp[0]-1][temp[1]] = 'O';

                world[temp[0]-1][temp[1]] = new Ant();
                break;
            
        case 1:
           
                ui_world[temp[0] + 1][temp[1]] = 'O'; 

                world[temp[0] + 1][temp[1]] = new Ant();
                break;
            
        case 2:
         
                ui_world[temp[0]][temp[1]-1] = 'O';
 
                world[temp[0]][temp[1]-1] = new Ant();
                break;
            
        case 3:
           
                ui_world[temp[0]][temp[1]+1] = 'O';
          
                world[temp[0]][temp[1]+1] = new Ant();
                break;
            
        default:
            break;
        }  
   
}