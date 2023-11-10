#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Mat imgHSV, mask;
int hmin = 0, smin = 0, vmin = 116;
int hmax = 111, smax = 142, vmax = 2555;
float w = 250, h = 350;
Mat matrix, imgWarp;
int main()
{
	//string path = "C:/opencv/pictures/maze1.jpg";
	//Mat img = imread(path);
	VideoCapture cap(0);
	Mat img;
	cap >> img;
	Point2f src[4] = { {185, 75}, {565, 35}, {220, 420}, {570, 410} };
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	for (int i = 0; i < 4; i++) {
		circle(img, src[i], 10, Scalar(0, 0, 255), FILLED);
	}

	imshow("Image", img);
	//imshow("ImageWarp", imgWarp);
	cvtColor(imgWarp, imgHSV, COLOR_BGR2HSV);
	/*namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 2555);*/

	while (true) {

		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);
		imshow("bineryImage", mask);
		waitKey(1);
	}
	waitKey(0);
	return 0;
}