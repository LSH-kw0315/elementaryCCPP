#include "doodlebug.h"

void Doodlebug::move(Organism***& world,int**& ui_world){
   srand((unsigned int)(time(NULL)));
   plusTurn();
   hunger+=1;
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
 
        int temp[2]={target_x,target_y};
        int* up = new int[2];
        int* down=new int[2];
        int* right=new int[2];
        int* left=new int[2];
        int n;
        up[0]=target_x-1; up[1]=target_y;
        down[0]=target_x+1; down[1]=target_y;
        right[0]=target_x; right[1]=target_y+1;
        left[0]=target_x; left[1]=target_y-1;
        //개미가 있으면
        if(up[0] >= 0&&ui_world[up[0]][up[1]]=='O' ){
            delete world[up[0]][up[1]];
            world[up[0]][up[1]]=this;
            ui_world[up[0]][up[1]]='X';
            hunger=0;
        }else if(down[0] <= 19 &&ui_world[down[0]][down[1]]=='O'){
            delete world[down[0]][down[1]];
            world[down[0]][down[1]]=this;
            ui_world[down[0]][down[1]]='X';
            hunger=0;
        }else if(left[1] >= 0 && ui_world[left[0]][left[1]]=='O'  ){
            delete world[left[0]][left[1]];
            world[left[0]][left[1]]=this;
            ui_world[left[0]][left[1]]='X';
            hunger=0;
        }else if(right[1] <= 19 && ui_world[right[0]][right[1]]=='O' ){
            delete world[right[0]][right[1]];
            world[right[0]][right[1]]=this;
            ui_world[right[0]][right[1]]='X';
            hunger=0;
        }
        //개미가 없으면
        else{
        if(target_x-1<0 || ui_world[up[0]][up[1]]!=' '){
            delete[] up;
            up=NULL;

        }else if(target_x+1>=20 || ui_world[down[0]][down[1]] != ' '){
            delete[] down;
            down=NULL;
        }
        if(target_y-1<0 || ui_world[left[0]][left[1]] != ' '){
            delete[] left;
            left=NULL;
        }else if(target_y+1>=20 || ui_world[right[0]][right[1]] != ' '){
            delete[] right;
            right=NULL;
        }
        int* position[4]={up,down,left,right};
        do{
            n=rand()%3;
        }while(position[n]!=NULL);
        ui_world[(position[n])[0]][(position[n])[1]]='X';
        world[(position[n])[0]][(position[n])[1]] = world[temp[0]][temp[1]];
        ui_world[temp[0]][temp[1]]=' ';
        world[temp[0]][temp[1]]=NULL;
        temp[0]=(position[n])[0];
        temp[1]=(position[n])[1];
    }
    if(hunger>=3){
        ui_world[temp[0]][temp[1]]=' ';
        delete world[temp[0]][temp[1]];
        world[temp[0]][temp[1]]=NULL;
    }else if(getTurn()>=8){
        breeding(world,ui_world);
    }
}

void Doodlebug::breeding(Organism***& world,int**& ui_world){
    
   srand((unsigned int)(time(NULL)));
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

        int temp[2]={target_x,target_y};
        int* up = new int[2];
        int* down=new int[2];
        int* right=new int[2];
        int* left=new int[2];
        int n;
        up[0]=target_x-1; up[1]=target_y;
        down[0]=target_x+1; down[1]=target_y;
        right[0]=target_x; right[1]=target_y+1;
        left[0]=target_x; left[1]=target_y-1;
        if(target_x-1<0 || world[up[0]][up[1]] != NULL){
            delete[] up;
            up=NULL;

        }else if(target_x+1>=20 || world[down[0]][down[1]] != NULL){
            delete[] down;
            down=NULL;
        }
        if(target_y-1<0 || world[left[0]][left[1]] != NULL){
            delete[] left;
            left=NULL;
        }else if(target_y+1>=20 || world[right[0]][right[1]] != NULL){
            delete[] right;
            right=NULL;
        }
        int* position[4]={up,down,left,right};
        do{
            n=rand()%3;
        }while(position[n]!=NULL);

        world[(position[n])[0]][(position[n])[1]] = new Doodlebug(); 
}