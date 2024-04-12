#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

int main(){
    //Finds the image that will be processed
    cv::VideoCapture cap(0);
    if (!cap.isOpened()){
        std::cout << "error";
        return 0;
    }
    cv::Mat frame_down;
    cap >> frame_down;
    if (frame_down.empty()){
        std::cout << "frame empty";
        return 0;
    }

    //Greyscales the image
    cv::Mat gray;
    cv::cvtColor(frame_down, gray, cv::COLOR_BGR2GRAY);

    //Turns the image into a binary black and white photo
    cv::Mat bw;
    cv::threshold(gray, bw, 45, 255, cv::THRESH_BINARY_INV);
    cv:: Mat eroded;
    cv::Mat morphKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::erode(bw, eroded, morphKernel);

    //
    cv::Mat temp;
    int numLables = cv::connectedComponents(bw, temp);
    cv::Mat output = cv::Mat::zeros(bw.size(), CV_8UC3);
    int blob = 0;

    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(eroded, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    cv::drawContours(frame_down, contours, -1, cv::Scalar(255, 0, 0));
    cv::drawContours(eroded, contours, -1, cv::Scalar(255, 0, 0));


    //for loop that finds the moments of the blobs
    for (auto contour : contours) {
        

        cv::Moments moments = cv::moments(contour);
        double I = moments.m00;
        
        double m10 = moments.m10;
        double m01 = moments.m01;
        double cx = m10 / I;
        double cy = m01 / I;
        double m11 = moments.m11;
        

        double ax = moments.m20;
        double ay = moments.m02;
        
        

        double theta = 0.5 * std::atan2((2*((I*m11) - (m10 * m01))),(((I*ax)- std::pow(m10,2))-((I*ay)-std::pow(m01,2))));
        if (theta == 0){
            continue;
        }
    
        blob++;
        std:: cout << "(" <<cx <<" , "<< cy <<")\n";
        std::cout << "Theta: "<< theta*(180/(std::atan(1)*4)) <<"\n";

        cv::line(frame_down, cv::Point(cx - 15, cy + 15), cv::Point(cx + 15, cy - 15), cv::Scalar(0,255,255), 2);
        cv::line(frame_down, cv::Point(cx + 15, cy + 15), cv::Point(cx - 15, cy - 15), cv::Scalar(0,255,255), 2);

        cv::line(frame_down, cv::Point(cx + 50, cy + theta*50), cv::Point(cx - 50, cy - theta * 50), cv::Scalar(255,255,0), 2);

        double min = std::numeric_limits<double>::max();
        int best_match = -1;
    }

    //displays number of blobs
    std::cout << "blobs: "<< blob<<"\n";
    //displays the imgs
    cv::imshow("original", frame_down);
    cv::waitKey(0);
    
    //ends program
    cv::destroyAllWindows();

    return 0;
}