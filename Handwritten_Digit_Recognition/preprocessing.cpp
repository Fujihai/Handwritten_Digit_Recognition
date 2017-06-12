#include "preprocessing.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

void findX(IplImage* imgSrc,int* min, int* max)             //找到ROI区域x的最小值和最大值
{
    /**
     * 主要思路：遍历每列，只要找到有一列存在黑点，就是x的最小值
     *         再接着找x的最大值
     */
    int i;
    int minFound=0;
    CvMat data;
    CvScalar maxVal = cvRealScalar(imgSrc->height * 255);
    CvScalar val = cvRealScalar(0);

    for (i = 0; i < imgSrc->width; i++)
    {
        cvGetCol(imgSrc, &data, i);
        val = cvSum(&data);
        if(val.val[0] < maxVal.val[0])
        {
            *max = i;
            if(!minFound)
            {
                *min = i;
                minFound = 1;
            }
        }
    }
}

void findY(IplImage* imgSrc,int* min, int* max)         //找到ROI区域x的最小值和最大值
{
    /**
     * 主要思路：遍历每行，只要找到有一行存在黑点，就是y的最小值
     *         再接着找y的最大值
     */
    int i;
    int minFound = 0;
    CvMat data;
    CvScalar maxVal = cvRealScalar(imgSrc->width * 255);
    CvScalar val = cvRealScalar(0);

    for (i = 0; i< imgSrc->height; i++)
    {
        cvGetRow(imgSrc, &data, i);
        val= cvSum(&data);
        if(val.val[0] < maxVal.val[0])
        {
            *max = i;
            if(!minFound)
            {
                *min = i;
                minFound = 1;
            }
        }
    }
}
CvRect findBB(IplImage* imgSrc)
{
    CvRect aux;
    int xmin, xmax, ymin, ymax;
    xmin = xmax = ymin = ymax = 0;

    findX(imgSrc, &xmin, &xmax);
    findY(imgSrc, &ymin, &ymax);

    aux = cvRect(xmin, ymin, xmax-xmin+1, ymax-ymin+1);

    return aux;
}

IplImage preprocessing(IplImage* imgSrc,int new_width, int new_height)        //new_width，new_height为目标图像的宽高值
{
    IplImage* result;
    IplImage* scaledResult;

    CvMat data;
    CvMat dataA;
    CvRect bb;
    CvRect bba;

    bb = findBB(imgSrc);

    cvGetSubRect(imgSrc, &data, cvRect(bb.x, bb.y, bb.width, bb.height));    //得到边界框的数据并且没有纵横比，边界框的x和y可以变化
    int size = (bb.width > bb.height) ?  bb.width : bb.height;               //创建宽高比为1的图像，及矩形，边长为宽和高中的较大值
    result = cvCreateImage(cvSize(size, size), 8, 1);
    cvSet(result, CV_RGB(255,255,255), NULL);

    int x=(int)floor((float)(size-bb.width) / 2.0f);                         //将图像放在正中间，大小归一化
    int y=(int)floor((float)(size-bb.height) / 2.0f);
    cvGetSubRect(result, &dataA, cvRect(x,y,bb.width, bb.height));
    cvCopy(&data, &dataA, NULL);

    scaledResult = cvCreateImage( cvSize(new_width, new_height), 8, 1 );    //按比例缩放结果
    cvResize(result, scaledResult, CV_INTER_NN);

    return *scaledResult;                                                   //直接返回处理以后的图片
}
