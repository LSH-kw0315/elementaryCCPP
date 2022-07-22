#include <iostream>
#include <vector>
using namespace std;
int main(){
    int score;
    int max=0;
    vector<int> scoreList;
    int* histogram;
    cout<<"please input scores. if you input less than or equal to -1, It will stop."<<endl;
    while(true){
        cin>>score;
        if(score<=-1){
            break;
        }
        scoreList.push_back(score);

        if(score>max){
            max=score;
        }
    }
    int index=max+1;
    histogram=new int[index];
    for(int i=0;i<index;i++){
        histogram[i]=0;
    }
    for(int i=0;i<scoreList.size();i++){
        histogram[scoreList[i]]++;
    }
    
    for(int i=0;i<index;i++){
        cout<<"score"<<i;
        for(int j=0;j<histogram[i];j++){
            cout<<"@";
        }
        cout<<endl;
    }
}