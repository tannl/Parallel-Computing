#include "../lib/knn.h"
#include <stdio.h>
//定义读取训练文件函数，将训练样本从磁盘文件letter.txt读入letters[M]数组中
void Get_from_letters(){
    FILE  *fp;
    int inde_i,inde_j;
    fp=fopen("./data/letter.txt","r");
    if(!fp){
	    printf("letter.txt文件未找到！请确认文件路径！\n");
	    exit(0);
    }
    for(inde_i=0; inde_i<M; inde_i++){
        fscanf(fp,"%c ",&letters[inde_i].c);
        for(inde_j=0; inde_j<16; inde_j++)
            fscanf(fp,"%d ",&letters[inde_i].array[inde_j]);
    }
    fclose(fp);
}


//定义读取测试文件，将测试样本从磁盘文件素描sum1.txt读入到nletters中
void Get_from_nletters(){
    int inde_i,inde_j;
    FILE *fp;
    fp=fopen("./data/sum.txt","r");
    if(!fp){
	    printf("sum.txt文件未找到！请确认文件路径！\n");
	    exit(0);
    }
    for(inde_i=0; inde_i<N; inde_i++){
        fscanf(fp,"%c ",&nletters[inde_i].c);
        for(inde_j=0; inde_j<16; inde_j++)
            fscanf(fp,"%d ",&nletters[inde_i].array[inde_j]);
    }
    fclose(fp);
}

//定义欧式距离函数，计算一个测试样本与各个训练样本之间的距离
void Distance(letter *p){
    int inde_i,inde_j;
    float sum=0.0;
    for(inde_i=0; inde_i<M; inde_i++){
        for(inde_j=0; inde_j<16; inde_j++){
            sum+=((letters[inde_i].array[inde_j]-(*p).array[inde_j])*(letters[inde_i].array[inde_j]-(*p).array[inde_j]));
        }
        letters[inde_i].distance=sqrt(sum);
        //恢复到原始值
        sum=0.0;
    }


}


//排序函数将letters距离按由小到大排列
void Sort(){
    int inde_i,inde_j;
    letter t;
    for(inde_i=0; inde_i<M-1; inde_i++){
        for(inde_j=inde_i+1; inde_j<M-1; inde_j++){
            if(letters[inde_i].distance>letters[inde_j].distance){
                t=letters[inde_i];
                letters[inde_i]=letters[inde_j];
                letters[inde_j]=t;

            }
        }
    }

}



//根据用户输入的k值，确定分类
char Knn(int q){
    int inde_i,inde_j,max;
    int array[26];
    for(inde_i=0; inde_i<26; inde_i++){
        array[inde_i]=0;
    }

    for(inde_i=0; inde_i<q; inde_i++){
        switch(letters[inde_i].c){
        case'A':
            array[0]++;
            break;
        case'B':
            array[1]++;
            break;
        case'C':
            array[2]++;
            break;
        case'D':
            array[3]++;
            break;
        case'E':
            array[4]++;
            break;
        case'F':
            array[5]++;
            break;
        case'G':
            array[6]++;
            break;
        case'H':
            array[7]++;
            break;
        case'I':
            array[8]++;
            break;
        case'J':
            array[9]++;
            break;
        case'K':
            array[10]++;
            break;
        case'L':
            array[11]++;
            break;
        case'M':
            array[12]++;
            break;
        case'N':
            array[13]++;
            break;
        case'O':
            array[14]++;
            break;
        case'P':
            array[15]++;
            break;
        case'Q':
            array[16]++;
            break;
        case'R':
            array[17]++;
            break;
        case'S':
            array[18]++;
            break;
        case'T':
            array[19]++;
            break;
        case'U':
            array[20]++;
            break;
        case'V':
            array[21]++;
            break;
        case'W':
            array[22]++;
            break;
        case'X':
            array[23]++;
            break;
        case'Y':
            array[24]++;
            break;
        case'Z':
            array[25]++;
            break;
        }
    }

    max=array[0];
    inde_j=0;
    for(inde_i=0; inde_i<26; inde_i++){
        if(array[inde_i]>max){
            max=array[inde_i];
            inde_j=inde_i;
        }
    }
    return (char)(inde_j+65);
}

