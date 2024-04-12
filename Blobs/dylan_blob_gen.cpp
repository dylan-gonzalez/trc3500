#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <random>
#include <cmath>

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

    // Rotate the blob to a 5 degree angle
    double angle = 5.0;
    cv::Point2f center(static_cast<float>(result.cols / 2), static_cast<float>(result.rows / 2));
    cv::Mat rotationMat = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(result, result, rotationMat, result.size());

    // Save result
    cv::imwrite("random_blobs_rotated.png", result);

    // Show results
    cv::imshow("result", result);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
