#-------------------------------------------------
#
# Project created by QtCreator 2017-06-04T15:08:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Handwritten_Digit_Recognition
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp \
    basic_ocr.cpp \
    preprocessing.cpp

HEADERS  += \
    basic_ocr.h \
    preprocessing.h

FORMS    +=

INCLUDEPATH += D:\opencv\build\include\opencv \
                    D:\opencv\build\include\opencv2 \
                    D:\opencv\build\include

unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_core2410d

unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_calib3d2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_contrib2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_features2d2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_flann2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_gpu2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_highgui2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_imgproc2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_legacy2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_ml2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_nonfree2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_objdetect2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_ocl2410d

unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_photo2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_stitching2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_superres2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_ts2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_video2410d
unix|win32: LIBS += -LD:/opencv/build/x86/vc12/lib/ -lopencv_videostab2410d

INCLUDEPATH += D:/opencv/build/x86/vc12
DEPENDPATH += D:/opencv/build/x86/vc12
