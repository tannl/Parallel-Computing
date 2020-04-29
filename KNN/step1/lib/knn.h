#ifndef _KNN_H_
#define _KNN_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define M 20000
#define N 100
#define K 100

//定义一个字符的结构体
typedef struct letter{
    char c;
    int array[16];
    float distance;
}letter;

//定义训练字符结构体数组,共有M个训练样本
letter letters[M];

//识别字符类数组，共有N个对比样本
letter nletters[N];

letter nletters_temp[10];
//定义读取训练文件函数，将训练样本从磁盘文件letter.txt读入letters[M]数组中
void Get_from_letters();

//定义读取测试文件，将测试样本从磁盘文件素描sum1.txt读入到nletters中
void Get_from_nletters();

//定义欧式距离函数，计算一个测试样本与各个训练样本之间的距离
void Distance(letter *p);

//排序函数将letters距离按由小到大排列
void select_Sort();


//根据用户输入的k值，确定分类
char Knn(int q);
#endif //_KNN_H_