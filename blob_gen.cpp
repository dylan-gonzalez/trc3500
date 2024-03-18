#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

int main(){
    //creates two blank imgs to store data for late use
    cv::Mat img = cv::Mat::zeros(cv::Size(640,400),CV_8UC3);
    cv::Mat inv_img = cv::Mat::zeros(img.size(),CV_8UC3);

    //places random shapes for the blobs to form
    cv::RNG random(std::time(NULL));

    int blobs = 6;

    int i = 0;
    while (i < blobs){
        cv::Point pt1;
        cv::Point pt2;
        
        pt1.x = random.uniform(65,575);
        pt1.y = random.uniform(65,335);
        pt2.x = random.uniform(1,640);
        pt2.y = random.uniform(1,400);
        int choice = random.uniform(0,3);
        if (choice == 0){
            int r = random.uniform(1,64);
            cv::circle(img,pt1,r,cv::Scalar(255,255,255),cv::FILLED,cv::LINE_8);
            i++;
        }
        else if(choice == 1){
            cv::line(img,pt1,pt2,cv::Scalar(255,255,255),random.uniform(4,10));
            i++;
        }
        else if(choice == 2){
            cv::rectangle(img,cv::Rect(pt1,pt2),cv::Scalar(255,255,255),cv::FILLED);
            i++;
        }
    }

    //Inverts the colour of the img
    cv::bitwise_not(img,inv_img);

    //displays the images created
    cv::imshow("blobs",inv_img);
    
    //Closes windows on key press
    cv::waitKey(0);

    img.release();
    cv::destroyAllWindows();
    return 0;
}