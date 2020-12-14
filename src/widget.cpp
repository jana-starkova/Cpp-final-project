#include "widget.h"
#include "ui_widget.h"
#include "watershedsegmenter.h"
#include <opencv2/opencv.hpp>

using namespace cv;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_btnScared{false},
    m_btnMid{false},
    m_btnBrave{false}
{
    // setup the ui
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&m_pixmap);
}


Widget::~Widget()
{
    delete ui;
}


void Widget::on_btnScared_toggled(const bool checked)
{
    m_btnScared = checked;
}


void Widget::on_btnMid_toggled(const bool checked)
{
    m_btnMid = checked;
}


void Widget::on_btnBrave_toggled(const bool checked)
{
    m_btnBrave = checked;
}


void Widget::displayScaredVid()
{
    if(!m_video.open(0))
    {
        return;
    }

    Mat output;
    Mat butterflies_frame;

    // open the butterflies gif
    VideoCapture butterflies;
    butterflies.open("butterflies.gif");

    // count frames of the butterflies gif
    int frame_counter;

    while(m_video.isOpened())
    {
        // stream the webcam output to a Mat
        m_video >> output;

        if(!output.empty())
        {
            // set the gif to an infinity loop
            if (frame_counter == butterflies.get(CAP_PROP_FRAME_COUNT))
            {
                frame_counter = 0;
                butterflies = VideoCapture("butterflies.gif");
            }
            ++frame_counter;

            // stream the butterflies gif to a Mat
            butterflies >> butterflies_frame;

            // mirror the webcam output
            Mat frame;
            flip(output, frame, 1);

            // resize the current frame of the butterflies gif
            cv::resize(butterflies_frame, butterflies_frame, cv::Size(output.cols, output.rows), 0, 0);

            // create a threshold mask of the current frame
            Mat butterflies_masked = thresholdGif(butterflies_frame);

            // copy the image on top of the webcam frame
            butterflies_frame.copyTo(frame(cv::Rect(0,0,butterflies_frame.cols, butterflies_frame.rows)), butterflies_masked);

            // convert Mat to QImage and show
            QImage qimg(frame.data,
                        frame.cols,
                        frame.rows,
                        frame.step,
                        QImage::Format_RGB888);

            m_pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
            ui->graphicsView->fitInView(&m_pixmap, Qt::KeepAspectRatio);
        }
        qApp->processEvents();
    }
}


void Widget::displayMidVid()
{
    if(!m_video.open(0))
    {
        return;
    }

    Mat output;
    Mat spider_frame;

    // open the spider gif
    VideoCapture spider;
    spider.open("spider.gif");

    int frame_counter{0};

    int spider_pos_x{20};
    int spider_pos_y{20};
    bool spider_down{true};
    bool spider_right{true};

    // loop the camera
    while(m_video.isOpened())
    {
        // stream the webcam output to a Mat
        m_video >> output;

        if(!output.empty())
        {
            // set the gif to an infinity loop
            if (frame_counter == spider.get(CAP_PROP_FRAME_COUNT))
            {
                frame_counter = 0;
                spider = VideoCapture("spider.gif");
                spider_right = !spider_right;
            }
            ++frame_counter;

            // stream the spider gif to a Mat
            spider >> spider_frame;

            // flip the current frame of the spider
            // depends on the previous direction
            if (!spider_right)
            {
                Mat flipped;
                flip(spider_frame, flipped, 1);
                spider_frame = flipped;
            }

            // create a threshold mask of the gif current frame
            Mat spider_masked = thresholdGif(spider_frame);

            // flip video
            Mat frame;
            flip(output, frame, 1);

            // update the position of the spider
            updatePosition(spider_pos_x, spider_pos_y,
                           spider_right, spider_down,
                           1, 1, frame, spider_frame);

            // copy the spider to the webcam frame
            spider_frame.copyTo(frame(cv::Rect(spider_pos_x,spider_pos_y,spider_frame.cols, spider_frame.rows)),spider_masked);

            // convert Mat to QImage and show
            QImage qimg(frame.data,
                        frame.cols,
                        frame.rows,
                        frame.step,
                        QImage::Format_RGB888);

            m_pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
            ui->graphicsView->fitInView(&m_pixmap, Qt::KeepAspectRatio);
        }
        qApp->processEvents();
    }
}


