#include <iostream>
#include <cmath>
using namespace std;
class Polynomial{
    private:
        double* expression;
        int maxDegree;
    public:
        Polynomial();
        Polynomial(int degree);
        Polynomial(Polynomial& source);
        int getMaxDegree(){return maxDegree;};
        int getCoefficient(int degree){return expression[degree];};
        void setCoefficient(int degree,double coefficient){expression[degree]=coefficient;};
        double caculate(double x);
        Polynomial& operator=(const Polynomial& right);
        ~Polynomial();
        Polynomial operator+(const double num);
        friend Polynomial operator+(const double num,const Polynomial& right);
        Polynomial operator+(const Polynomial& right);
        Polynomial operator-(const double num);
        friend Polynomial operator-(const double num,const Polynomial& right);
        Polynomial operator-(const Polynomial& right);
        Polynomial operator*(const double num);
        friend Polynomial operator*(const double num,const Polynomial& right);
        Polynomial operator*(const Polynomial& right);
        void PrintingExpression();
};
int main(){

    Polynomial pol1;
    pol1=Polynomial();
    Polynomial pol2(2);
    
    for(int i=0;i<=pol1.getMaxDegree();i++){
        pol1.setCoefficient(i,1);
    }
    for(int i=0;i<=pol2.getMaxDegree();i++){
        pol2.setCoefficient(i,1);
    }
    pol1.PrintingExpression();
    pol2.PrintingExpression();
    //복사생성자 실험
    Polynomial copyRes(pol1);
    copyRes.PrintingExpression();
    //등호 실험
    Polynomial eqlRes;
    eqlRes=Polynomial();
    eqlRes=pol2;
    eqlRes.PrintingExpression();
    //연산실험1
    Polynomial addAndsubRes;
    addAndsubRes=pol1+5;
    addAndsubRes.PrintingExpression();
    addAndsubRes=5+addAndsubRes;
    addAndsubRes.PrintingExpression();
    addAndsubRes=addAndsubRes-5;
    addAndsubRes.PrintingExpression();
    addAndsubRes=5-addAndsubRes;
    addAndsubRes.PrintingExpression();
    addAndsubRes=pol1+pol2;
    addAndsubRes.PrintingExpression();
    //연산실험2
    Polynomial MulRes;
    MulRes=pol2*5;
    MulRes.PrintingExpression();
    MulRes=5*MulRes;
    MulRes.PrintingExpression();
    MulRes=pol1*pol2;
    MulRes.PrintingExpression();
    //결과실험
    double cacRes=MulRes.caculate(2);
    cout<<"The result is "<<cacRes;
}
Polynomial::Polynomial():maxDegree(3){
    expression=new double[maxDegree+1];
    for(int i=0;i<=maxDegree;i++){
        expression[i]=0;
    }
};
Polynomial::Polynomial(int degree):maxDegree(degree){
    expression=new double[maxDegree+1];
        for(int i=0;i<=maxDegree;i++){
        expression[i]=0;
    }
};
Polynomial::Polynomial(Polynomial& source):maxDegree(source.maxDegree){
    expression=new double[maxDegree+1];
    for(int i=0;i<=maxDegree;i++){
        expression[i]=source.expression[i];
    }
};


Polynomial Polynomial::operator+(const double num){
    Polynomial res(*this);
    res.expression[0]+=num;
    return res;
};
Polynomial operator+(const double num,const Polynomial& right){
    Polynomial res(right.maxDegree);
    for(int i=0;i<=res.maxDegree;i++){
        res.expression[i]=right.expression[i];
    }
    res.expression[0]+=num;
    return res;
};
Polynomial Polynomial::operator+(const Polynomial& right){
    int minimum=this->maxDegree<right.maxDegree?this->maxDegree:right.maxDegree;
    Polynomial res(*this);
    for(int i=0;i<=minimum;i++){
        res.expression[i]+=right.expression[i];
    }
    return res;
};
Polynomial Polynomial::operator-(const double num){
    Polynomial res(*this);
    res.expression[0]-=num;
    return res;  
};
Polynomial operator-(const double num,const Polynomial& right){
    Polynomial res(right.maxDegree);
    for(int i=0;i<=res.maxDegree;i++){
        res.expression[i]=right.expression[i];
    }
    res.expression[0]-=num;
    return res;
};
Polynomial Polynomial::operator-(const Polynomial& right){
    int minimum=this->maxDegree<right.maxDegree?this->maxDegree:right.maxDegree;
    Polynomial res(*this);
    for(int i=0;i<=minimum;i++){
        res.expression[i]-=right.expression[i];
    }
    return res;
};
Polynomial Polynomial::operator*(const double num){
    Polynomial res(*this);
    for(int i=0;i<=res.maxDegree;i++){
        res.expression[i]*=num;
    }
    return res;
};
Polynomial operator*(const double num,const Polynomial& right){
    Polynomial res(right.maxDegree);
    for(int i=0;i<=res.maxDegree;i++){
        res.expression[i]=right.expression[i];
        res.expression[i]*=num;
    }
    return res;
};
Polynomial Polynomial::operator*(const Polynomial& right){
    int newDegree=this->maxDegree+right.maxDegree;
    Polynomial res(newDegree);
    for(int i=0;i<=this->maxDegree;i++){
        for(int j=0;j<=right.maxDegree;j++){
            res.expression[i+j]+=this->expression[i]*right.expression[j];
        }
    }
    return res;
};
Polynomial& Polynomial :: operator=(const Polynomial& right){
    if(right.maxDegree!=maxDegree){
        delete[] expression;
        expression=new double[(right.maxDegree)+1];
        maxDegree=right.maxDegree;
    }
    for(int i=0;i<=maxDegree;i++){
        expression[i]=right.expression[i];
    }
    return *this;
};
Polynomial::~Polynomial(){
    delete[] expression;
};
double Polynomial::caculate(double x){
    double res=0;
    for(int i=0;i<=maxDegree;i++){
        res+=expression[i]*pow(x,i);
    }
    return res;
};
void Polynomial::PrintingExpression(){
    cout<<"The Expression is.."<<endl;
    for(int i=maxDegree;i>=0;i--){
        if(i>0){
            cout<<expression[i]<<"X^"<<i<<" + ";
        }else{
            cout<<expression[i];
        } 
    }
    cout<<endl<<endl;
}