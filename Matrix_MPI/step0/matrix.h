#ifndef _MATRIX_H
#define _MATRIX_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ROW 2000
#define COL 2000
#define LEN 2000
#define MATRIX_MALLOC(type,matrix_row,matrix_col) \
( (type(*)[matrix_row])malloc(sizeof(type)*matrix_row*matrix_col) )

double (*matrix1)[COL];
double (*matrix2)[COL];
double (*matrix3)[COL];
double (*matrix4)[COL];

void read_Data();
void check_Data();
void matrix_Compute();

#endif