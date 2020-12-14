#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include "opencv2/opencv.hpp"
#include <QMessageBox>
#include <QCloseEvent>


using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void displayScaredVid();
    void displayMidVid();
    void displayBraveVid();
    Mat thresholdGif(Mat frame);
    void updatePosition(int& x_pos, int& y_pos,
                        bool& right, bool& down,
                        int x_speed, int y_speed,
                        Mat frame, Mat gif_frame);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_btnScared_toggled(bool checked);
    void on_btnMid_toggled(bool checked);
    void on_btnBrave_toggled(bool checked);
    void on_pushButton_clicked();


private:
    Ui::Widget *ui;
    QGraphicsPixmapItem m_pixmap;
    cv::VideoCapture m_video;

    bool m_btnScared;
    bool m_btnMid;
    bool m_btnBrave;
};

#endif // WIDGET_H
