#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

cv::Mat videocap(int fps) {
    cv::VideoCapture cap(0);
    cv::Mat frame;

    if (!cap.isOpened()) {
        std::cout << "Camera Cannot be Opened";
        return frame;
    }

    while (cap.read(frame)) {

        cv::Mat gray;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        cv::imshow("webcam", gray);


        if (cv::waitKey(1000 / fps) >= 0) {
            cv::destroyAllWindows();
            cap.release();
            return gray;
        }
    }


    return frame;
}
/*
cv::Mat decoder(cv::Mat capture){
    if (capture.empty()){
        std::cout << "No capture given";
        return capture;
    }

    cv::Mat corners;

}*/

cv::Mat read_im() {

    //read image
    cv::Mat im = imread("IMG_20240227_0003.jpg", cv::IMREAD_COLOR);
    cv::Mat im_resized;
    cv::resize(im, im_resized, cv::Size(), 2, 2);

    //grayscale image
    cv::Mat gray;
    cv::cvtColor(im_resized, gray, cv::COLOR_BGR2GRAY);

    return gray;
}


cv::Mat bounding_box(cv::Mat bw, std::vector<std::vector<cv::Point>>  contours, cv::Mat output) {
    // Find contours in the thresholded image
    /*std::vector<std::vector<cv::Point>> contours;
    cv::findContours(barcode, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);*/

    // Find the contour with the largest area (assuming it's the barcode)
    double maxArea = 0;
    int maxAreaIdx = -1;
    for (size_t i = 0; i < contours.size(); ++i) {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea) {
            maxArea = area;
            maxAreaIdx = i;
        }
    }
    
    // Find the bounding box of the largest contour
    if (maxAreaIdx != -1) {
        cv::Rect boundingBox = cv::boundingRect(contours[maxAreaIdx]);

        // Crop the original image to the bounding box
        cv::Mat roi = bw(boundingBox).clone(); // Ensure a deep copy is made to avoid memory corruption

        // Now 'roi' contains the cropped region of the original image that corresponds to the barcode
        cv::imshow("cropped to barcode size", roi);
        cv::waitKey(0);
        return roi;
    }

}



cv::Mat crop_barcode(cv::Mat bw) {

    //find contours again
    //creates a matrix to draw our contours on
    cv::Mat temp;
    int numLables = cv::connectedComponents(bw, temp);
    cv::Mat output_with_contours = cv::Mat::zeros(bw.size(), CV_8UC3);
    int blob = 0;
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(bw, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //draws the contours onto the images
    cv::drawContours(output_with_contours, contours, -1, cv::Scalar(255, 0, 0));
    cv::imshow("contours cropped to barcode size", output_with_contours);
    cv::waitKey(0);

    // Find the bounding boxes of all contours
    std::vector<cv::Rect> boundingBoxes;//
    for (size_t i = 0; i < contours.size(); ++i) {//
        cv::Rect boundingBox = cv::boundingRect(contours[i]);//
        boundingBoxes.push_back(boundingBox);//
    }
    
    // Sort bounding boxes based on their x-coordinates
    std::sort(boundingBoxes.begin(), boundingBoxes.end(), [](const cv::Rect& bbox1, const cv::Rect& bbox2) {
        return bbox1.x < bbox2.x;
        });

    //crop height
      
    // Find the indices of bounding boxes closest to the left and right edges
    int leftIdx = 0;
    int rightIdx = static_cast<int>(boundingBoxes.size()) - 1;

    // Crop the image to the region between the bounding boxes closest to the left and right edges
    int x = boundingBoxes[leftIdx].x; // Left edge
    int width = boundingBoxes[rightIdx].x + boundingBoxes[rightIdx].width - x; // Width
    int y = 0; // Top edge
    int height = output_with_contours.rows; // Height

    // Calculate the new height to get the middle third
    int newHeight = height / 3;

    cv::Mat cropped_height;
    // Crop the height to get the remaining middle third
    cropped_height = output_with_contours(cv::Rect(0, newHeight, output_with_contours.cols, newHeight)).clone();
    cv::imshow("contours height cropped", cropped_height);
    cv::waitKey(0);

    // Crop the cropped height image to the adjusted region
    cv::Mat roi = cropped_height(cv::Rect(x, y, width, newHeight)).clone();
    cv::imshow("contours width cropped", roi);
    cv::waitKey(0);

    // Now 'roi' contains the cropped region of the original image where the contours closest to the vertical edges touch the left and right edges
    return roi;



}




cv::Mat draw_contours(cv::Mat gray) {

    //Turns the image into a binary black and white photo
    cv::Mat bw;
    
    /*
    thresholded values for camera pic - uncomment and adjust for demo
    cv::threshold(gray, bw, 95, 255, cv::THRESH_BINARY_INV);
    */

    
    //thresholded value for read_im capture
    cv::threshold(gray, bw, 30, 255, cv::THRESH_BINARY_INV); 

    /*
    // Perform Otsu's thresholding and invert the binary image
    cv::Mat thresh;
    cv::threshold(gray, thresh, 0, 255, cv::THRESH_OTSU + cv::THRESH_BINARY_INV);
    

    // Apply dilation to the thresholded image
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::Mat dilate;
    cv::dilate(thresh, dilate, kernel, cv::Point(-1, -1), 3);
    cv::imshow("dilate", dilate);

    */

    //creates a matrix to draw our contours on
    cv::Mat temp;
    int numLables = cv::connectedComponents(bw, temp);
    cv::Mat output_with_contours = cv::Mat::zeros(bw.size(), CV_8UC3);
    int blob = 0;
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(bw, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //draws the contours onto the images
    cv::drawContours(output_with_contours, contours, -1, cv::Scalar(255, 0, 0));


    //find initial bounding box
    //blur barcode a lot
    cv::Mat blured;
    cv::GaussianBlur(bw, blured, cv::Size(51,51), 0); 

    //creates a matrix to draw our contours on
    cv::Mat temp2;
    int numLables2 = cv::connectedComponents(bw, temp);
    cv::Mat barcode_blob = cv::Mat::zeros(blured.size(), CV_8UC3);
    int blob2 = 0;
    std::vector<std::vector<cv::Point>> contours2;
    cv::findContours(blured, contours2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    //draws the contours onto the images
    cv::drawContours(barcode_blob, contours2, -1, cv::Scalar(255, 0, 0));

    cv::Mat bounded_output = bounding_box(bw, contours2, output_with_contours);

    return bounded_output;

}



int main() {
    cv::Mat capture;

    //capture image with camera - uncomment for demo
    //capture = videocap(20);
    
    //OR read image - for testing 
    capture = read_im();

    //show image
    //cv::imshow("capture", capture);

    //draw contours
    cv::Mat bounded_output;
    bounded_output = draw_contours(capture);
    //cv::imshow("contours cropped to barcode size", bounded_output);
    //cv::waitKey(0);

    //crop further
    cv::Mat cropped_contours;
    cropped_contours = crop_barcode(bounded_output);
    cv::imshow("cropped further", cropped_contours);
    cv::waitKey(0);
    



    //cv::Mat bbox;
    //bbox = bounding_box(contours);
    //cv::imshow("region of interest", bbox);
    

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}