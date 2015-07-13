/*
 * find_contours.cpp
 *
 *  Created on: Jul 13, 2015
 *      Author: magnus
 */

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;
using namespace cv;

Mat src, src_gray, canny_output;
Mat canvas;
int thresh = 100;
int max_thresh = 255;

RNG rng(12345);

void thresh_callback(int, void*);
void imshow_matrix(Mat, int, int);

int find_contours() {

	destroyAllWindows();

	src = imread("img/cat_2.jpg");
	Size catSize = src.size();

	imshow_matrix(src, 0, 0);

	cvtColor(src, src_gray, CV_BGR2GRAY );

	createTrackbar("catbar", "window(0,0)", &thresh, max_thresh, thresh_callback);

	thresh_callback(0,0);

	waitKey(0);

	return 0;
}

void thresh_callback(int, void*) {



	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;


	Canny(src_gray, canny_output, thresh, thresh*1.5, 3 );

	/// Find contours
	findContours( canny_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Draw contours
	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	for( uint i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
	}

	/// Show in a window

	imshow_matrix(canny_output, 0, 1);
	imshow_matrix(drawing, 0, 2);


}

void imshow_matrix(Mat img, int x, int y)
{
	//Padding around images
	int padding = 30;

	//Window title

	ostringstream titleStringStream;
	titleStringStream << "window(" << x << "," << y << ")";
	string title = titleStringStream.str();

	//Image size
	Size imgSize = img.size();

	imshow(title, img);
	moveWindow(title, x*(imgSize.width+padding), y*(imgSize.height+padding));
}
