#include <opencv2/opencv.hpp>
#include "contrast_image.h";
#include "mix_images.h";
#include "mouse_events.h";
#include "cat_face.h";
#include "find_contours.h";
#include "hough_circles.h";

using namespace std;
using namespace cv;

int main() {

	//contrast_image();
	//mix_images();

	//mouse_events();

	//cat_face();
	//find_contours();

	hough_circles();

	return 0;
}
