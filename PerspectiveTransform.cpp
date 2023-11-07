#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

float w = 250, h = 350;
Mat matrix, imgWarp;

int main()
{
	string path = "C:/test.jpg"; //Please paste the path of the picture here.
	Mat img = imread(path);

	Point2f src[4] = { {305, 105}, {1000, 50}, {360, 740}, {1000, 725} }; //You can adjust the parameters.
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	for (int i = 0; i < 4; i++) {
		circle(img, src[i], 10, Scalar(0, 0, 255), FILLED);
	}

	imshow("Image", img);
	imshow("ImageWarp", imgWarp);
	waitKey(0);

	return 0;
}