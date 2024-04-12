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
    cv::threshold(gray,bw,160,255,cv::THRESH_BINARY_INV);

    //Blurs the background to reduce unwanted blob detections
    cv::Mat blured;
    cv::GaussianBlur(bw,blured,cv::Size(9,9),0);

    //creates a matrix to draw our contours on
    cv::Mat temp;
    int numLables = cv::connectedComponents(blured,temp);
    cv::Mat output = cv::Mat::zeros(blured.size(),CV_8UC3);
    int blob = 0;
    int R_count = 0;
    int L_count = 0;
    int T_count = 0;
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(blured,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);

    //draws the contours onto the images
    cv::drawContours(output,contours,-1,cv::Scalar(255,0,0));
    cv::drawContours(frame_down,contours,-1,cv::Scalar(255,0,0));

    //for loop that finds the moments of the blobs
    for (auto contour : contours) {
        blob++;

        cv::Moments moments = cv::moments(contour);
        double I = moments.m00;
        
        double m10 = moments.m10;
        double m01 = moments.m01;
        double cx = m10 / I;
        double cy = m01 / I;
        double m11 = moments.m11;
        std:: cout << "(" <<cx <<" , "<< cy <<")\n";

        double ax = moments.m20;
        double ay = moments.m02;
        

        double theta = 0.5 * std::atan2((2*((I*m11) - (m10 * m01))),(((I*ax)- std::pow(m10,2))-((I*ay)-std::pow(m01,2))));

        std::cout << "Theta: "<< theta*(180/(std::atan(1)*4)) <<"\n";

        cv::line(frame_down, cv::Point(cx - 15, cy + 15), cv::Point(cx + 15, cy - 15), cv::Scalar(0,255,255), 2);
        cv::line(frame_down, cv::Point(cx + 15, cy + 15), cv::Point(cx - 15, cy - 15), cv::Scalar(0,255,255), 2);

        cv::line(frame_down, cv::Point(cx + 50, cy + theta*50), cv::Point(cx - 50, cy - theta * 50), cv::Scalar(255,255,0), 2);

        double min = std::numeric_limits<double>::max();
        if (I > 250 && I < 900){
            putText(frame_down, std::string(1, 'R'), cv::Point(cx + 5, cy + 5), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 3);
            R_count++;
        }
        else if (I > 900 && I <1500){
            putText(frame_down, std::string(1, 'T'), cv::Point(cx + 5, cy + 5), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 3);
            T_count++;
        }
        else if (I > 1000 && I<10000){
            putText(frame_down, std::string(1, 'L'), cv::Point(cx + 5, cy + 5), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 3);
            L_count++;
        }
        
        int best_match = -1;
    }

    std::cout << "R Total: " << R_count<<"\n";
    std::cout << "L Total: " << L_count<<"\n";
    std::cout << "T Total: " << T_count<<"\n";

    //displays number of blobs
    std::cout << "blobs: "<< blob<<"\n";
    //displays the imgs
    cv::imshow("original", frame_down);
    cv::waitKey(0);
    
    //ends program
    cv::destroyAllWindows();

    return 0;
    
}