void Widget::displayBraveVid()
{
    if(!m_video.open(0))
    {
        return;
    }

    Mat output;
    Mat background;
    Mat ghost_frame;

    // open the video with ghost
    VideoCapture ghost;
    ghost.open("ghost.gif");
    int frame_counter{0};

    int ghost_pos_x{20};
    int ghost_pos_y{20};
    bool ghost_down{true};
    bool ghost_right{true};

    int global_frame_counter{0};

    while(m_video.isOpened())
    {
        // stream the webcam output to Mats
        // one holds the background, one holds the segmented person
        m_video >> output;
        m_video >> background;

        if(!output.empty())
        {
            // flip the webcam outputs
            Mat background_;
            flip(background, background_, 1);

            Mat frame;
            flip(output, frame, 1);

            // set the gif to an infinity loop
            if (frame_counter == ghost.get(CAP_PROP_FRAME_COUNT))
            {
                frame_counter = 0;
                ghost = VideoCapture("ghost.gif");
            }
            ++frame_counter;

            // stream the ghost gif to a Mat
            ghost >> ghost_frame;

            // mask the ghost to remove the background
            Mat ghost_masked = thresholdGif(ghost_frame);

            // update the position of the ghost
            updatePosition(ghost_pos_x, ghost_pos_y,
                           ghost_right, ghost_down,
                           3, 1, background_, ghost_frame);

            // appear ghost after the spooky blinks
            if (global_frame_counter > 40)
            {
                ghost_frame.copyTo(background_(cv::Rect(ghost_pos_x,ghost_pos_y,ghost_frame.cols, ghost_frame.rows)),ghost_masked);
            }

            // watershed segmenter
            cv::Mat blank(frame.size(),CV_8U,cv::Scalar(0xFF));
            cv::Mat dest;

            // create markers image
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

            // create watershed segmentation object and mask the frame
            WatershedSegmenter segmenter;
            segmenter.setMarkers(markers);
            cv::Mat wshedMask = segmenter.process(frame);
            cv::Mat mask;
            convertScaleAbs(wshedMask, mask, 1, 0);
            threshold(mask, mask, 1, 255, THRESH_BINARY);
            bitwise_and(frame, frame, dest, mask);
            dest.convertTo(dest,CV_8U);

            dest.copyTo(background_(cv::Rect(0,0,dest.cols, dest.rows)),dest);

            // spooky blinking of the screen
            if (global_frame_counter > 30 && global_frame_counter < 40)
            {
                if (global_frame_counter % 3 == 0)
                    background_ = Scalar(0,0,0);
                if (global_frame_counter % 5 == 0)
                    background_ = Scalar(255,255,255);
            }

            // convert Mat to QImage and show
            QImage qimg(background_.data,
                        background_.cols,
                        background_.rows,
                        background_.step,
                        QImage::Format_RGB888);
            m_pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
            ui->graphicsView->fitInView(&m_pixmap, Qt::KeepAspectRatio);

            ++global_frame_counter;
        }
        qApp->processEvents();
    }
}


Mat Widget::thresholdGif(const Mat frame)
{
    Mat grayscale;
    cv::cvtColor(frame, grayscale, COLOR_BGR2GRAY);
    double thresh = 180;
    double max_val = 255; //this is ignored but has to be set
    Mat frame_masked;
    threshold(grayscale,frame_masked, thresh, max_val, THRESH_TOZERO_INV);

    return frame_masked;
}


void Widget::updatePosition(int& x_pos, int& y_pos,
                            bool& right, bool& down,
                            const int x_speed, const int y_speed,
                            const Mat frame, const Mat gif_frame)
{
    if(right){
        x_pos = x_pos + x_speed;
        if (x_pos > (frame.cols - gif_frame.cols - 20)){
            right = false;
        }
    } else {
        x_pos = x_pos - x_speed;
        if (x_pos < 20){
            right = true;
        }
    }
    if(down){
        y_pos = y_pos + y_speed;
        if (y_pos > (frame.rows - 1.8*gif_frame.rows)){
            down = false;
        }
    } else {
        y_pos = y_pos - y_speed;
        if (y_pos < 20){
            down = true;
        }
    }
}


void Widget::on_pushButton_clicked()
{
    if(m_btnScared)
    {
        ui->selectCheck->setText("You're a chicken");
        displayScaredVid();
    }
    else if (m_btnMid)
    {
        ui->selectCheck->setText("And who should know?");
        displayMidVid();
    }
    else if (m_btnBrave)
    {
        ui->selectCheck->setText("A hero! Let's play!");
        displayBraveVid();
    }
    else
    {
        ui->selectCheck->setText("Please choose an option");
    }
}


void Widget::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Bye now!");
    msgBox.setStyleSheet("QLabel{min-width:250 px};");

    if(m_video.isOpened())
    {
        msgBox.setInformativeText("Hope you had fun,  see you soon!");
        msgBox.exec();
        m_video.release();
        event->accept();
    }
    else
    {
        msgBox.setInformativeText("Next time you can try to play...");
        msgBox.exec();
        event->accept();
    }
}
