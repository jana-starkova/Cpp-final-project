#ifndef WATERSHEDSEGMENTER_H
#define WATERSHEDSEGMENTER_H

#include <opencv2/opencv.hpp>

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

#endif // WATERSHEDSEGMENTER_H
