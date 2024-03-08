#include <opencv2/opencv.hpp>
#include <iostream>

int main(){
    cv::Mat temp;
    temp = cv::imread("../DEMO_components_02.png",cv::IMREAD_UNCHANGED);
    cv::imshow("temp",temp);
    return 0;
}