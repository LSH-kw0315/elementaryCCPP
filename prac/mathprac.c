#include <math.h>
#include <stdio.h>
double factorial(double src){
    double des=1.0;
    for(int cnt=(int)src; cnt>0; cnt--){
        des*=(double)cnt;
    }
    return des;
}
int main(){
    double an[10];
    double bn[10];
    double cn[10];
    for(int i=0;i<10;i++){
        cn[i]=1.0;
        if(i%2==0){
            an[i]=0.0;
            double tmp = (double)(2*i);
            bn[i]=1.0/(factorial(tmp));
        }else{
            bn[i]=0.0;
            double tmp=(double)(2*i-1);
            an[i]=1.0/(factorial(tmp));
        }
    }
    for(int i=0;i<10;i++){
        printf("%f \n",an[i]);
        printf("%f \n",bn[i]);
        for(int j=0;j<i;j++){
            //해야할 것은 anbn을 어떻게 한꺼번에 계산하게 하느냐임.
            cn[i]+=an[i-j]*bn[j];            
        }
        printf("%.3f \n\n",cn[i]);
    }
}