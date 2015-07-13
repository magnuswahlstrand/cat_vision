/*
 * hough_circles.cpp
 *
 *  Created on: Jul 13, 2015
 *      Author: magnus
 */


#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;


int hough_circles() {

	Mat img, img_gray;

	img = imread("img/plates.jpg");

	cvtColor(img, img_gray, CV_BGR2GRAY);

	//Reduce the noise to avoid false circles
	GaussianBlur(img_gray, img_gray, Size(9,9), 2, 2);

	vector<Vec3f> circles;

	//Minimum distance between detected centers
	int minDist = img.rows/8;
	int minRadius = 0; //Unknown
	int maxRadius = 0; //Unknown

	//Apply the Hough transform to the image
	HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1, minDist, 200, 100, minRadius, maxRadius);

	for(size_t i = 0; i < circles.size(); i++)
	{
		Point center( cvRound(circles[i][0]) , cvRound(circles[i][1]));

		int radius = cvRound(circles[i][2]);

		circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);

		circle(img, center, radius, Scalar(0, 0, 255), 3, 8, 0);

	}

	imshow("YAY", img);
	waitKey(0);

	return 0;
}
