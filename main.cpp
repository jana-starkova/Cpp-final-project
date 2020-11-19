#include <iostream>
#include "widget.h"
#include "opencv2/opencv.hpp"
#include <QApplication>

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();

    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    for(;;)
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera
        imshow("Scary Camera", frame);
        if(cv::waitKey(30) >= 0) break; // waits for a key event for a "delay" of 30ms
    }

    // the camera will be deinitialized automatically in VideoCapture destructor

    return a.exec();
}
