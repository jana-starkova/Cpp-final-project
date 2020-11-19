#include <iostream>
#include "widget.h"
#include "opencv2/opencv.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();

    // OpenCV test
    cv::Mat Mat;

    return a.exec();
}
