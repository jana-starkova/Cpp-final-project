#include <iostream>
#include "widget.h"
#include "opencv2/opencv.hpp"
#include <QApplication>
#include <QtMultimedia/QMediaPlaylist>
#include <QtMultimedia/QMediaPlayer>
#include <QVideoWidget>
#include <QtMultimedia/QCamera>
#include <QMainWindow>

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.show();

    return a.exec();
}

