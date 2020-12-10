#include "widget.h"
#include "ui_widget.h"

#include <QVideoWidget>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
#include <string>


using namespace cv;
using namespace std;
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
int radius = 0;


class WatershedSegmenter{
private:
    cv::Mat markers;
public:
    void setMarkers(cv::Mat& markerImage)
    {
        markerImage.convertTo(markers, CV_32S);
    }

    cv::Mat process(cv::Mat &image)
    {
        cv::watershed(image, markers);
        markers.convertTo(markers,CV_8U);
        return markers;
    }
};

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_btnScared{false},
    m_btnMid{false},
    m_btnBrave{false}
{
    ui->setupUi(this);

    // TODO: is this okay?
    //Camera *camera = new Camera(this);
    /*
    m_camera = new Camera(this);
    m_camera->setViewfinder(ui->cameraView);
    m_camera->start();*/

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnScared_toggled(bool checked)
{
    m_btnScared = checked;
}

void Widget::on_btnMid_toggled(bool checked)
{
    m_btnMid = checked;
}

void Widget::on_btnBrave_toggled(bool checked)
{
    m_btnBrave = checked;
}

/**
 * This function shows the video for very scared users.
 * The output - spiders walking on the user.
 *
 */
void Widget::displayScaredVid()
{
    using namespace cv;

    if(!video.open(0))
        return;

    Mat output;
    Mat butterflies = imread("butterflies.png", -1);

    // remove the image background
    Mat mask;
    std::vector<Mat> rgbLayer;

    if(butterflies.channels() == 4)
    {
        split(butterflies,rgbLayer);         // seperate channels
        Mat cs[3] = { rgbLayer[0],rgbLayer[1],rgbLayer[2] };
        merge(cs,3,butterflies);  // put them together again
        mask = rgbLayer[3];       // alpha channel used as mask
    }


    while(video.isOpened())
    {
        video >> output;

        if(!output.empty())
        {
            // mirror the camera output
            Mat frame;
            flip(output, frame, 1);

            // copy the image on top of the frame
            butterflies.copyTo(frame(cv::Rect(0,0,butterflies.cols, butterflies.rows)),mask);

            QImage qimg(frame.data,
                        frame.cols,
                        frame.rows,
                        frame.step,
                        QImage::Format_RGB888);

            pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
            ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
        }
        qApp->processEvents();
    }

}

// put spiders on face
void Widget::displayMidVid()
{

    if(!video.open(0))
        return;

    Mat output;
    Mat ghost_frame;

    // open the video with ghost
    VideoCapture ghost; // = VideoCapture("ghost.gif");
    ghost.open("ghost.gif");
    int frame_counter = 0;
    int ghost_pos_x = 20;
    int ghost_pos_y = 20;
    bool ghost_down = true;
    bool ghost_right = true;

    while(video.isOpened())
    {
        video >> output;

        if(!output.empty())
        {
            // set the ghost gif to an infinity loop
            if (frame_counter == ghost.get(CAP_PROP_FRAME_COUNT)){
                frame_counter = 0;
                ghost = VideoCapture("ghost.gif");
            }
            frame_counter++;
            ghost >> ghost_frame;

            // mask the ghost to remove the background
            Mat gray_ghost;
            cv::cvtColor(ghost_frame, gray_ghost, COLOR_BGR2GRAY);
            double thresh = 254;
            double maxValue = 255; //this is ignored but has to be set
            Mat ghost_masked;
            threshold(gray_ghost,ghost_masked, thresh, maxValue, THRESH_TOZERO_INV);

            // flip video
            Mat frame;
            flip(output, frame, 1);

            // update the position of the ghost
            if(ghost_right){
                ghost_pos_x = ghost_pos_x + 2;
                if (ghost_pos_x > (frame.cols - ghost_frame.cols - 20)){
                    ghost_right = false;
                }
            } else {
                ghost_pos_x = ghost_pos_x - 2;
                if (ghost_pos_x < 20){
                    ghost_right = true;
                }
            }
            if(ghost_down){
                ghost_pos_y = ghost_pos_y + 1;
                if (ghost_pos_y > (frame.rows - 1.8*ghost_frame.rows)){
                    ghost_down = false;
                }
            } else {
                ghost_pos_y = ghost_pos_y - 1;
                if (ghost_pos_y < 20){
                    ghost_down = true;
                }
            }

            ghost_frame.copyTo(frame(cv::Rect(ghost_pos_x,ghost_pos_y,ghost_frame.cols, ghost_frame.rows)),ghost_masked);


            QImage qimg(frame.data,
                        frame.cols,
                        frame.rows,
                        frame.step,
                        QImage::Format_RGB888);


            pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );

            ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
        }
        qApp->processEvents();
    }

}

