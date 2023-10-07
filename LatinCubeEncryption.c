#include <stdio.h>
#include <stdlib.h>
#include "ReadBatchImg.h"
#include "SimultaneousPMDF.h"
#include "LatinCube.h"
#include "ChaoticSystem.h"

int main(int argc, char * argv[]) {
    int i = 0, j = 0;
    int KEYSTREAMLEN = 128;
    //int diffusionKSlen = 2097152;
    imgNum = readBatchImages(argv[1]);

    printf("%d\n", imgNum);
    batchImgInfo *imgsInfo = (batchImgInfo*)malloc(sizeof(batchImgInfo) * imgNum);

    loadBatchImageInfo(imgsInfo);

    unsigned char ***resizedImgdata;
    resizedImgdata = (unsigned char***)malloc(sizeof(unsigned char**) * 128);
    for (i = 0; i < 128; ++i) {
        resizedImgdata[i] = (unsigned char**)malloc(sizeof(unsigned char*) * 128);
        for (j = 0; j < 128; ++j) {
            resizedImgdata[i][j] = (unsigned char*)malloc(sizeof(unsigned char) * 128);
        }
    }   //用于加密阶段的三维转换

    unsigned char ***decryptionImgdata;
    decryptionImgdata = (unsigned char***)malloc(sizeof(unsigned char**) * 128);
    for (i = 0; i < 128; ++i) {
        decryptionImgdata[i] = (unsigned char**)malloc(sizeof(unsigned char*) * 128);
        for (j = 0; j < 128; ++j) {
            decryptionImgdata[i][j] = (unsigned char*)malloc(sizeof(unsigned char) * 128);
        }
    }   //用于解密阶段的三维转换
    unsigned char ***resizedImgdata1;
    resizedImgdata1 = (unsigned char***)malloc(sizeof(unsigned char**) * 128);
    for (i = 0; i < 128; ++i) {
        resizedImgdata1[i] = (unsigned char**)malloc(sizeof(unsigned char*) * 128);
        for (j = 0; j < 128; ++j) {
            resizedImgdata1[i][j] = (unsigned char*)malloc(sizeof(unsigned char) * 128);
        }
    }   //用于解密阶段的三维转换

    //unsigned char* tmpImgdata = (unsigned char*)malloc(sizeof(unsigned char) * diffusionKSLen);
    double *chaoticSeqShuffle = (double*)malloc(sizeof(double) * KEYSTREAMLEN);

    ipvLorenz ipvLnz;
    ipvLnz.x = 15.4565353637745;
    ipvLnz.y = 7.5625829485526;
    ipvLnz.z = 13.2584757411935;
    
    generatingKeySeq(chaoticSeqShuffle, &ipvLnz, KEYSTREAMLEN, 1);
    quick_sort_shuffleIndex(chaoticSeqShuffle, indexMatrix, 0, KEYSTREAMLEN-1);
    generate3DLatinCube(indexMatrix);

    unsigned char *tmpdata = (unsigned char*)malloc(sizeof(unsigned char) * diffusionKSLen);
    double *chaoticSeqDiffusion = (double*)malloc(sizeof(double) * diffusionKSLen);
    double *dediffusion = (double*)malloc(sizeof(double) * diffusionKSLen);


    generatingKeySeq(chaoticSeqDiffusion, &ipvLnz, diffusionKSLen, 0);
    for (i = 0; i < diffusionKSLen; ++i) {
        dediffusion[i] = chaoticSeqDiffusion[i];
    }
    PermutationDiffusion(imgsInfo, resizedImgdata, tmpdata, chaoticSeqDiffusion);//加密
    dePermutationDiffusion(imgsInfo, resizedImgdata, decryptionImgdata, tmpdata, dediffusion);//解密
    int m = 0, k = 0;


    //printf("%d\n", imgNum);
    writeBatchImg(imgsInfo);
    freeBatchImg(imgsInfo);
    //free(chaoticSeq);
    //free(indexMatrix);
    return 0;
}
