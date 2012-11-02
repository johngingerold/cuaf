#ifndef MARKERDETECTOR_H
#define MARKERDETECTOR_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <vector>

class MarkerDetector
{
    public:
        bool showVisualization;
        //element size for opening filter
        int openingSize;

        //point filter settings
        float sizeTolerance;
        float ratioTolerance;
        float differenceTolerance;
        float sizeToDistanceTolerance;

        //min and max color values for thresholding
        cv::Scalar sidesColorMin, sidesColorMax;
        cv::Scalar middleColorMin, middleColorMax;

        MarkerDetector();
        virtual ~MarkerDetector();

        //Gets marker coordinates relative to the center
        bool process(cv::Mat &, int &, int &);
    protected:
    private:
        cv::SimpleBlobDetector* blobDetector;

        void thresholdColor(cv::Mat&, cv::Mat &, cv::Scalar, cv::Scalar);
        void detectBlobs(cv::Mat &, std::vector<cv::KeyPoint> &);
        void visualize(cv::Mat &, std::vector<cv::KeyPoint> &, cv::Mat &, std::vector<cv::KeyPoint> &, std::vector<cv::KeyPoint> &);
        void filterMarkerKeypoints(std::vector<cv::KeyPoint> &, std::vector<cv::KeyPoint> &, std::vector<cv::KeyPoint> &);
};

#endif // MARKERDETECTOR_H
