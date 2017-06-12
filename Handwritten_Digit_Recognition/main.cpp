#include <QApplication>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui_c.h>
#include <basic_ocr.h>

#define True (1)
#define False (0)
#define ESC (27)

IplImage* whiteBoard;               //白板内容,在白板上面绘制(修改数据)
IplImage* screenBuffer;             //临时内容，克隆白板内容，用于显示图像
IplImage* infoWindow;
int pRed, pGreen, pBlue;
basicOCR ocr;

int isDrawing;                      //当前用户是否正在书写数字
int penWidth;                       //笔迹宽度
int lastX, lastY;                   //记录上一个点的坐标，用于绘制
int keyValue;                       //按键键值
int recNum;                         //当前识别的数字

QString strRecNum;

char ch_strOptHeader[25] = "-----------------------";
char ch_strOptReduce[25] = "Reduce penWidth : -";
char ch_strOptIncrease[25] = "Increase penWidth : +";
char ch_strOptClean[25] = "Clean whiteBoard : r";
char ch_strOptRecognize[25] = "Recognized number : c";
char ch_strOptSavePbm[25] = "Save Current image : s";
char ch_strQuit[15] = "Quit : Esc";
char ch_strOptTail[25] = "-----------------------";

void init();                                                        //初始化
void drawPoint(int x, int y);                                       //画点
void drawCursor(int x, int y);                                      //画鼠标位置的点,直接在screenBuffer上面画
void on_mouse(int event, int x, int y, int flags, void* param);     //鼠标的回调函数
void showInfo();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    init();
    showInfo();

    for(;;)
    {
        cvShowImage("whiteBoard", screenBuffer);
        keyValue = cvWaitKey(10);       //这个延时是必须的，否则无法处理按键事件
        if((char)keyValue == ESC)
        {
            break;
        }
        else if((char)keyValue == '+')
        {
            penWidth++;
            drawCursor(lastX, lastY);
        }
        else if(((char)keyValue == '-')&&(penWidth > 1))
        {
            penWidth--;
            drawCursor(lastX, lastY);
        }
        else if((char)keyValue == 'r')
        {
            cvSet(whiteBoard, cvRealScalar(255), NULL);
            drawCursor(lastX, lastY);
        }
        else if((char)keyValue == 's')
        {
            cvSaveImage("E:/out.pbm", whiteBoard);
        }
        else if((char)keyValue == 'c')
        {
            ocr.classify(whiteBoard,1);
            recNum = ocr.getRecognitionResult();
            qDebug()<<"----------------------res: "<< recNum;
            showInfo();
        }
    }

    cvDestroyAllWindows();

    return a.exec();
}

void on_mouse(int event, int x, int y, int flags, void *param)
{
    lastX = x;
    lastY = y;
    drawCursor(x, y);

    if(event == CV_EVENT_LBUTTONDOWN)
    {
        isDrawing = True;
        drawPoint(x, y);
    }
    else if(event == CV_EVENT_LBUTTONUP)
    {
        isDrawing = False;
    }
    else if(event == CV_EVENT_MOUSEMOVE && flags & CV_EVENT_FLAG_LBUTTON)   //CV_EVENT_FLAG_LBUTTON为左键拖拽，如果为左键拖拽移动的话，直接画点
    {
        if(True == isDrawing)
            drawPoint(x, y);
    }
}

void drawPoint(int x, int y)
{
    cvCircle(whiteBoard, cvPoint(x, y), penWidth, CV_RGB(pRed, pGreen, pBlue), -1, 4, 0);   //画圆,是在whiteBoard上面画的
    screenBuffer = cvCloneImage(whiteBoard);
    cvShowImage("whiteBoard", screenBuffer);
}

void drawCursor(int x, int y)
{
    screenBuffer = cvCloneImage(whiteBoard);
    cvCircle(screenBuffer, cvPoint(x, y), penWidth,CV_RGB(0, 0, 0), 1, 4, 0);
}

void init()
{
    ocr.basicOCR_init();

    isDrawing = False;
    penWidth = 8;
    pRed = pGreen = pBlue = 0;
    lastX = lastY = 0;

    whiteBoard = cvCreateImage(cvSize(128, 128), IPL_DEPTH_8U, 1);      //创建白板，大小为128x128,图像元素的位深度为无符号8位整型，每个元素(像素)的通道数为1
    cvSet(whiteBoard, CV_RGB(255, 255, 255), NULL);                     //为图像数据中的每个元素赋值,全255为白色，全0为黑色,对图像数据域的矩阵进行赋值得到一副空白图
    screenBuffer = cvCloneImage(whiteBoard);                            //把画完的whiteBoard数据写到screenBuffer上面

    infoWindow = cvCreateImage(cvSize(512,512), IPL_DEPTH_8U, 1);
    cvSet(infoWindow, CV_RGB(255, 255, 255), NULL);

    cvNamedWindow("whiteBoard", CV_WINDOW_NORMAL);                                     //创建窗体
    cvResizeWindow("whiteBoard", 512, 512);                             //大小为512x512

    cvSetMouseCallback("whiteBoard", &on_mouse, 0);                     //设置whiteBoard窗体的鼠标回调函数
    cvShowImage("whiteBoard",whiteBoard);
}

void showInfo()
{
       //CvSize size = cvSize(512, 512);                                  //定义信息显示窗体大小
       //IplImage * infoImage = cvCreateImage(size, IPL_DEPTH_8U, 3);     //创建信息显示窗体
       cvSet(infoWindow,CV_RGB(255, 255, 255), NULL);                     //设置窗体为空白

       QByteArray ba_strRecNum;
       char* ch_strRecNum;

       strRecNum = "Recognized Number : " + QString::number(recNum);      //已识别数字
       ba_strRecNum = strRecNum.toLatin1();
       ch_strRecNum = ba_strRecNum.data();

       CvScalar color = cvScalar(0, 0, 0);
       CvFont recNumFont;

       cvInitFont(&recNumFont, CV_FONT_HERSHEY_COMPLEX, 0.5, 1.0, 0);

       //信息窗体显示设置
       //cvPutText(infoWindow, ch_strRecNum, cvPoint(0,20), &recNumFont, color);
       cvPutText(infoWindow, ch_strOptHeader, cvPoint(0,20), &recNumFont, color);
       cvPutText(infoWindow, ch_strOptReduce, cvPoint(80,50), &recNumFont, color);
       cvPutText(infoWindow, ch_strOptIncrease, cvPoint(80,80), &recNumFont, color);
       cvPutText(infoWindow, ch_strOptClean, cvPoint(80,110), &recNumFont, color);
       cvPutText(infoWindow, ch_strOptRecognize, cvPoint(80,140), &recNumFont, color);
       cvPutText(infoWindow, ch_strQuit, cvPoint(80,170), &recNumFont, color);
       cvPutText(infoWindow, ch_strOptSavePbm, cvPoint(80,200), &recNumFont, color);
       cvPutText(infoWindow, ch_strOptTail, cvPoint(0,230), &recNumFont, color);
       cvPutText(infoWindow, ch_strRecNum, cvPoint(80,260), &recNumFont, color);


       cvNamedWindow("infoWindow");
       cvShowImage("infoWindow", infoWindow);/* 创建图片并显示文本 */
       cvWaitKey(0);
}
