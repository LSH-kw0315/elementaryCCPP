#include <iostream>
#include <cstdlib>
#include <cctype>
using namespace std;
void error(char*);
void up_msg(char*);
void msg(char*);
void low_msg(char*);
void (*functable[])(char *)={error,msg,up_msg,low_msg};
void* fake[4]={error,msg,up_msg,low_msg};
int main(){
    int n=0;
    cin>>n;
    if(n<1||n>3){
        (*functable[0])("err");
        fake[0];
        cout<<endl<<functable[0];
        cout<<endl<<(*functable[0]);
    }else{
        (*functable[n])("Hi");
    }
}
void error(char * s){cerr<<s;}
void msg(char* s){cout<<s;}
void up_msg(char* s){cout<<s<<"up";}
void low_msg(char* s){cout<<s<<"low";}