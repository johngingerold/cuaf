#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "MarkerDetector.h"


using namespace std;
using namespace cv;

int value;

void onChangeSizeTolerance( int val, void* object)
{
    MarkerDetector* obj = (MarkerDetector*) object;
    obj->sizeTolerance = val / 100.0;
}

void onChangeRatioTolerance( int val, void* object)
{
    MarkerDetector* obj = (MarkerDetector*) object;
    obj->ratioTolerance = val / 100.0;
}

void onChangeDiffTolerance( int val, void* object)
{
   MarkerDetector* obj = (MarkerDetector*) object;
   obj->differenceTolerance = val / 100.0;
}

void onChangeSizeToDistTolerance( int val, void* object)
{
   MarkerDetector* obj = (MarkerDetector*) object;
   obj->sizeToDistanceTolerance = val / 100.0;
}

void onChangeColorH( int val, void* object)
{
   cv::Scalar* obj = (cv::Scalar*) object;
   (*obj)[0] = val;
}

void onChangeColorS( int val, void* object)
{
   cv::Scalar* obj = (cv::Scalar*) object;
   (*obj)[1] = val;
}

void onChangeColorV( int val, void* object)
{
    cv::Scalar* obj = (cv::Scalar*) object;
    (*obj)[2] = val;
}

void createColorSelectors(char* name, char* window, cv::Scalar &color)
{
    std::string hName = std::string(name) + " H";
    cv::createTrackbar(hName, window, &value, 180, onChangeColorH, &color);
    setTrackbarPos(hName, window, color[0]);

    std::string sName = std::string(name) + " S";
    cv::createTrackbar(sName, window, &value, 255, onChangeColorS, &color);
    setTrackbarPos(sName, window, color[1]);

    std::string vName = std::string(name) + " V";
    cv::createTrackbar(vName, window, &value, 255, onChangeColorV, &color);
    setTrackbarPos(vName, window, color[2]);
}

int main(int argc, char ** argv)
{
    VideoCapture capture = VideoCapture(0);
    if (!capture.isOpened()) {
        cout << "Could not open video capture." << endl;
        return 1;
    }

    MarkerDetector markerDetector;
    markerDetector.showVisualization = true;
    cv::namedWindow("Marker Detector");
    cv::createTrackbar( "Size Tolerance", "Marker Detector", &value, 100, onChangeSizeTolerance, &markerDetector);
    cv::createTrackbar( "Ratio Tolerance", "Marker Detector", &value, 100, onChangeRatioTolerance, &markerDetector);
    cv::createTrackbar( "Diff Tolerance", "Marker Detector", &value, 100, onChangeDiffTolerance, &markerDetector);
    cv::createTrackbar( "sToDif Tolerance", "Marker Detector", &value, 600, onChangeSizeToDistTolerance, &markerDetector);
    setTrackbarPos("Size Tolerance", "Marker Detector", markerDetector.sizeTolerance*100);
    setTrackbarPos("Ratio Tolerance", "Marker Detector", markerDetector.ratioTolerance*100);
    setTrackbarPos("Diff Tolerance", "Marker Detector", markerDetector.differenceTolerance*100);
    setTrackbarPos("sToDif Tolerance", "Marker Detector", markerDetector.sizeToDistanceTolerance*100);

    cv::namedWindow("Color Settings");
    createColorSelectors("SIDE MIN", "Color Settings", markerDetector.sidesColorMin);
    createColorSelectors("SIDE MAX", "Color Settings", markerDetector.sidesColorMax);
    createColorSelectors("MIDD MIN", "Color Settings", markerDetector.middleColorMin);
    createColorSelectors("MIDD MAX", "Color Settings", markerDetector.middleColorMax);

    double rate = 30;//capture.get(CV_CAP_PROP_FPS);
    bool stop = false;
    // Delay between each frame in ms corresponds to video frame rate
    int delay= 1000/rate;

    cv::Mat frame; // current video frame
    // for all frames in video
    while (!stop) {
        if (!capture.read(frame))
            break;
        cv::imshow("Camera Frame", frame);

        int displacementX, displacementY;
        if( markerDetector.process(frame, displacementX, displacementY) )
            cout << displacementX << " " << displacementY << endl;

        if (cv::waitKey(delay)>=0)
            stop= true;
    }
    capture.release();

    return 0;
}
