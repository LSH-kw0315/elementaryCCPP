#include <iostream>
#include <string>
using namespace std;
class DynamicStringArray{
    private:
        string* dynamicArray;
        int size;
    public:
        DynamicStringArray();
        DynamicStringArray(DynamicStringArray& source);
        int getSize(){return size;};
        void addEntry(string str);
        bool deleteEntry(string str);
        string getEntry(int num);
        DynamicStringArray& operator=(const DynamicStringArray& right);
        ~DynamicStringArray();

};
int main(){
    DynamicStringArray arr1;
    string str;
    cout<<"input 5 elements."<<endl;
    for(int i=0;i<5;i++){
        cin>>str;
        arr1.addEntry(str);
        cout<<endl<<arr1.getEntry(i)<<" is inputted."<<endl;
    }
    cout<<endl<<endl<<arr1.getSize()<<endl<<endl;
    
    while(true){
        cout<<"please input what you want to delete."<<endl;
        cin>>str;
        if(arr1.deleteEntry(str)){
            break;
        }
    }
    DynamicStringArray arr2(arr1);
    for(int i=0;i<arr2.getSize();i++){
        cout<<"element"<<i+1<<" is "<<arr2.getEntry(i)<<"."<<endl;
    }
    cout<<endl<<endl;

    arr2.addEntry("testing");
    DynamicStringArray arr3=arr2;
    for(int i=0;i<arr3.getSize();i++){
        cout<<"element"<<i+1<<" is "<<arr3.getEntry(i)<<"."<<endl;
    }


}
DynamicStringArray::DynamicStringArray(){
    size=0;
    dynamicArray=NULL;
}
DynamicStringArray::DynamicStringArray(DynamicStringArray& source){
    size=source.size;
    dynamicArray=new string[size];
    for(int i=0;i<size;i++){
        dynamicArray[i]=source.dynamicArray[i];
    }
}
void DynamicStringArray::addEntry(string str){
    string* temp=new string[size+1];
    for(int i=0;i<size;i++){
        temp[i]=dynamicArray[i];
    }
    temp[size]=str;
    delete[] dynamicArray;
    dynamicArray=temp;
    size++;
    temp=NULL;
}
bool DynamicStringArray::deleteEntry(string str){
    for(int i=0;i<size;i++){
        if(dynamicArray[i]==str){
            string* temp=new string[size-1];
            int cnt=0;
            for(int i=0;i<size;i++){
                 if(cnt>=size-1){
                    break;
                }
                if(dynamicArray[i]==str){
                    continue;
                }
                temp[i]=dynamicArray[i];
                cnt++;
            }
            delete[] dynamicArray;
            dynamicArray=temp;
            size--;
            temp=NULL;
            return true;
        }
    }
    return false;
}
string DynamicStringArray::getEntry(int num){
    if(num>size){
        return NULL;
    }else{
        return dynamicArray[num];
    }
}
DynamicStringArray& DynamicStringArray::operator=(const DynamicStringArray& right){
    if(size!=right.size){
        delete[] dynamicArray;
        dynamicArray=new string[right.size];
    }
    size=right.size;
    for(int i=0;i<size;i++){
        dynamicArray[i]=right.dynamicArray[i];
    }
    return *this;
}
DynamicStringArray::~DynamicStringArray(){
    delete[] dynamicArray;
    dynamicArray=NULL;
}