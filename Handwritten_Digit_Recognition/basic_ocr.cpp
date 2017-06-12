#include "preprocessing.h"
#include "basic_ocr.h"

//basicOCR::basicOCR()
//{
//    //sprintf(filePath, "E:/Qt_Projects/Handwritten_Digit_Recognition/OCR");
//    sprintf(filePath, "debug/OCR");
//    trainSamples = 50;
//    classes = 10;
//    size = 128;

//    trainData = cvCreateMat(trainSamples * classes, size*size, CV_32FC1);
//    trainClasses = cvCreateMat(trainSamples * classes, 1, CV_32FC1);

//    featureExtraction();

//    train();

//    test();
//}

void basicOCR::basicOCR_init()
{
    sprintf(filePath, "debug/OCR");
    trainSamples = 90;
    classes = 10;
    size = 128;

    trainData = cvCreateMat(trainSamples * classes, size*size, CV_32FC1);       //创建矩阵
    trainClasses = cvCreateMat(trainSamples * classes, 1, CV_32FC1);

    getData();

    train();

    test();
}

void basicOCR::getData()            //把预处理过后的图片转换成特征向量的数据，再把数据给knn使用
{
    IplImage* srcImage;
    IplImage  prsImage;
    CvMat row, data;
    char file[255];
    int i, j;
    for(i = 0; i < classes; i++)
    {
        for(j = 0; j < trainSamples; j++)
        {
            //加载pbm格式图像，作为训练
            if(j < 10)
            {
                sprintf(file, "%s/%d/%d0%d.pbm", filePath, i, i, j);
                qDebug()<<file;
            }
            else
            {
                sprintf(file, "%s/%d/%d%d.pbm", filePath, i, i, j);
                qDebug()<<file;
            }
            srcImage = cvLoadImage(file, 0);
            if(!srcImage)
            {
                qDebug("Error: Can not load image %s\n", file);
                exit(-1);
            }

            prsImage = preprocessing(srcImage, size, size);     //图像预处理

            cvGetRow(trainClasses, &row, (i*trainSamples+j));   //设置预处理图像对应的分类
            cvSet(&row, cvRealScalar(i));

            cvGetRow(trainData, &row, (i*trainSamples+j));     //设置数据

            IplImage* img = cvCreateImage(cvSize(size, size), IPL_DEPTH_32F, 1);

            cvConvertScale(&prsImage, img, 0.0039215, 0);       //8位图转换成32位浮点数图表示，大小归一化，dst(i)=src(i)*scale+shift

            cvGetSubRect(img, &data, cvRect(0, 0, size, size));

            CvMat row_header, *row1;

            row1 = cvReshape(&data, &row_header, 0, 1);         //转换size X size的数据矩阵到向量里面
            cvCopy(row1, &row, NULL);
        }
    }
}

void basicOCR::train()
{
    knn = new CvKNearest(trainData, trainClasses, 0, false, K);     //设置knn模型的数据及数据对应的类别,false表示用于分类，K是邻居数
}

void basicOCR::test()
{
    IplImage* srcImage;
    IplImage  prsImage;

    //CvMat row, data;
    char file[255];
    int i, j;
    int error = 0;
    int testCount = 0;

    for(i = 0; i < classes; i++)
    {
        for(j = 90; j < 100; j++)
        {
            sprintf(file, "%s/%d/%d%d.pbm", filePath, i, i, j);
            srcImage = cvLoadImage(file, 0);
            if(!srcImage)
            {
                qDebug("Error: Cant load image %s\n", file);
                exit(-1);
            }

            prsImage = preprocessing(srcImage, size, size);
            float r = classify(&prsImage, 0);
            if((int)r != i)
                error++;
            testCount++;
        }
    }
    float totalError = 100 * (float)error/(float)testCount;
    qDebug()<<"Misrecognized rate: "<<totalError;
}

float basicOCR::classify(IplImage* img, int showResult)         //showResult代表是使用测试样本进行分类还是用户数据进行分类
{
    IplImage prsImage;
    CvMat data;
    CvMat* nearest = cvCreateMat(1, K, CV_32FC1);
    float result;

    prsImage = preprocessing(img, size, size);

    IplImage* img32 = cvCreateImage(cvSize(size, size), IPL_DEPTH_32F, 1);
    cvConvertScale(&prsImage, img32, 0.0039215, 0);                     //大小归一化
    cvGetSubRect(img32, &data, cvRect(0, 0, size, size));
    CvMat row_header, *row1;
    row1 = cvReshape(&data, &row_header, 0, 1);

    result = knn->find_nearest(row1, K, 0, 0, nearest, 0);          //nearest，输出参数，返回的是指向邻居向量本身的指针，即指向的是原来的训练数据

    int accuracy = 0;
    for(int i = 0; i < K; i++)
    {
        if((nearest->data.fl[i]) == result)
            accuracy ++;
    }

    float pre = 100 * ((float)accuracy/(float)K);
    if(1 == showResult)
    {
        qDebug("|\t    %.0f    \t| \t    %.2f%%  \t| \t %d of %d \t| \n", result, pre, accuracy, K);
        //qDebug(" ------------------------------------------------------------------------\n");
        recognition_result = result;
    }
    return result;
}

int basicOCR::getRecognitionResult()
{
    return recognition_result;
}

