#include <stdio.h>
#include <stdlib.h>
typedef struct{
    int row;
    int column;
}Matrix;
int** CreateMatrix(int row,int column){
    int** arr=(int**)malloc(sizeof(int*)*row);
    //동적할당은 결국 포인터를 이용하는 것이다.
    //그렇다면 1차원배열은 포인터이고, 그 포인터를 가리키기 위해서는 상위의 것은 이중포인터일 수 밖에 없다.
    for(int i=0;i<row;i++){
        arr[i]=(int*)malloc(sizeof(int)*column);
    }
    return arr;
}
void RemoveAllocation(int** arr,Matrix mat){
    for(int i=0;i<mat.row;i++){
        free(arr[i]);
    }
    free(*arr);
    printf("All right! \n");
}
void ReceiveMatrixRowAndColumn(int* row, int* column){
    printf("Size of row:");
    scanf("%d",row);
    printf("Size of column:");
    scanf("%d",column);
}
void InitializeMatrix(int** arr,Matrix mat){
    int input;
    for(int i=0;i<mat.row;i++){
        printf("please input %d number on line %d \n",mat.column,i+1);
        for(int j=0;j<mat.column;j++){
            scanf("%d", &input);
            arr[i][j]=input;
        }
    }
}
int** CaculateMatrix(int** first,int** second,Matrix mat1,Matrix mat2){
    //첫번째의 열이 행, 두번째의 행이 열
    int result_row=mat1.row;
    int result_column=mat2.column;
    int** result=CreateMatrix(result_row,result_column);
    for(int i=0;i<result_row;i++){
        for(int k=0;k<result_column;k++){
            result[i][k]=0;
            for(int j=0;j<mat1.column;j++){
                result[i][k]+=first[i][j]*second[j][k];
            }
        }
    }       
    return result;
}
int main(void){
    Matrix mat1;
    Matrix mat2;
    printf("This Program is matrix-multipying-program.\n");
    printf("please input size of row and column of First matrix.\n");
    ReceiveMatrixRowAndColumn(&mat1.row,&mat1.column);
    printf("please input size of row and column of Second matrix. \n");
    ReceiveMatrixRowAndColumn(&mat2.row,&mat2.column);
    if(mat1.row != mat2.column || mat1.row<=0 || mat1.column <=0 || mat2.row<=0 || mat2.column<=0){
        printf("Sorry. We can't caculate it.");
        return 0;
    }
    int** matrix1=CreateMatrix(mat1.row,mat1.column);
    int** matrix2=CreateMatrix(mat2.row,mat2.column);
    printf("please input Elements of First matrix. \n");
    InitializeMatrix(matrix1,mat1);
    printf("please input Elements of Second matrix. \n");
    InitializeMatrix(matrix2,mat2);
    printf("Now Caculating...\n");
    int** result=CaculateMatrix(matrix1,matrix2,mat1,mat2);
    Matrix mat_result={mat1.row,mat2.column};
    for(int i=0;i<mat_result.row;i++){
        for(int j=0;j<mat_result.column;j++){
            printf("%d ",result[i][j]);
        }
        printf("\n");
    }
    RemoveAllocation(matrix1,mat1);
    RemoveAllocation(matrix2,mat2);
    RemoveAllocation(result,mat_result);
}