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
    cv::Mat im = imread("../resources/IMG_20240227_0003.jpg", cv::IMREAD_COLOR);
    cv::Mat im_resized;
    cv::resize(im, im_resized, cv::Size(), 2, 2);

    //grayscale image
    cv::Mat gray;
    cv::cvtColor(im_resized, gray, cv::COLOR_BGR2GRAY);

    return gray;
}

int get_bits(std::vector<std::vector<cv::Point>> contours, cv::Mat image) {
    
    std::vector<cv::RotatedRect> minRect( contours.size() );

    // find minAreaRect for each contour (each line)
    for (size_t i = 0; i < contours.size(); i++)
    {   
        // filter unwanted objects (contours with less than 4 points, contours with too small area)
        //if (contours[i].size() < 4 || contourArea(contours[i]) < someAreaInPixels)
        //    continue;

        // you can draw contours for debugging
        // drawContours(binaryImage, contours, i, Scalar(255,0,0), 1, 8, hierarchy, 0, Point());

        minRect[i] = minAreaRect(cv::Mat(contours[i]));

        //std::cout << minRect.size << "\n";



        // now you can use minRect.size.width to determine width of the bar
        // minRect contains center point, size and angle
    }

    /// Draw contours + rotated rects + ellipses
    cv::Mat drawing = cv::Mat::zeros( image.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        std::cout << "printing" << i << "\n";

        cv::Scalar color = cv::Scalar( 0, 255, 0);
        // contour
        drawContours( drawing, contours, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
        // rotated rectangle
        cv::Point2f rect_points[4]; minRect[i].points( rect_points );
        for( int j = 0; j < 4; j++ )
            line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
    }

    cv::imshow("boxes", drawing );
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 1;

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

    get_bits(contours, output);
    return output;

}



int main() {
    cv::Mat capture;

    //capture image with camera - uncomment for demo
    //capture = videocap(20);
    
    //OR read image - for testing 
    capture = read_im();

    //show image
    cv::imshow("capture", capture);

    //draw contours
    cv::Mat contours;
    contours = draw_contours(capture);
    cv::imshow("contours", contours);
    

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}