#include "ReadBatchImg.h"

char imgNames[Max_File_Num][File_Name_Len] = {{'\0'}};
char outputImgNames[Max_File_Num][File_Name_Len] = {{'\0'}};
int imgNum = 0;

int readBatchImages(char* imgsPath) {

    int i = 0;
    DWORD fileattr;
    DIR* drstreamob;                            //目录流对象
    struct dirent* dirp;                        //指向目录具体对象
    drstreamob = opendir(imgsPath);              //返回参数所指的目录流对象
    while ((dirp = readdir(drstreamob))) {        //读取目录流对象
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)      //跳过..和.
            continue;
        fileattr = GetFileAttributes(dirp->d_name);                //这是一个目录的话

        strcpy(imgNames[i], imgsPath);                      //先赋值路径
        strcpy(outputImgNames[i], imgsPath);
        strcat(outputImgNames[i], "CipherImages\\");
        mkdir(outputImgNames[i]);

        strcat(imgNames[i], dirp->d_name);                 //赋值文件名
        strcat(outputImgNames[i], dirp->d_name);            //输出的文件名

        fileattr = GetFileAttributes(imgNames[i]);
        if (fileattr & FILE_ATTRIBUTE_DIRECTORY) {
            memset(imgNames[i], 0, File_Name_Len);
            memset(outputImgNames[i], 0, File_Name_Len);
            continue;
        }
        ++i;
    }
    return i;
}

void loadBatchImageInfo(batchImgInfo* imgsInfo) {
    int i = 0, j = 0, m = 0, k = 0;
    FILE *fp[imgNum];
    int colormap_size = 0;
    //rgb_quad* quad;

    FILEHEADER filehead;
    INFOHEADER fileInfo;
    //imgsInfo = (batchImgInfo*)malloc(sizeof(batchImgInfo) * imgNum);


    for (i = 0; i < imgNum; ++i) {
        k = 0;
        if ((fp[i] = fopen(imgNames[i], "rb")) == NULL) {
            printf("File open error...\n");
            return;
        }
        fread(&filehead, 1, 14, fp[i]);
        fread(&fileInfo, 1, 40, fp[i]);
        //printf("%s\n", imgNames[i]);




        imgsInfo[i].width = fileInfo.biWidth;
        imgsInfo[i].height = fileInfo.biHeight;
        imgsInfo[i].imglen = imgsInfo[i].width * imgsInfo[i].height;

        imgsInfo[i].imgData = (unsigned char*)malloc(sizeof(unsigned char) * imgsInfo[i].imglen);

        imgsInfo[i].bitCounts = 8;
        colormap_size = 4 * 256;

        imgsInfo[i].offbits = colormap_size + 54;

        imgsInfo[i].imgHeader = (unsigned char*)malloc(sizeof(unsigned char) * imgsInfo[i].offbits);
        fseek(fp[i], 0, SEEK_SET);          //把文件指针复位到起始位置
        fread(imgsInfo[i].imgHeader, 1, imgsInfo[i].offbits, fp[i]);

        fread(imgsInfo[i].imgData, 1, imgsInfo[i].imglen, fp[i]);        //读取文件头和信息头

        fclose(fp[i]);
    }
}

void writeBatchImg(batchImgInfo* imgsInfo) {

    int i = 0, j = 0, m = 0, k = 0;
    FILE *fp[imgNum];
    int width = 0, height = 0;
    //printf("%d", imgNum);
    for (i = 0; i < imgNum; ++i) {
        if ((fp[i] = fopen(outputImgNames[i], "wb")) == NULL) {
            printf("Error written error...\n");
            return;
        }
        //printf("here");
        fwrite(imgsInfo[i].imgHeader, 1, imgsInfo[i].offbits, fp[i]);
        fwrite(imgsInfo[i].imgData, 1, imgsInfo[i].imglen, fp[i]);

        fclose(fp[i]);
    }
}

void freeBatchImg(batchImgInfo* imgsInfo) {
    int i;
    for (i = 0; i < imgNum; ++i)
        free(imgsInfo[i].imgData);
    free(imgsInfo);
}