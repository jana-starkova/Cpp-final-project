#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <opencv2/opencv.hpp>
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
    /**
     * @brief Widget constructor
     * @param parent
     */
    explicit Widget(QWidget *parent = nullptr);

    /**
     * @brief Widget destructor
     */
    ~Widget();

    /**
     * @brief Shows camera output for a scared user
     * Adds flying butterflies to the foreground of the camera output
     */
    void displayScaredVid();

    /**
     * @brief Shows camera output for undecisive user
     * Puts a walking spider on the users face
     */
    void displayMidVid();

    /**
     * @brief Shows camera output for brave users
     * Segments the user using Watershed Segmenter and places
     * a ghost in the background
     * 3 layers - background, ghost, user (top)
     */
    void displayBraveVid();

    /**
     * @brief removes the background of a current gif frame
     */
    Mat thresholdGif(const Mat frame);

    /**
     * @brief updates position of the current gif frame
     * @param x_pos - int, x position
     * @param y_pos - int, y position
     * @param right - bool, is moving right
     * @param down - bool, is moving down
     * @param x_speed - int, horizontal speed
     * @param y_speed - int, vertical speed
     * @param frame - Mat, background frame
     * @param gif_frame - Mat, gif frame
     */
    void updatePosition(int& x_pos, int& y_pos,
                        bool& right, bool& down,
                        const int x_speed, const int y_speed,
                        const Mat frame, const Mat gif_frame);

protected:
    /**
     * @brief Shows pop up window when closing the widget
     * @param *event - closing event of the widget
     */
    void closeEvent(QCloseEvent *event);

private slots:
    /**
     * @brief sets the value of m_btnScared
     * @param checked (bool)
     */
    void on_btnScared_toggled(const bool checked);

    /**
     * @brief sets the value of m_btnMid
     * @param checked (bool)
     */
    void on_btnMid_toggled(const bool checked);

    /**
     * @brief sets the value of m_btnBrave
     * @param checked (bool)
     */
    void on_btnBrave_toggled(const bool checked);

    /**
     * @brief Open camera with user-preferred effects
     */
    void on_pushButton_clicked();

private:
    // widget user interface
    Ui::Widget *ui;
    // captures the frames through the webcam
    cv::VideoCapture m_video;
    // m_pixmap is used to display the video frames onto the widget
    QGraphicsPixmapItem m_pixmap;

    // toggle buttons in the ui, used to select the level of creepiness
    bool m_btnScared;
    bool m_btnMid;
    bool m_btnBrave;
};

#endif // WIDGET_H
