#include "../lib/knn.h"
#include <stdio.h>
int main(int argc, char **argv){
    int index_i;
    int correct_Number=0,error_Number=0;
    letter * p;
    char c;
    printf("训练样本为%d\n",M);
    Get_from_letters();
    printf("测试样本为%d\n",N);
    Get_from_nletters();
    for(index_i=0; index_i<N; index_i++){
        p=&nletters[index_i];
        Distance(p);
        Sort();
        c=Knn(K);
        if(nletters[index_i].c==c){
            printf("第%d个字符属于%c类,识别正确!\n",index_i+1,nletters[index_i].c);
            correct_Number++;
        }
        else{
            printf("第%d个字符属于%c类，识别为%c类,识别错误!\n",index_i+1, nletters[index_i].c, c);
            error_Number++;
        }
    }
    printf("正确个数为%d",correct_Number);
    printf("错误个数为%d",error_Number);
    printf("正确率为%f",(float)(correct_Number)/N);
    return 0;
}
