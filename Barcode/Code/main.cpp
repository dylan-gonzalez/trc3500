#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

cv::Mat videocap(int fps) {
    cv::VideoCapture cap(0);
    cv::Mat frame;

    if (!cap.isOpened()) {
        std::cout << "Camera Cannot be Opened";
        return frame;
    }

    while (cap.read(frame)) {

        cv::Mat gray;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        cv::imshow("webcam", gray);


        if (cv::waitKey(1000 / fps) >= 0) {
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


cv::Mat draw_contours(cv::Mat gray) {


    //Turns the image into a binary black and white photo
    cv::Mat bw;
    cv::threshold(gray, bw, 95, 255, cv::THRESH_BINARY_INV);

    //Blurs the background to reduce unwanted blob detections
    cv::Mat blured;
    cv::GaussianBlur(bw, blured, cv::Size(5, 5), 0);

    //creates a matrix to draw our contours on
    cv::Mat temp;
    int numLables = cv::connectedComponents(blured, temp);
    cv::Mat output = cv::Mat::zeros(blured.size(), CV_8UC3);
    int blob = 0;
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(blured, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //draws the contours onto the images
    cv::drawContours(output, contours, -1, cv::Scalar(255, 0, 0));
    return output;

}


int main() {
    cv::Mat capture;

    capture = videocap(20);
    cv::imshow("capture", capture);

    cv::Mat contours;
    contours = draw_contours(capture);

    cv::imshow("contours", contours);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}