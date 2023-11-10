#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Mat imgHSV, mask;
int hmin = 0, smin = 0, vmin = 116;
int hmax = 111, smax = 175, vmax = 2555;
float w = 600, h = 600;
Mat matrix, imgWarp;
int main()
{
	VideoCapture cap(1);
	Mat img;
	cap >> img;
	Point2f src[4] = { {185, 75}, {565, 35}, {220, 420}, {570, 410} };
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };
	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));
	cvtColor(imgWarp, imgHSV, COLOR_BGR2HSV);
	imshow("Image", img);
	Scalar lower(hmin, smin, vmin);
	Scalar upper(hmax, smax, vmax);
	inRange(imgHSV, lower, upper, mask);
	imshow("bineryImage", mask);
	waitKey(0);
	return 0;
}