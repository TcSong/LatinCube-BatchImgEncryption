#include <math.h>
#include "SimultaneousPMDF.h"

int diffusionKSLen = 2097152;
int imgLen = 262144;

void PermutationDiffusion(batchImgInfo *imgsInfo, unsigned char ***resizedImgdata, unsigned char *tmpdata, double *chaoticSeq) {
    int i, j, m = 0, k;
    unsigned char *diffusionKS = (unsigned char*)malloc(sizeof(unsigned char) * diffusionKSLen);
    unsigned char seed = 148;

    for (i = 0; i < imgNum; ++i) {
        for (j = 0; j < imgLen; ++j) {
            tmpdata[m] = imgsInfo[i].imgData[j];//原一维数据
            chaoticSeq[m] = fabs(chaoticSeq[m]);
            while (chaoticSeq[m] > 1)
                chaoticSeq[m] /= 10;
            chaoticSeq[m] *= (1.0e+15);//顺便处理量化一下密钥流
            diffusionKS[m] = (long long)(chaoticSeq[m]) & 255;      //扩散用密钥流
            m++;
        }
    }

    //latin cube based 置乱
//    m = 0;
//    for (i = 0; i < 128; ++i) {
//        for (j = 0; j < 128; ++j) {
//            for (k = 0; k < 128; ++k) {
//                //resizedImgdata[i][j][k] = tmpdata[m];
//                tmpdata[m] = resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]];
////                if (m != 0)
////                    tmpdata[m] = diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]] ^
////                                ((resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]] +
////                                  diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]]) & 255) ^ (tmpdata[m-1]);
////                else
////                    tmpdata[m] = diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]] ^
////                                ((resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]] +
////                                  diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]]) & 255) ^ (seed);
//                m++;
//            }
//        }
//    }


    m = 0;
    for (i = 0; i < 128; ++i) {
        for (j = 0; j < 128; ++j) {
            for (k = 0; k < 128; ++k) {
                resizedImgdata[i][j][k] = tmpdata[m];
                m++;
            }
        }
    }

    //加密
    m = 0;
    for (i = 0; i < 128; ++i) {
        for (j = 0; j < 128; ++j) {
            for (k = 0; k < 128; ++k) {
                //tmpdata[m] = resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]];
                if (m != 0)
//                    tmpdata[m] = tmpdata[m-1]^resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]]
//                                    ^ ks3d[x_coor[m]][y_coor[m]][z_coor[m]];
                    tmpdata[m] = diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]] ^
                                ((resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]] +
                                  diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]]) & 255) ^ (tmpdata[m-1]);

                else
//                    tmpdata[m] = seed^resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]]
//                                    ^ ks3d[x_coor[m]][y_coor[m]][z_coor[m]];
                    tmpdata[m] = diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]] ^
                                ((resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]] +
                                  diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]]) & 255) ^ (seed);
                m++;
            }
        }
    }


    //置乱后数据
    m = 0;
    for (i = 0; i < imgNum; ++i) {
        for (j = 0; j < 262144; ++j) {
            imgsInfo[i].imgData[j] = tmpdata[m];
            m++;
        }
    }


}


void dePermutationDiffusion(batchImgInfo *imgsInfo, unsigned char ***resizedImgdata,
                            unsigned char ***decryptionImgdata, unsigned char *tmpdata, double* chaoticSeq) {
    int i, j, m = 0, k;
    unsigned char seed = 148;

    unsigned char *diffusionKS = (unsigned char*)malloc(sizeof(unsigned char) * diffusionKSLen);

    //加密后的一维转三维
    for (i = 0; i < 128; ++i) {
        for (j = 0; j < 128; ++j) {
            for (k = 0; k < 128; ++k) {
                decryptionImgdata[i][j][k] = tmpdata[m];
                //tmpdata[m] = resizedImgdata[i][j][k];
                chaoticSeq[m] = fabs(chaoticSeq[m]);
                while (chaoticSeq[m] > 1)
                    chaoticSeq[m] /= 10;
                chaoticSeq[m] *= (1.0e+15);//顺便处理量化一下密钥流
                diffusionKS[m] = (long long)(chaoticSeq[m]) & 255;      //扩散用密钥流
                m++;
            }
        }
    }

    unsigned char preSeed = seed;
//    //解密置乱
//    m = 0;
//    for (i = 0; i < 128; ++i) {
//        for (j = 0; j < 128; ++j) {
//            for (k = 0; k < 128; ++k) {
////                resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]] = ((decryptionImgdata[i][j][k] ^ preSeed ^
////                                                                    diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]])
////                                                                   + 256 - diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]]) & 255 ;
////                preSeed = decryptionImgdata[i][j][k];
//                resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]] = decryptionImgdata[i][j][k];
//                m++;
//            }
//        }
//    }

    m = 0;
    for (i = 0; i < 128; ++i) {
        for (j = 0; j < 128; ++j) {
            for (k = 0; k < 128; ++k) {
                //resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]] = decryptionImgdata[i][j][k];
                resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]] = ((decryptionImgdata[i][j][k] ^ preSeed ^
                                                                    diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]])
                                                                   + 256 - diffusionKS[x_coor[m]*128*128+y_coor[m]*128+z_coor[m]]) & 255 ;
                //resizedImgdata[x_coor[m]][y_coor[m]][z_coor[m]] = preSeed^decryptionImgdata[i][j][k];
                preSeed = decryptionImgdata[i][j][k];
                m++;
            }
        }
    }

    m = 0;
    for (i = 0; i < 128; ++i) {
        for (j = 0; j < 128; ++j) {
            for (k = 0; k < 128; ++k) {
                //resizedImgdata[i][j][k] = tmpdata[m];
                tmpdata[m] = resizedImgdata[i][j][k];
                m++;
            }
        }
    }

    m = 0;
    for (i = 0; i < imgNum; ++i) {
        for (j = 0; j < 262144; ++j) {
            imgsInfo[i].imgData[j] = tmpdata[m];
            m++;
        }
    }

}
