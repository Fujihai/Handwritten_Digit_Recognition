#ifndef BASIC_OCR_H
#define BASIC_OCR_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/ml.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <QDebug>

class basicOCR
{
private:
    char filePath[255];         //训练样本路径
    int trainSamples;           //训练样本数
    int classes;                //样本分类数(0-9),为10个
    int recognition_result;     //识别结果
    CvMat* trainData;           //存放训练样本的特征
    CvMat* trainClasses;        //存放训练样本的标签(分类)
    int size;                   //进行归一化的图像大小size x size
    static const int K = 5;     //K近邻算法的最大邻居个数
    CvKNearest *knn;
    void getData();             //特征提取
    void train();               //设置knn模型所需要的数据

public:
    float classify(IplImage* img, int showResult);
    int getRecognitionResult();
    void basicOCR_init();
    //basicOCR();
    void test();
};


#endif // BASIC_OCR_H
