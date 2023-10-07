#ifndef SIMULTANEOUSPMDF_H_INCLUDED
#define SIMULTANEOUSPMDF_H_INCLUDED

#include "ReadBatchImg.h"
#include "LatinCube.h"

extern int diffusionKSLen;
extern int imgLen;

//坐标还是用一维的比较好
extern void PermutationDiffusion(batchImgInfo *imgsInfo, unsigned char ***resizedImgData, unsigned char *tmpImgData, double* chaoticSeq);

extern void dePermutationDiffusion(batchImgInfo *imgsInfo, unsigned char ***resizedImgData,
                                   unsigned char ***decryptionImgdata, unsigned char *tmpImgData, double* chaoticSeq);

//逆置乱的时候还是按照原来的顺序进行交换
#endif // SIMULTANEOUSPMDF_H_INCLUDED
