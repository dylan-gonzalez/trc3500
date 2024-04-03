#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <random>



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
/*
// Function to perform orientation discrimination
std::vector<double> performOrientationDiscrimination(const std::vector<cv::Point>& originalContour, int numSteps) {
    std::vector<double> similarityIndices; // Vector to store similarity indices for each rotation angle

    for (int angle = 0; angle < 360; angle += numSteps) {
        // Rotate the original contour by 'angle' degrees
        cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point2f(static_cast<float>(originalContour.cols / 2), static_cast<float>(originalContour.rows / 2)), angle, 1.0);
        std::vector<cv::Point> rotatedContour;
        cv::transform(originalContour, rotatedContour, rotationMatrix);

        // Calculate similarity index between original contour and rotated contour
        double similarityIndex = calculateSimilarityIndex(originalContour, rotatedContour);
        similarityIndices.push_back(similarityIndex);
    }

    return similarityIndices;
}
*/

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


    /*
    // Perform orientation discrimination with 5-degree steps
    std::vector<double> similarityIndices = performOrientationDiscrimination(originalContour, 5);
    
    
    // Display similarity indices
    std::cout << "Similarity Indices:" << std::endl;
    for (int i = 0; i < similarityIndices.size(); ++i) {
        std::cout << "Angle: " << i * 5 << " degrees, Similarity Index: " << similarityIndices[i] << std::endl;
    }
    */

    // Show results
    cv::imshow("result", result);
    cv::waitKey(0);
    cv::destroyAllWindows();


    return 0;
}
