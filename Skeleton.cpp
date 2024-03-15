#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES



/*

Sample program for capturing and displaying a frame, and printing the pixel
values at a given position.   
To compile, copy the following lines into a file, say "compile.bat", and
save it in the directory where this file is stored. Then, run it in a Windows terminal.

  
*/

int main() {
    cv::VideoCapture cap(0); // On my laptop "0" is the built-in camera. 
    if (!cap.isOpened()) {
        std::cerr << "Error opening the camera!" << std::endl;
        return -1;
    }

    // cv::Mat frame;
    // cap >> frame;
    // cv::Mat frame = imread("./assets/DEMO_circle_fish_star_01.jpg",cv::IMREAD_COLOR);
    // cv::Mat frame_down;
    // cv::resize(frame,frame_down,cv::Size(),0.5,0.5);
    cv::Mat frame = imread("assets/DEMO_components_02.png",cv::IMREAD_COLOR);
    cv::Mat frame_down;
    cv::resize(frame,frame_down,cv::Size(),0.2,0.2);

    if (frame.empty()) {
        std::cerr << "No frame captured?" << std::endl;
        return -1;
    }
    cv::Mat gray;

    cv::cvtColor(frame_down, gray, cv::COLOR_BGR2GRAY);

    cv::Mat bw;

    cv::threshold(gray,bw,180,255,cv::THRESH_BINARY_INV);
    cv::Mat blured;
    cv::GaussianBlur(bw,blured,cv::Size(5,5),0);

    cv::Mat temp;
    int numLables = cv::connectedComponents(blured,temp);
    cv::Mat output = cv::Mat::zeros(blured.size(),CV_8UC3);
    int blob = 0;

    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(blured,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);

    cv::drawContours(output,contours,-1,cv::Scalar(255,0,0));


    for (auto contour : contours) {
        blob++;

        cv::Moments moments = cv::moments(contour);
        double I = moments.m00;
        std:: cout << "" << I<<"\n";
        
        double m10 = moments.m10;
        double m01 = moments.m01;
        double cx = m10 / I;
        double cy = m01 / I;
        double m11 = moments.m11;
        std:: cout << "(" <<cx <<" , "<< cy <<")\n";

        double ax = moments.m20;
        double ay = moments.m02;
        

        double theta = 0.5 * std::atan2((2*((I*m11) - (m10 * m01))),(((I*ax)- std::pow(m10,2))-((I*ay)-std::pow(m01,2))));

        std::cout << "Theta: "<< theta*(180/(std::atan(1)*4)) <<"";

        cv::line(output, cv::Point(cx + 50, cy + theta*50), cv::Point(cx - 50, cy - theta * 50), (0,255,255), 2);

        cv::line(output, cv::Point(cx + 25, cy), cv::Point(cx - 25, cy), (255,255,255), 2);
        cv::line(output, cv::Point(cx, cy+25), cv::Point(cx, cy-25), (255,255,255), 2);

    }
    
    std::cout << ""<< blob<<"\n";
    
    cv::imshow("original", blured);
    cv::imshow("contours", output);

    // int width = bw.size().width;
    // int height = bw.size().height;

    // std::cout << ""<< width << " x "<<height<<"\n";

    // int count;

    // for (int i = 0; i < width; ++i){
    //     for (int j = 0; j <height; ++j){
    //         cv::Vec3b pixel = bw.at<cv::Vec3b>(i,j);
    //         if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0){
    //             ++count;
    //         }
    //     }
    // }

    
    // std::cout << ""<< count <<"";
    cv::waitKey(0);
    
    cap.release();
    cv::destroyAllWindows();
    
    return 0;
} // main()