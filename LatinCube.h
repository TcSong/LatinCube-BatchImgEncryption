#ifndef LATINCUBE_H_INCLUDED
#define LATINCUBE_H_INCLUDED

#include <stdio.h>
#include "ChaoticSystem.h"

extern int indexMatrix[128];  //ÅÅÐòµÄË÷Òý


extern int z_coor[2097163];
extern int x_coor[2097163];
extern int y_coor[2097163];

//extern int shuffleCoordinates[512][512][8];
extern void generatingKeySeq(double* chaoticSeq, ipvLorenz* ipvLnz, int len, int preflag);

extern void quick_sort_shuffleIndex(double chaoticSeq[], int indexArray[], int l, int r);
extern void sortIntegerindex(int value[], int index[], int l, int r);
/*
 the name of chaotic indexmatrix; its length
 the length of first, second, third dimension of the Latin Cube
 */
extern void generate3DLatinCube(int *indexMatrix1);

//extern void generatePermutatoinCoordiantes()

#endif // LATINCUBE_H_INCLUDED
