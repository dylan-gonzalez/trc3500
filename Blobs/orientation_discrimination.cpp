#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES


/*
To perform orientation discrimination:

    1. take picture of orignal image
    2. take picture of rotated image
    3. compare similarity, record similarity index
    4. repeat for other rotated images 

*/



// Function to calculate similarity index between two contours
double calculateSimilarityIndex(const std::vector<cv::Point>& contour1, const std::vector<cv::Point>& contour2) {
    // Compute similarity index between contours
    double intersectionArea = 0.0;
    for (const cv::Point& pt1 : contour1) {
        for (const cv::Point& pt2 : contour2) {
            if (pt1 == pt2) {
                intersectionArea++;
                break;
            }
        }
    }
    double similarityIndex = intersectionArea / std::min(contour1.size(), contour2.size());
    return similarityIndex;
}


int main() {
    //Finds the image that will be processed
    // First Camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "Error opening first camera" << std::endl;
        return -1;
    }
    // Process the first camera's frame
    cv::Mat frame_down;
    cap >> frame_down;
    if (frame_down.empty()) {
        std::cout << "Empty frame from first camera" << std::endl;
        return -1;
    }
    // Close the first camera
    cap.release();

    // Second Camera
    cv::VideoCapture cap2(1); // Open the second camera (adjust the index if needed)
    if (!cap2.isOpened()) {
        std::cout << "Error opening second camera" << std::endl;
        return -1;
    }

    // Process the second camera's frame
    cv::Mat frame_down2;
    cap2 >> frame_down2;
    if (frame_down2.empty()) {
        std::cout << "Empty frame from second camera" << std::endl;
        return -1;
    }

    // Close the second camera
    cap2.release();




    //Greyscales the image
    cv::Mat gray;
    cv::cvtColor(frame_down, gray, cv::COLOR_BGR2GRAY);

    //Turns the image into a binary black and white photo
    cv::Mat bw;
    cv::threshold(gray, bw, 80, 255, cv::THRESH_BINARY_INV);

    //Blurs the background to reduce unwanted blob detections
    cv::Mat blured;
    cv::GaussianBlur(bw, blured, cv::Size(5, 5), 0);

    //creates a matrix to draw our contours on
    cv::Mat temp;
    int numLables = cv::connectedComponents(blured, temp);
    cv::Mat output = cv::Mat::zeros(blured.size(), CV_8UC3);
    int blob = 0;
    std::vector<std::vector<cv::Point>> contours1;
    cv::findContours(blured, contours1, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //draws the contours onto the images
    cv::drawContours(frame_down, contours1, -1, cv::Scalar(255, 0, 0));
    cv::imshow("original", frame_down);

    // Get the first contour
    std::vector<cv::Point> originalContour = contours1[0];





    //SECOND image
    // want to save this first result - then take picture of another image - and then perform similarity comparison

    //Greyscales the image
    cv::Mat gray2;
    cv::cvtColor(frame_down2, gray2, cv::COLOR_BGR2GRAY);

    //Turns the image into a binary black and white photo
    cv::Mat bw2;
    cv::threshold(gray2, bw2, 80, 255, cv::THRESH_BINARY_INV);

    //Blurs the background to reduce unwanted blob detections
    cv::Mat blured2;
    cv::GaussianBlur(bw2, blured2, cv::Size(5, 5), 0);

    //creates a matrix to draw our contours on
    cv::Mat temp2;
    int numLables2 = cv::connectedComponents(blured2, temp2);
    cv::Mat output2 = cv::Mat::zeros(blured2.size(), CV_8UC3);
    int blob2 = 0;
    std::vector<std::vector<cv::Point>> contours2;
    cv::findContours(blured2, contours2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //draws the contours onto the images
    cv::drawContours(frame_down2, contours2, -1, cv::Scalar(255, 0, 0));
    cv::imshow("rotated", frame_down2);

    // Get the second contour
    std::vector<cv::Point> rotatedContour = contours2[0];







    // compare 2 images
    double similarityIndex = calculateSimilarityIndex(originalContour, rotatedContour);
   
    // Display similarity indices
    std::cout << "Similarity Index: " << similarityIndex << std::endl;

    //ends program
    cv::destroyAllWindows();

    return 0;
}