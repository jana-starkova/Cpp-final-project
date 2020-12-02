#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include "opencv2/opencv.hpp"
#include <QMessageBox>
#include <QCloseEvent>
#include "camera.h"

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

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_btnScared_toggled(bool checked);
    void on_btnMid_toggled(bool checked);
    void on_btnBrave_toggled(bool checked);
    void on_pushButton_clicked();


private:
    Ui::Widget *ui;
    Camera* m_camera;
    QGraphicsPixmapItem pixmap;
    cv::VideoCapture video;

    bool m_btnScared;
    bool m_btnMid;
    bool m_btnBrave;
};

#endif // WIDGET_H
