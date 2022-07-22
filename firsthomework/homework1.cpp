#include <iostream>
using namespace std;

class TwoD{
    private:
        double** arr;
        int maxRows;
        int maxCols;
    public:
        TwoD();
        TwoD(int row,int col) ;
        TwoD(TwoD& source);
        int getMaxCols(){return maxCols;};
        int getMaxRows(){return maxRows;};
        void setDouble(int row,int col,double num);
        double getDouble(int row,int col);
        friend TwoD operator+(const TwoD& left,const TwoD& right);
        TwoD& operator=(const TwoD& right);
        ~TwoD();                

};
int main(){
    int matrix1_Row,matrix1_Col,matrix2_Row,matrix2_Col;
    //초기화 과정
    cout<<"please input size of first matrix : ";
    cin>>matrix1_Row>>matrix1_Col;
    cin.clear();
    TwoD matrix1(matrix1_Row,matrix1_Col);
    cout<<"please input real number elements in "<<matrix1_Row<<"X"<<matrix1_Col<<": ";
    for(int i=0;i<matrix1.getMaxRows();i++){
        for(int j=0;j<matrix1.getMaxCols();j++){
            double num;
            cin>>num;
            matrix1.setDouble(i,j,num);
        }
    }

    cout<<"please input size of second matrix :";
    cin>>matrix2_Row>>matrix2_Col;
    cin.clear();
    TwoD matrix2(matrix2_Row,matrix2_Col);
        cout<<"please input real number elements in "<<matrix2_Row<<"X"<<matrix2_Col<<": ";
    for(int i=0;i<matrix2.getMaxRows();i++){
        for(int j=0;j<matrix2.getMaxCols();j++){
            double num;
            cin>>num;
            matrix2.setDouble(i,j,num);
        }
    }
    //클래스 객체 생성이 잘 됐는지 확인
    for(int i=0;i<matrix1.getMaxRows();i++){
        for(int j=0;j<matrix1.getMaxCols();j++){
            cout<<matrix1.getDouble(i,j)<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    for(int i=0;i<matrix2.getMaxRows();i++){
        for(int j=0;j<matrix2.getMaxCols();j++){
            cout<<matrix2.getDouble(i,j)<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    //복사생성자
    cout<<"now test copy constructor."<<endl;
    TwoD res1(matrix1);
    for(int i=0;i<res1.getMaxRows();i++){
        for(int j=0;j<res1.getMaxCols();j++){
            cout<<res1.getDouble(i,j)<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    //등호
    cout<<"now test operator 'equal to'."<<endl;
    res1=matrix2;
    for(int i=0;i<res1.getMaxRows();i++){
        for(int j=0;j<res1.getMaxCols();j++){
            cout<<res1.getDouble(i,j)<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    //더하기
    cout<<"now test operator 'plus'."<<endl;
    res1=matrix1+matrix2;
    for(int i=0;i<res1.getMaxRows();i++){
        for(int j=0;j<res1.getMaxCols();j++){
            cout<<res1.getDouble(i,j)<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    //소멸자
    res1.~TwoD();
    return 0;
}
double TwoD::getDouble(int row,int col){
    return arr[row][col];
}
void TwoD::setDouble(int row,int col,double num){
    arr[row][col]=num;
}
TwoD::TwoD():maxRows(7),maxCols(7){
    arr=new double*[maxRows];
    for(int i=0;i<maxRows;i++){
        arr[i]=new double[maxCols];
    }
};
TwoD::TwoD(int row,int col):maxRows(row),maxCols(col){
    arr=new double*[maxRows];
    for(int i=0;i<maxRows;i++){
        arr[i]=new double[maxCols];
    }
};
TwoD::TwoD(TwoD& source):maxRows(source.maxRows),maxCols(source.maxCols){
    arr=new double*[maxRows];
    for(int i=0;i<maxRows;i++){
        arr[i]=new double[maxCols];
        for(int j=0;j<maxCols;j++){
            arr[i][j]=source.arr[i][j];
        }
    }
    
};

TwoD& TwoD::operator=(const TwoD& right){
    if(maxRows!=right.maxRows || maxCols!=right.maxCols){
        delete [] arr;
        arr=new double*[right.maxRows];
        for(int i=0;i<right.maxRows;i++){
            arr[i]=new double[right.maxCols];
        }
    }
    maxCols=right.maxCols;
    maxRows=right.maxRows;
    for(int i=0;i<maxRows;i++){
        for(int j=0;j<maxCols;j++){
            arr[i][j]=right.arr[i][j];
        }
    }
    return *this;
};
TwoD::~TwoD(){
    for(int i=0;i<maxRows;i++){
        delete [] arr[i];
    }
    delete [] arr;
    cout<<"delete!"<<endl;
};
TwoD operator+(const TwoD& left, const TwoD& right){
    if(left.maxCols!=right.maxCols || right.maxRows != left.maxRows){
        cout<<"we can't caculate this because they have different size each other. \n";
        exit(1);
    }else{
        int resMaxRows=left.maxRows;
        int resMaxCols=left.maxCols;
        TwoD res(resMaxRows,resMaxCols);
        for(int i=0;i<resMaxRows;i++){
            for(int j=0;j<resMaxCols;j++){
                   double sum=left.arr[i][j]+right.arr[i][j];
                   res.setDouble(i,j,sum);
            }
        }
        return res;
    }
};