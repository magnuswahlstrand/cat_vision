/*
 * cat_face.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: magnus
 */

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );
Mat getResizedCat( Rect );

/** Global variables */
String face_cascade_name = "haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";


Mat cat;

void overlayImage(Mat &result, Mat &overlay, Point location) {

//	addWeighted(background, 0.5, overlay, 0.5, 0, result);

    MatIterator_<Vec3b> it, end;

    Size catSize = overlay.size();

    int offset_x = location.x-catSize.width/2;
    int offset_y = location.y-catSize.height/2;


    for(int row = 0; row+offset_y < result.rows; ++row) {
        uchar* p = result.ptr(row + offset_y);
        uchar* r = overlay.ptr(row);

        if(row > overlay.rows - 1 )
        	break;


        for(int col = offset_x; col < result.cols*3 && col-offset_x < overlay.cols*3; col += 3) {

        	int cr = *r++;
        	int cg = *r++;
        	int cb = *r++;

        	// Green screen 35, 255, 6
        	if(!((cr > 0 && cr < 70) && (cg > 150 && cg < 256) && (cb > 0 && cb < 60)))
        	{
				*(p++ + offset_x*3) = cr;
				*(p++ + offset_x*3) = cg;
				*(p++ + offset_x*3) = cb;
        	}
        	else {
            	*p++;
            	*p++;
            	*p++;
        	}
        	/*

        	*r++;
        	*r++;
        	*r++;*/

            // *p++ = *r++;  //points to each pixel B,G,R value in turn assuming a CV_8UC3 color image
        }

    }
}

int cat_face() {

    VideoCapture capture;
    Mat frame;

    if(true)
    {
    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };

    //-- 2. Read the video streams
    capture.open(-1);
    if( !capture.isOpened() ){ printf("--(!)Error loading eyes cascade\n"); return -1; };

    while (  capture.read(frame) )
    {
        if( frame.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }

        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame );

        int c = waitKey(10);
        if( (char)c == 27 ) { break; } // escape
    }
    return 0;
    }
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

    for( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        //ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        //circle( frame, center, 10, Scalar(255,0,0), -1);

        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );

        for( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            //circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }

        //Resize cat image
        cat = getResizedCat(faces[i]);

        overlayImage(frame, cat, center);
    }



    //-- Show what you got




    imshow( window_name, frame );
}

Mat getResizedCat(Rect faceRectangle) {
	Mat cat, dst;

	cat = imread("img/cat_2.jpg");
	resize(cat, dst, faceRectangle.size());

	return dst;
}
