#include <iostream>
#include <cstring>
using namespace std;

int main(){
    int cnt=0;;
    char str[100];
    cout<<"please input the string up to 99 letters :";
    cin.getline(str,100,'\n'); 
    char* head=str;
    char* tail=str+(strlen(str)-1);
    for(int i=0;i<strlen(str)/2;i++){
        if(*(head+i)==*(tail-i)){
            cnt++;
        }
    }
    if(cnt>=strlen(str)/2){
        cout<<str<<" is a kind of recursive word."<<endl;
    }else{
        cout<<str<<" is a normal word."<<endl;
    }

}