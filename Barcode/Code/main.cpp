#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/barcode.hpp>
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
        
        

        cv::imshow("webcam",frame);


        if (cv::waitKey(1000/fps)>= 0){
            cv::destroyAllWindows();
            cap.release();
            return frame;
        }
    }

    return frame;
}

cv::Mat decoder(cv::Mat capture){
    if (capture.empty()){
        std::cout << "No capture given";
        return capture;
    }
    cv::Mat gray;

    cv::cvtColor(capture,gray,cv::COLOR_BGR2GRAY);

    std::vector<cv::Point> corners;
    std::vector<std::string> decode_info;
    std::vector<std::string> decode_type;

    cv::Ptr ba = cv::makePtr<cv::barcode::BarcodeDetector>();
   
    ba -> detectAndDecodeWithType(gray,decode_info,decode_type,corners);

    for (size_t i = 0; i < corners.size(); i += 4){
        const size_t idx = i/4;
        const bool isdec = idx < decode_info.size()
        && idx < decode_type.size()
        && !decode_type[idx].empty();
        const cv::Scalar lineColor = isdec ? cv::Scalar(0,255,0) : cv::Scalar(255,0,0);
        
        std::vector<cv::Point> contour(corners.begin()+i,corners.begin() + i + 4);
        const std::vector<std::vector<cv::Point>> contours{contour};
        cv::drawContours(capture, contours,0,lineColor,1);

        for (size_t j = 0; j < 4; j++){
            cv::circle(capture,contour[j], 2, cv::Scalar(0,0,0),-1);
        }

        if (isdec){
            std::ostringstream buf;
            buf << "[" << decode_type[idx] << "]"<< decode_info[idx];
            cv::putText(capture,buf.str(),contour[1],cv::FONT_HERSHEY_COMPLEX,0.8,cv::Scalar(0,0,255),2);
        }
    }
    return capture;
}


int main(){
    cv::Mat capture;

    capture = videocap(20);

    cv::Mat detect;

    detect = decoder(capture);

    cv::imshow("capture",detect);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}