#ifndef WATERSHEDSEGMENTER_H
#define WATERSHEDSEGMENTER_H

#include <opencv2/opencv.hpp>

class WatershedSegmenter{
private:
    cv::Mat m_markers;
public:
    void setMarkers(cv::Mat& markerImage)
    {
        markerImage.convertTo(m_markers, CV_32S);
    }

    cv::Mat process(cv::Mat &image)
    {
        cv::watershed(image, m_markers);
        m_markers.convertTo(m_markers,CV_8U);
        return m_markers;
    }
};

#endif // WATERSHEDSEGMENTER_H
