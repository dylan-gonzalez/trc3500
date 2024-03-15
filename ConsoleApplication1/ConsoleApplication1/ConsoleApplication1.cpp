#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


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
    // cv::Mat frame = imread("../DEMO_circle_fish_star_01.jpg",cv::IMREAD_COLOR);
    // cv::Mat frame_down;
    // cv::resize(frame,frame_down,cv::Size(),0.5,0.5);
    cv::Mat frame = imread("../DEMO_components_02.png",cv::IMREAD_COLOR);
    cv::Mat frame_down;
    cv::resize(frame,frame_down,cv::Size(),0.2,0.2);

    if (frame.empty()) {
        std::cerr << "No frame captured?" << std::endl;
        return -1;
    }
    cv::Mat gray;

    cv::cvtColor(frame_down, gray, cv::COLOR_BGR2GRAY);

    cv::Mat bw;

    cv::threshold(gray,bw,127,255,cv::THRESH_BINARY);

    cv::Mat temp;
    int numLables = cv::connectedComponents(bw,temp);
    cv::Mat output = cv::Mat::zeros(bw.size(),CV_8UC3);

    for (int label = 1; label<numLables;++label){
        cv::Mat mask = (temp != label);

        std::vector<std::vector<cv::Point>>contours;
        cv::findContours(mask,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);
        cv::drawContours(output,contours,-1,cv::Scalar(255,0,0));
    }

    cv::imshow("original", bw);
    cv::imshow("contours", output);


    /*
    cv::Vec3b pixel = frame.at<cv::Vec3b>(y, x);
    std::cout << "Pixel values at (" << x << ", " << y << "): ";
    std::cout << "B: " << static_cast<int>(pixel[0]) << " ";
    std::cout << "G: " << static_cast<int>(pixel[1]) << " ";
    std::cout << "R: " << static_cast<int>(pixel[2]) << std::endl;
    */
    cv::waitKey(0);
    
    cap.release();
    cv::destroyAllWindows();
    
    return 0;
} // main()