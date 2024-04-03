#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES




int main() {
    //Finds the image that will be processed
    cv::Mat frame = imread("assets/DEMO_components_02.png", cv::IMREAD_COLOR);
    cv::Mat frame_down;
    cv::resize(frame, frame_down, cv::Size(), 0.2, 0.2);

    //Greyscales the image
    cv::Mat gray;
    cv::cvtColor(frame_down, gray, cv::COLOR_BGR2GRAY);

    //Turns the image into a binary black and white photo
    cv::Mat bw;
    cv::threshold(gray, bw, 179, 255, cv::THRESH_BINARY_INV);

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
    cv::drawContours(frame_down, contours, -1, cv::Scalar(255, 0, 0));

    /*
    // Store the first 5 detected contours as reference contours
    std::vector<std::vector<cv::Point>> referenceContours;
    for (int i = 0; i < 5 && i < contours.size(); ++i) {
        referenceContours.push_back(contours[i]);
        
    }
    */

    //for loop that finds the moments of the blobs and assigns catgegories
    for (auto contour : contours) {
        blob++;

        cv::Moments moments = cv::moments(contour);
        double I = moments.m00;

        double m10 = moments.m10;
        double m01 = moments.m01;
        double cx = m10 / I;
        double cy = m01 / I;
        double m11 = moments.m11;
        std::cout << "(" << cx << " , " << cy << ")\n";

        double ax = moments.m20;
        double ay = moments.m02;


        double theta = 0.5 * std::atan2((2 * ((I * m11) - (m10 * m01))), (((I * ax) - std::pow(m10, 2)) - ((I * ay) - std::pow(m01, 2))));

        std::cout << "Theta: " << theta * (180 / (std::atan(1) * 4)) << "\n";

        cv::line(frame_down, cv::Point(cx - 15, cy + 15), cv::Point(cx + 15, cy - 15), cv::Scalar(0, 255, 255), 2);
        cv::line(frame_down, cv::Point(cx + 15, cy + 15), cv::Point(cx - 15, cy - 15), cv::Scalar(0, 255, 255), 2);

        cv::line(frame_down, cv::Point(cx + 50, cy + theta * 50), cv::Point(cx - 50, cy - theta * 50), cv::Scalar(255, 255, 0), 2);

        //double min = std::numeric_limits<double>::max();
        int best_match = -1;

        
        //area categories
        const double areaThresholds[] = { 500, 6000, 10000, 10600, 11000, 12000 }; // Adjust as needed

        // Define area thresholds for categorization
        const char categoryLabels[] = { 'A', 'B', 'C', 'D', 'E', 'F' }; // Labels for categories
        //const char newCategoryLabels[] = { 'U', 'V', 'W', 'X', 'Y', 'Z' }; // Labels for new categories

        // Determine category based on area
        char category = ' ';
        for (int i = 0; i < sizeof(areaThresholds) / sizeof(areaThresholds[0]); ++i) {
            if (I < areaThresholds[i]) {
                category = categoryLabels[i];
                break;
            }
            
        }

        //print out contour area and category
        std::cout << "Contour Area: " << I << " Category: " << category << std::endl;

        //print category letter on picture
        //cv::putText(frame_down, std::string(1, category), cv::Point(cx, cy), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);

        /*
        double minSimilarity = std::numeric_limits<double>::max();
        //iterate through each category

        for (int c = 0; c < referenceContours.size(); ++c) {
            char newCategoryLetter = newCategoryLabels[c]; // New category letter for contours not similar enough
            //iterate through contours inside that category
            for (int z = 0; z < xxxx.size(); ++c) {
                double similarity = cv::matchShapes(contour, referenceContours[i], cv::CONTOURS_MATCH_I1, 0);
                if (similarity > minSimilarity) {
                    category = newletter;
                }
            }

        }
        */
 

    //displays number of blobs
    std::cout << "blobs: " << blob << "\n";
    //displays the imgs
    cv::imshow("original", frame_down);
    cv::waitKey(0);

    //ends program
    cv::destroyAllWindows();

    return 0;
}