#include "widget.h"
#include "ui_widget.h"

#include <QVideoWidget>
#include <unistd.h>
#include <iostream>
#include <vector>


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

void Widget::displayScaredVid()
{
    /**
     * This function shows the video for very scared users.
     * The output - spiders walking on the user.
     *
     */

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

        // mirror the camera output
        Mat frame;
        flip(output, frame, 1);

        if(!frame.empty())
        {
            // copy the image on top of the frame
            butterflies.copyTo(frame(cv::Rect(0,0,butterflies.cols, butterflies.rows)),mask);

            QImage qimg(frame.data,
                        frame.cols,
                        frame.rows,
                        frame.step,
                        QImage::Format_RGB888);

            // add a line to the video
            /*
            for (int i=frame.rows/4; i<frame.rows/2; i++){
                qimg.setPixel(i,i, 0);
            }
            */

            pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
            ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
        }
        qApp->processEvents();
    }

}

void Widget::displayMidVid()
{
    using namespace cv;

    if(!video.open(0))
        return;

    Mat frame;
    while(video.isOpened())
    {
        video >> frame;
        if(!frame.empty())
        {

            copyMakeBorder(frame,
                           frame,
                           frame.rows/2,
                           frame.rows/2,
                           frame.cols/2,
                           frame.cols/2,
                           BORDER_REFLECT);

            QImage qimg(frame.data,
                        frame.cols,
                        frame.rows,
                        frame.step,
                        QImage::Format_RGB888);
            pixmap.setPixmap( QPixmap::fromImage(qimg) );
            ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
        }
        qApp->processEvents();
    }

}

void Widget::displayBraveVid()
{
    using namespace cv;

    if(!video.open(0))
        return;

    Mat frame;
    while(video.isOpened())
    {
        video >> frame;
        if(!frame.empty())
        {
            copyMakeBorder(frame,
                           frame,
                           frame.rows*2,
                           frame.rows*2,
                           frame.cols*2,
                           frame.cols*2,
                           BORDER_REFLECT);

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


void Widget::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Bye now!");

    if(video.isOpened())
    {
        msgBox.setInformativeText("Hope you had fun, see you soon!");
        msgBox.exec();
        video.release();
        event->accept();
    }
    else
    {
        msgBox.setInformativeText("Next time, you can try to play...");
        msgBox.exec();
        event->accept();
    }
}
