#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/ml.h>
#include <stdio.h>
#include <ctype.h>
#include <QDebug>

IplImage preprocessing(IplImage* imgSrc,int new_width, int new_height);

#endif // PREPROCESSING_H
