#include <stdio.h>
#include "LatinCube.h"
#include "ChaoticSystem.h"

//int shuffleCoordinates[512][512][8] = {0};

int indexMatrix[128] = {0};


int z_coor[2097163];
int x_coor[2097163];
int y_coor[2097163];

void generatingKeySeq(double* chaoticSeq, ipvLorenz* ipvLnz, int len, int preflag) {
    int i, j = 0;
    if (preflag)
        preIterateIpvLorenz(500, ipvLnz);
    for (i = 0; i < len/3; ++i) {
        iterateIpvdLorenz(1, ipvLnz);
        chaoticSeq[j++] = ipvLnz->x;
        chaoticSeq[j++] = ipvLnz->y;
        chaoticSeq[j++] = ipvLnz->z;
    }
    //printf("asf");
    iterateIpvdLorenz(1, ipvLnz);
    switch(len%3) {
        case 0:;
        case 1:
            iterateIpvdLorenz(1, ipvLnz);
            chaoticSeq[j++] = ipvLnz->x;
        case 2:
            iterateIpvdLorenz(1, ipvLnz);
            chaoticSeq[j++] = ipvLnz->x;
            chaoticSeq[j++] = ipvLnz->y;
        default:;
    }
    for (i = 0; i < 128; ++i) {
        indexMatrix[i] = i;
    }
//    for (k = 0; k < 3; ++k)
//        printf("%lf ", chaoticSeq[k]);
}

void quick_sort_shuffleIndex(double chaoticSeq[], int* indexMatrix, int l, int r) {
    int tmp = 0;
//    int k;
//    for (k = 0; k < 3; ++k)
//        printf("%lf ", chaoticSeq[k]);
//    getchar();
    if(l >= r) return;
    int i = l, j = r; 
    double x = chaoticSeq[i];


    //if (l < r) {
        //int i, j;
        //double x;
        //i = l;
        //j = r;
        //x = chaoticSeq[i];
        while (i < j) {
            while (i < j && chaoticSeq[j] > x) {
                --j;
            }
            if (i < j) {
                tmp = indexMatrix[i];
                indexMatrix[i] = indexMatrix[j];
                indexMatrix[j] = tmp;
                chaoticSeq[i++] = chaoticSeq[j];
            }

            while (i < j && chaoticSeq[i] < x) {
                ++i;
            }
            if (i < j) {
                tmp = indexMatrix[i];
                indexMatrix[i] = indexMatrix[j];
                indexMatrix[j] = tmp;
                chaoticSeq[j--] = chaoticSeq[i];
            }
        }
        chaoticSeq[i] = x;
        quick_sort_shuffleIndex(chaoticSeq, indexMatrix, l, i - 1);
        quick_sort_shuffleIndex(chaoticSeq, indexMatrix, i + 1, r);
    //}
//    int k;
//    for (k = 0; k < 3; ++k)
//        printf("%d ", indexArray[k]);
}

void sortIntegerindex(int chaoticSeq[], int indexArray[], int l, int r) {
    int tmp = 0;
    if (l < r) {
        int i, j;
        int x;
        i = l;
        j = r;
        x = chaoticSeq[i];
        while (i < j) {
            while (i < j && chaoticSeq[j] > x) {
                --j;
            }
            if (i < j) {
                tmp = indexArray[i];
                indexArray[i] = indexArray[j];
                indexArray[j] = tmp;
                chaoticSeq[i++] = chaoticSeq[j];
            }
            while (i < j && chaoticSeq[i] < x) {
                ++i;
            }
            if (i < j) {
                tmp = indexArray[i];
                indexArray[i] = indexArray[j];
                indexArray[j] = tmp;
                chaoticSeq[j--] = chaoticSeq[i];
            }
        }
        chaoticSeq[i] = x;
        sortIntegerindex(chaoticSeq, indexArray, l, i-1);
        sortIntegerindex(chaoticSeq, indexArray, i+1, r);
    }
}

void generate3DLatinCube(int* indexMatrix) {
    int i, j, k, m = 0;
    //int tmp[8] = {0}, newVal[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    //int newVal[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    int index[128][128][128] = {0};       //512*512*8这里还是应该用一维的形式，要不然不太好理解，然后排序的时候分别排序
    int index1[128][128][128] = {0};
    int index2[128][128][128] = {0};
    for (i = 0; i < 128; ++i) {
        for (j = 0; j < 128; ++j) {
            for (k = 0; k < 128; ++k) {
                index[i][j][k] = (indexMatrix[k] + indexMatrix[j] + indexMatrix[i]) % 128;
                x_coor[m] = index[i][j][k];
                index1[i][j][k] = (indexMatrix[k] + 2 * indexMatrix[j] + 4 * indexMatrix[i]) % 128;
                y_coor[m] = index1[i][j][k];
                index2[i][j][k] = (indexMatrix[k] + 3 * indexMatrix[j] + 9 * indexMatrix[i]) % 128;
                z_coor[m] = index2[i][j][k];
                m ++;

//                tmp[k] = index2[i][j][k];
//                ++m;
            }
        }
    }
}





