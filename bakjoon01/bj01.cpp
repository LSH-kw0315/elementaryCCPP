#include <iostream>
int main(void){
    int input;
    int cnt0[41];
    int cnt1[41];
    cnt0[0]=1;
    cnt0[1]=0;
    cnt1[0]=0;
    cnt1[1]=1;
    for(int i=2;i<41;i++){
       cnt0[i]=cnt0[i-2]+cnt0[i-1];
       cnt1[i]=cnt1[i-2]+cnt1[i-1];
    }
    std::cin>>input;
    std::cin.ignore(1000,'\n');
    std::cout<<cnt0[input]<<" "<<cnt1[input]<<"\n";
    return 0;
}