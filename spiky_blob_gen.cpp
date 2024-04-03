#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

int main() {

    //creates two blank imgs to store data for late use
    cv::Mat img = cv::Mat::zeros(cv::Size(640, 400), CV_8UC3);
    cv::Mat inv_img = cv::Mat::zeros(img.size(), CV_8UC3);

    // Generate a random contour for the blob
    std::vector<cv::Point> contour; //vector of points for the contour

    cv::RNG rng(std::time(NULL)); //initialises instance of random number generator

    int numPoints = rng.uniform(5, 20); // generates random integer between 5 and 20

    std::cout << "Number of points: " << numPoints << std::endl; //print numPoints
    
    // for each numPoint get a random coordinate
    for (int i = 0; i < numPoints; ++i) {

        // Create a point with initial coordinates (x, y)
        int x = rng.uniform(50, img.cols); // Random x-coordinate
        int y = rng.uniform(50, img.rows); // Random y-coordinate
        cv::Point point(x, y);

        /*// Print the updated coordinates
        std::cout << " x: " << point.x << std::endl;
        std::cout << " y: " << point.y << std::endl; */

        // Add the point to the contour
        contour.push_back(cv::Point(x, y)); //adding each point to the contour

        //contours.push_back(std::vector<cv::Point>{cv::Point(x, y)});// append point to contours, which is a vector of points
    }

    // Add the contour to the contours vector
    std::vector<std::vector<cv::Point>> contours; 
    contours.push_back(contour); // 1 contour inside this contour vector
    
    // Draw the contour
    cv::drawContours(img, contours, 0, cv::Scalar(255, 255, 255), cv::FILLED); // draw the 1 contour

    //Inverts the colour of the img
    cv::bitwise_not(img, inv_img);

    //displays the images created
    cv::imshow("blobs", img);

    //Closes windows on key press
    cv::waitKey(0);

    img.release();
    cv::destroyAllWindows();

    /**/
    return 0;
}