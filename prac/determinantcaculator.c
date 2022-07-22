#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
//무슨 열과 행이 제외당하는가도 필요함.
int** ModifyMatrix(Matrix* mat,int** original,int excluded_row,int excluded_column){
    int arr_row=(*mat).row-1;
    int arr_column=(*mat).column-1;
    int** arr=CreateMatrix(arr_row,arr_column);
    int current_arr_row=0;
    int current_arr_column=0;
    for(int i=0;i<(*mat).row;i++){
        int cnt=0;
        for(int j=0;j<(*mat).column;j++){
            if(i==excluded_row-1 || j==excluded_column-1){
                cnt++;
                continue;
            }else{
                arr[current_arr_row][current_arr_column]=original[i][j];
                current_arr_column++;
            }
        }
        if(cnt<(*mat).row){
            current_arr_row++;
        }
        current_arr_column=0;
    }
    (*mat).row-=1;
    (*mat).column-=1;
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
    int input=0;
    while(input<=0){
        printf("Size of determinant:");
        scanf("%d",&input);
    }
    *row=input;
    *column=input;
}
void InitializeMatrix(int** arr,Matrix mat){
    int input;
    for(int i=0;i<mat.row;i++){
        printf("please input %d unsigned integer on line %d \n",mat.column,i+1);
        for(int j=0;j<mat.column;j++){
            scanf("%d", &input);
            arr[i][j]=input;
        }
    }
}
int CaculateDeterminant(int** arr,Matrix* mat){
    int result=0;
    if((*mat).row==2){
        result=arr[0][0]*arr[1][1]-arr[0][1]*arr[1][0];
    }
    else{
        for(int j=0;j<mat->column;j++){
            Matrix copy=*mat;
            int** modarr=ModifyMatrix(&copy,arr,1,j+1);
            result+=(int)(pow((double)-1,(double)j))*arr[0][j]*CaculateDeterminant(modarr,&copy);
        }
        
    }
    return result;
}
int main(void){
    Matrix mat1;
    printf("This Program is determinant solving program.\n");
    printf("please input size of determinant.\n");
    ReceiveMatrixRowAndColumn(&mat1.row,&mat1.column);
    int** matrix1=CreateMatrix(mat1.row,mat1.column);
    printf("please input Elements of determinant. \n");
    InitializeMatrix(matrix1,mat1);
    printf("Now Caculating...\n");
    int result=CaculateDeterminant(matrix1,&mat1);
    printf("det is %d. \n",result);
    RemoveAllocation(matrix1,mat1);
    

}