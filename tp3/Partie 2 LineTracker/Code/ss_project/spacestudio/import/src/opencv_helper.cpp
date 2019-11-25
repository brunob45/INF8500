#include "opencv_helper.h"
#include "SpaceDisplay.h"
#include <opencv2/opencv.hpp>

int get_image(char* file, unsigned char image_pixels[], unsigned int width, unsigned int height){
	cv::Mat image, gray_image;
	image = cv::imread(file, cv::IMREAD_COLOR);
	if (!image.data) {
		printf("No image data \n");
		return -1;
	}

	cv::cvtColor(image, gray_image, CV_BGR2GRAY);
	for(unsigned int i = 0; i < width*height; i++){
		image_pixels[i] = gray_image.ptr()[i];
	}
	
	cv::namedWindow("Vision");
	cv::moveWindow("Vision", 100, 100);

	cv::Mat image_gray (height, width, CV_8UC1, image_pixels);
	cv::imshow ("Vision", image_gray);
	cvWaitKey(1);
	
	return 0;
}

void display_lines(line_points* lines, unsigned int width, unsigned int height, unsigned int nb_lines){
	cv::Mat image(height, width, CV_8UC3, cv::Scalar(0));

	for(unsigned int index=0; index<nb_lines; index++) {
		line_points line = lines[index];
		cv::line(image, cv::Point(line.x0, line.y0), cv::Point(line.x1, line.y1), cv::Scalar(0,255,0), 1, CV_AA);
	}

	cv::namedWindow("Detected lines");
	cv::moveWindow("Detected lines", 500, 100);
	cv::imshow("Detected lines", image);
	cvWaitKey(1);
}