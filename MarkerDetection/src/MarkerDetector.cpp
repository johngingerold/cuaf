#include "MarkerDetector.h"

MarkerDetector::MarkerDetector() {
    showVisualization = false;

    //point filter settings4
    sizeTolerance = 0.4;
    ratioTolerance = 0.4;
    differenceTolerance = 0.1;
    sizeToDistanceTolerance = 3.5;

    //thresholding settings
    openingSize = 2;

    //min and max colors in HSV
    sidesColorMin = cv::Scalar(0, 111, 163);
    sidesColorMax = cv::Scalar(15, 255, 255);
    middleColorMin = cv::Scalar(23, 0, 133);
    middleColorMax = cv::Scalar(73, 174, 255);

    //blob detector settings
    cv::SimpleBlobDetector::Params params;
    params.minDistBetweenBlobs = 10.0f;
    params.filterByInertia = false;
    params.filterByConvexity = false;
    params.filterByColor = false;
    params.filterByCircularity = false;
    params.filterByArea = true;
    params.minArea = 150;
    params.maxArea = 50000;

    blobDetector = new cv::SimpleBlobDetector(params);
}

void MarkerDetector::thresholdColor(cv::Mat &in, cv::Mat &out, cv::Scalar colorMin, cv::Scalar colorMax) {
    //thresholding
    cv::cvtColor(in, out, CV_BGR2HSV);
    cv::inRange(out, colorMin, colorMax, out);

    //remove small blobs with morphological opening filter
    cv::Mat element(openingSize*2+1, openingSize*2+1, CV_8U, cv::Scalar(1));
    cv::morphologyEx(out, out, cv::MORPH_OPEN, element);

    //close holes with morphological closeing filter
    cv::morphologyEx(out, out, cv::MORPH_CLOSE, element);
}

void MarkerDetector::detectBlobs(cv::Mat &in, std::vector<cv::KeyPoint> &keypoints) {
    blobDetector->detect(in, keypoints);
}

void MarkerDetector::visualize(cv::Mat &threshSides, std::vector<cv::KeyPoint> &keypointsSides, cv::Mat &threshMiddle, std::vector<cv::KeyPoint> &keypointsMiddle, std::vector<cv::KeyPoint> &markerKeypoints)
{
    //merge threshSides and threshMiddle images to one with threshSides in red and threshMiddle in yellow
    cv::Mat result(threshSides.rows, threshSides.cols, CV_8UC3, cv::Scalar(0,0,0));
    for(int i = 0; i < result.rows; i++) {
        for(int j = 0; j < result.cols; j++) {
            uchar* sidesVal = &threshSides.at<uchar>(i,j);
            uchar* middleVal = &threshMiddle.at<uchar>(i,j);
            cv::Vec3b* resultVals = &result.at<cv::Vec3b>(i,j);

            if(*middleVal == 255 && *sidesVal == 255) {
                *resultVals = cv::Vec3b(255, 255, 255);
                (*resultVals)[2] = 255;
            } else if(*sidesVal == 255) {
                *resultVals = cv::Vec3b(0, 0, 255);
            } else if(*middleVal == 255) {
                *resultVals = cv::Vec3b(0, 255, 255);
            }
        }
    }

    //draw all found blob keypoints
    cv::drawKeypoints(result, keypointsSides, result, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::drawKeypoints(result, keypointsMiddle, result, cv::Scalar(255, 0, 0), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    //cv::drawKeypoints(result, markerKeypoints, result, Scalar(255, 255, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    //connect all found marker keypoints with different color lines (max 3 markers)
    cv::Scalar colors[] = {cv::Scalar(255,0,0), cv::Scalar(235,255,0), cv::Scalar(255,0,255)};
    for(int i = 0; i < markerKeypoints.size()/3 && i < 3; i++) {
        line(result, markerKeypoints[3*i+0].pt, markerKeypoints[3*i+2].pt, colors[i], 4);
        line(result, markerKeypoints[3*i+1].pt, markerKeypoints[3*i+2].pt, colors[i], 4);
    }

    cv::imshow("Marker Detector", result);
}


void MarkerDetector::filterMarkerKeypoints(std::vector<cv::KeyPoint> &keypointsSides, std::vector<cv::KeyPoint> &keypointsMiddle, std::vector<cv::KeyPoint> &keypointsFiltered)
{
    int s_count = keypointsSides.size();
    int m_count = keypointsMiddle.size();

    //for each side point pair
    for(int i = 0; i < s_count - 1; i++) {
        cv::KeyPoint* s1 = &keypointsSides[i]; //first side point
        for(int j = i+1; j < s_count; j++) {
            cv::KeyPoint* s2 = &keypointsSides[j]; //second side point

            if(abs(s1->size/s2->size-1) > sizeTolerance) continue;

            float D = sqrt(pow(s1->pt.x - s2->pt.x, 2) + pow(s1->pt.y - s2->pt.y, 2)); //side to side distance
            for(int k = 0; k < m_count; k++) {
                cv::KeyPoint* m = &keypointsMiddle[k]; //middle point

                float d1 = sqrt(pow(s1->pt.x - m->pt.x, 2) + pow(s1->pt.y - m->pt.y, 2)); //one side to middle distance
                float d2 = sqrt(pow(s2->pt.x - m->pt.x, 2) + pow(s2->pt.y - m->pt.y, 2)); //onother side to middle distance

                //check if d1 and d2 are ~ similar
                if(abs(d1/d2 - 1) > ratioTolerance) continue;
                if(abs(d1+d2-D)/D > differenceTolerance) continue;

                //check if all keypoints have same sizes
                if(abs(m->size/s1->size-1) > sizeTolerance) continue;
                if(abs(m->size/s2->size-1) > sizeTolerance) continue;

                //check distance between keypoints relative to their sizes (check if area between them is not empty)
                if(d1/m->size > sizeToDistanceTolerance) continue;
                if(d2/m->size > sizeToDistanceTolerance) continue;

                //marker points found
                keypointsFiltered.push_back(*s1);
                keypointsFiltered.push_back(*s2);
                keypointsFiltered.push_back(*m);
            }
        }
    }
}

bool MarkerDetector::process(cv::Mat &in, int &x, int &y)
{
    cv::Mat threshSides, threshMiddle;
    thresholdColor(in, threshSides, sidesColorMin, sidesColorMax);
    thresholdColor(in, threshMiddle, middleColorMin, middleColorMax);

    std::vector<cv::KeyPoint> keypointsSides, keypointsMiddle;
    detectBlobs(threshSides, keypointsSides);
    detectBlobs(threshMiddle, keypointsMiddle);

    std::vector<cv::KeyPoint> markerKeypoints;
    filterMarkerKeypoints(keypointsSides, keypointsMiddle, markerKeypoints);

    if(showVisualization)
        visualize(threshSides, keypointsSides, threshMiddle, keypointsMiddle, markerKeypoints);

    if(markerKeypoints.size() != 3) //only one marker found
        return false;

    x = markerKeypoints[2].pt.x - in.cols/2;
    y = in.rows/2 - markerKeypoints[2].pt.y;
}


MarkerDetector::~MarkerDetector() {
    delete blobDetector;
}
