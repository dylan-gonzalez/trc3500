#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <random>


int main() {
    // Define random seed to change the pattern
    int seedval = 55;
    std::mt19937 rng(seedval);

    // Define image size
    int width = 500;
    int height = 500;

    // Create random noise image
    cv::Mat noise(height, width, CV_8UC1);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            noise.at<uchar>(y, x) = rng() % 256;
        }
    }

    // Blur the noise image to control the size
    cv::Mat blur;
    cv::GaussianBlur(noise, blur, cv::Size(0, 0), 15, 15, cv::BORDER_DEFAULT);

    // Stretch the blurred image to full dynamic range
    cv::Mat stretch;
    cv::normalize(blur, stretch, 0, 255, cv::NORM_MINMAX, CV_8UC1);

    // Threshold stretched image to control the size
    cv::Mat thresh;
    cv::threshold(stretch, thresh, 175, 255, cv::THRESH_BINARY);

    // Apply morphology open and close to smooth out shapes
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(9, 9));
    cv::Mat result;
    cv::morphologyEx(thresh, result, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(result, result, cv::MORPH_CLOSE, kernel);

    // Save result
    cv::imwrite("random_blob.png", result);


    // Find contours in the result image
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(result.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


    // Ensure only one contour is found
    if (contours.size() < 1) {
        std::cerr << "Error: No contours found" << std::endl;
        return -1;
    }
    
    // Get the first contour
    std::vector<cv::Point> originalContour = contours[0];

    // Rotate the contour by 5 degrees until 360 degrees
    for (int angle = 5; angle <= 360; angle += 5) {
        cv::Mat rotatedResult = result.clone(); // Create a copy of the original result

        // Rotate the contour by 'angle' degrees
        cv::Point2f center(static_cast<float>(rotatedResult.cols / 2), static_cast<float>(rotatedResult.rows / 2));
        cv::Mat rotationMat = cv::getRotationMatrix2D(center, angle, 1.0);
        cv::warpAffine(rotatedResult, rotatedResult, rotationMat, rotatedResult.size());

        // Display the rotated result
        cv::imshow("Rotated Result", rotatedResult);
        cv::waitKey(100); // Adjust the wait time as needed (milliseconds)

        // Save the rotated result
        std::string filename = "random_blobs_rotated_by" + std::to_string(angle) + ".png";
        cv::imwrite(filename, rotatedResult);
    }


    // Show results
    cv::imshow("result", result);
    cv::waitKey(0);
    cv::destroyAllWindows();


    return 0;
}
