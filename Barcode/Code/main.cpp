#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

cv::Mat videocap(int fps){
    cv::VideoCapture cap(0);
    cv::Mat frame;

    if (!cap.isOpened()){
        std::cout << "Camera Cannot be Opened";
        return frame;
    }

    while (cap.read(frame)){
        
        cv::Mat gray;

        cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);

        cv::imshow("webcam",gray);


        if (cv::waitKey(1000/fps)>= 0){
            cv::destroyAllWindows();
            cap.release();
            return gray;
        }
    }

    return frame;
}
/*
cv::Mat decoder(cv::Mat capture){
    if (capture.empty()){
        std::cout << "No capture given";
        return capture;
    }

    cv::Mat corners;

}*/


int main(){
    cv::Mat capture;

    capture = videocap(20);
    cv::imshow("capture",capture);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}