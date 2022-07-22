#include <iostream>
#include <string>
using namespace std;
class Student{
    private:
         string name;
         int numClasses;
         string* classList;
    public:
        Student();
        void setStudent();
        void printAllInfo();
        void printAllClasses();
        void printName();
        void printClassNum();

       
        void empty();

        Student& operator=(const Student& right);

        ~Student();

};
int main(){
   Student st1;
   Student st2;
   st1.setStudent();
   st2.setStudent();
   st1.printAllInfo();
   st1.empty();
   st1.printAllInfo();
   st1=st2;
   st1.printAllInfo();  
}
Student::Student():name(""),numClasses(0){
    classList=NULL;
};

void Student::printAllClasses(){
    cout<<"All classes that "<<name<<" is taking are ";
    for(int i=0;i<numClasses;i++){
        if(i<numClasses-1){
            cout<<classList[i]<<", ";
        }else{
            cout<<classList[i]<<".";
        }
    }
    cout<<endl<<endl;
};
void Student::printClassNum(){
    cout<<name<<" is taking "<<numClasses<<"Classes."<<endl;
};
void Student::printName(){
    cout<<"The name is "<<name<<"."<<endl;
};
void Student::printAllInfo(){
    printName();
    printClassNum();
    printAllClasses();
};

void Student::empty(){
    delete[] classList;
    classList=NULL;
    numClasses=0;
};
Student& Student::operator=(const Student& right){
    name=right.name;
    if(numClasses!=right.numClasses){
        delete[] classList;
        classList=new string[right.numClasses];      
    }
    numClasses=right.numClasses;
    for(int i=0;i<numClasses;i++){
        classList[i]=right.classList[i];
    }
    return (*this); 
};
Student::~Student(){
    delete[] classList;
    classList=NULL;
}
void Student::setStudent(){
    cout<<"please input student's name :";
    cin>>name;//공백도 가능
    cout<<"please input the number of classes:";
    cin>>numClasses;
    cout<<"please input the name of classes below.";
    classList=new string[numClasses];
    for(int i=0;i<numClasses;i++){
        cin>>classList[i];
    }
}