// place ghost in the background
// OPTIONAL: play creepy music
// foreground substraction inspired by https://docs.opencv.org/3.4/d1/dc5/tutorial_background_subtraction.html

void Widget::displayBraveVid()
{
    using namespace cv;

    if(!video.open(0))
        return;

    Mat output, fgMask;
    //Mat ghost = imread("ghost.gif", -1);


    // create Background Subtractor objects
    //Ptr<BackgroundSubtractor> pBackSub;
    //pBackSub = createBackgroundSubtractorMOG2(5, 0.01);
    //pBackSub = createBackgroundSubtractorKNN(10, 1);

    while(video.isOpened())
    {
        video >> output;

        if(!output.empty())
        {
            // flip video
            Mat frame;
            flip(output, frame, 1);

            // background substraction, doesn't work
            //update the background model
            //pBackSub->apply(frame, fgMask);

            //imshow("FG Mask", fgMask);

            cv::Mat blank(frame.size(),CV_8U,cv::Scalar(0xFF));
            cv::Mat dest;

            // Create markers image
            cv::Mat markers(frame.size(),CV_8U,cv::Scalar(-1));

            //top rectangle
            //markers(Rect(0,0,frame.cols, 5)) = Scalar::all(1);
            //bottom rectangle
            //markers(Rect(0,frame.rows-5,frame.cols, 5)) = Scalar::all(1);
            //left rectangle
            markers(Rect(0,0,5,frame.rows)) = Scalar::all(1);
            //right rectangle
            markers(Rect(frame.cols-5,0,5,frame.rows)) = Scalar::all(1);
            //centre rectangle
            int centreW = frame.cols/4;
            int centreH = frame.rows/4;
            markers(Rect((frame.cols/2)-(centreW/2),(frame.rows/2)-(centreH/2), centreW, centreH)) = Scalar::all(2);
            markers.convertTo(markers, COLOR_BGRA2GRAY);

            //Create watershed segmentation object and mask the frame
            WatershedSegmenter segmenter;
            segmenter.setMarkers(markers);
            cv::Mat wshedMask = segmenter.process(frame);
            cv::Mat mask;
            convertScaleAbs(wshedMask, mask, 1, 0);
            threshold(mask, mask, 1, 255, THRESH_BINARY);
            bitwise_and(frame, frame, dest, mask);
            dest.convertTo(dest,CV_8U);

            Mat frame2;

            // copy source, destination, mask
            //copyTo(frame2,frame,fgMask);

            /*
             * 3 layers
             * background: frame
             * middle: ghosts
             * top: dest
             */


            // convert Mat to QImage and show
            QImage qimg(dest.data,
                        dest.cols,
                        dest.rows,
                        dest.step,
                        QImage::Format_RGB888);
            pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
            ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
        }
        qApp->processEvents();
    }

}

void Widget::on_pushButton_clicked()
{
    if(m_btnScared){
        ui->selectCheck->setText("You're a chicken");
        displayScaredVid();
    } else if (m_btnMid){
        ui->selectCheck->setText("And who should know?");
        displayMidVid();
    } else if (m_btnBrave){
        ui->selectCheck->setText("A hero! Let's play!");
        displayBraveVid();
    } else {
        ui->selectCheck->setText("Please choose an option");
    }
}


/**
 * Show pop up window when closing the widget
 * @param *event - closing event of the widget
 */
void Widget::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Bye now!");
    msgBox.setStyleSheet("QLabel{min-width:250 px};");

    if(video.isOpened())
    {
        msgBox.setInformativeText("Hope you had fun,  see you soon!");
        msgBox.exec();
        video.release();
        event->accept();
    }
    else
    {
        msgBox.setInformativeText("Next time you can try to play...");
        msgBox.exec();
        event->accept();
    }
}
