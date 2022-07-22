#include "doodlebug.h"
#include <iostream>
void Doodlebug::move(Organism***& world, int**& ui_world) {
 plusTurn();
 hunger += 1;
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
 int temp[2] = { target_x,target_y };
 int n;
 //개미가 있으면
 if ((temp[0] - 1 >= 0 && ui_world[temp[0] - 1][temp[1]] == 'O') || (temp[0] + 1 <= 19 &&
ui_world[temp[0] + 1][temp[1]] == 'O') || (temp[1] - 1 >= 0 && ui_world[temp[0]][temp[1] - 1]
== 'O') || (temp[1] + 1 <= 19 && ui_world[temp[0]][temp[1] + 1] == 'O')) {
 while (true) {
 n = rand() % 4;
 if ((n == 0 && temp[0] - 1 >= 0 && ui_world[temp[0] - 1][temp[1]] == 'O') || (n ==
1 && temp[0] + 1 <= 19 && ui_world[temp[0] + 1][temp[1]] == 'O') || (n == 2 && temp[1] - 1 >= 0
&& ui_world[temp[0]][temp[1] - 1] == 'O') || (n == 3 && temp[1] + 1 <= 19 &&
ui_world[temp[0]][temp[1] + 1] == 'O')) {
 break;
 }
 }
 switch (n)
 {
 case 0:
 delete world[temp[0] - 1][temp[1]];
 world[temp[0] - 1][temp[1]] = new Doodlebug();
 world[temp[0] - 1][temp[1]] = this;
 ui_world[temp[0] - 1][temp[1]] = 'X';
 ui_world[temp[0]][temp[1]] = ' ';
 world[temp[0]][temp[1]] = nullptr;
 hunger = 0;
 break;
 case 1:
 delete world[temp[0] + 1][temp[1]];
 world[temp[0] + 1][temp[1]] = new Doodlebug();
 world[temp[0] + 1][temp[1]] = this;
 ui_world[temp[0] + 1][temp[1]] = 'X';
 ui_world[temp[0]][temp[1]] = ' ';
 world[temp[0]][temp[1]] = nullptr;
 hunger = 0;
 break;
 case 2:
 delete world[temp[0]][temp[1] - 1];
 world[temp[0]][temp[1] - 1] = new Doodlebug();
 world[temp[0]][temp[1] - 1] = this;
 ui_world[temp[0]][temp[1] - 1] = 'X';
 ui_world[temp[0]][temp[1]] = ' ';
 world[temp[0]][temp[1]] = nullptr;
 hunger = 0;
 break;
 case 3:
 delete world[temp[0]][temp[1] + 1];
 world[temp[0]][temp[1] + 1] = new Doodlebug();
 world[temp[0]][temp[1] + 1] = this;
 ui_world[temp[0]][temp[1] + 1] = 'X';
 ui_world[temp[0]][temp[1]] = ' ';
 world[temp[0]][temp[1]] = nullptr;
 hunger = 0;
 break;
 default:
 break;
 }
 }
 //개미가 없으면
 else{
 int cnt = 0;
 while(true){
 n = rand() % 4;
 if ((n == 0 && temp[0] - 1 >= 0 && ui_world[temp[0] - 1][temp[1]] == ' ') || (n == 1 &&
temp[0] + 1 <= 19 && ui_world[temp[0] + 1][temp[1]] == ' ') || (n == 2 && temp[1] - 1 >= 0 &&
ui_world[temp[0]][temp[1] - 1] == ' ') || (n == 3 && temp[1] + 1 <= 19 &&
ui_world[temp[0]][temp[1] + 1] == ' ')) {
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

 ui_world[temp[0] - 1][temp[1]] = 'X';
 world[temp[0]-1][temp[1]] = this;
 ui_world[temp[0]][temp[1]] = ' ';

 world[temp[0]][temp[1]] = nullptr;
 temp[0] = temp[0]-1;
 break;

 case 1:

 ui_world[temp[0] + 1][temp[1]] = 'X';

 world[temp[0] + 1][temp[1]] = this;
 ui_world[temp[0]][temp[1]] = ' ';

 world[temp[0]][temp[1]] = nullptr;
 temp[0] = temp[0] + 1;
 break;

 case 2:
 ui_world[temp[0]][temp[1] - 1] = 'X';

 world[temp[0]][temp[1]-1] = this;

 ui_world[temp[0]][temp[1]] = ' ';

 world[temp[0]][temp[1]] = nullptr;
 temp[1] = temp[1] - 1;
 break;

 case 3:

 ui_world[temp[0]][temp[1] + 1] = 'X';
 world[temp[0]][temp[1]+1] = this;
 ui_world[temp[0]][temp[1]] = ' ';
 world[temp[0]][temp[1]] = nullptr;
 temp[1] = temp[1] + 1;
 break;

 default:
 break;
 }

 }
 if(hunger>=3){
 ui_world[temp[0]][temp[1]]=' ';
 world[temp[0]][temp[1]] = nullptr;
 delete world[temp[0]][temp[1]];
 }else if(getTurn()>=8 && getTurn()>0){
 breeding(world,ui_world);
 }
}
void Doodlebug::breeding(Organism***& world,int**& ui_world){
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
 if ((n == 0 && temp[0] - 1 >= 0 && ui_world[temp[0] - 1][temp[1]] == ' ') || (n ==
1 && temp[0] + 1 <= 19 && ui_world[temp[0] + 1][temp[1]] == ' ') || (n == 2 && temp[1] - 1 >= 0
&& ui_world[temp[0]][temp[1] - 1] == ' ') || (n == 3 && temp[1] + 1 <= 19 &&
ui_world[temp[0]][temp[1] + 1] == ' ')) {
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

 ui_world[temp[0] - 1][temp[1]] = 'X';

 world[temp[0] - 1][temp[1]] = new Doodlebug();
 break;

 case 1:

 ui_world[temp[0] + 1][temp[1]] = 'X';

 world[temp[0] + 1][temp[1]] = new Doodlebug();
 break;

 case 2:

 ui_world[temp[0]][temp[1] - 1] = 'X';

 world[temp[0]][temp[1] - 1] = new Doodlebug();
 break;

 case 3:

 ui_world[temp[0]][temp[1] + 1] = 'X';

 world[temp[0]][temp[1] + 1] = new Doodlebug();
 break;

 default:
 break;
 }
}