#include <iostream>
#include <queue>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
cv::Mat imgHSV, mask;
int hmin = 0, smin = 0, vmin = 116;
int hmax = 111, smax = 175, vmax = 2555;
float w = 600, h = 600;
cv::Mat matrix, imgWarp;
struct Point {
	int row;
	int col;
	Point() {
		row = col = -1;
	}

	Point(int x, int y) {
		this->row = x;
		this->col = y;
	}
	bool operator==(const Point& rhs) const {
		if (this->row == rhs.row && this->col == rhs.col)
			return true;
		return false;
	}
};
int maze[6][6] = {
{0, 1, 1, 1, 1, 1},
{0, 1, 0, 0, 0, 0},
{0, 1, 0, 1 ,1, 0},
{0, 1, 0, 1, 0, 0},
{0, 0, 0, 1, 0, 1},
{1, 1, 1, 1, 0, 0},
};
void mazePath(void* maze, int m, int n, Point& startP, Point endP, vector<Point>& shortestPath) {
	int** maze2d = new int* [m];
	for (int i = 0; i < m; ++i) {
		maze2d[i] = (int*)maze + i * n;
	}
	if (maze2d[startP.row][startP.col] == 1 || maze2d[startP.row][startP.col] == 1) return; //输入错误
	if (startP == endP) {
		shortestPath.push_back(startP);
		return;
	}
	Point** mark = new Point * [m];
	for (int i = 0; i < m; ++i) {
		mark[i] = new Point[n];
	}
	queue<Point> queuePoint;
	queuePoint.push(startP);
	mark[startP.row][startP.col] = startP;
	while (queuePoint.empty() == false) {
		Point pointFront = queuePoint.front();
		queuePoint.pop();
		if (pointFront.row - 1 >= 0 && maze2d[pointFront.row - 1][pointFront.col] == 0) {
			if (mark[pointFront.row - 1][pointFront.col] == Point()) {
				mark[pointFront.row - 1][pointFront.col] = pointFront;
				queuePoint.push(Point(pointFront.row - 1, pointFront.col));
				if (Point(pointFront.row - 1, pointFront.col) == endP) {
					break;
				}
			}
		}
		if (pointFront.col + 1 < n && maze2d[pointFront.row][pointFront.col + 1] == 0) {
			if (mark[pointFront.row][pointFront.col + 1] == Point()) {
				mark[pointFront.row][pointFront.col + 1] = pointFront;
				queuePoint.push(Point(pointFront.row, pointFront.col + 1));
				if (Point(pointFront.row, pointFront.col + 1) == endP) {
					break;
				}
			}
		}
		if (pointFront.row + 1 < m && maze2d[pointFront.row + 1][pointFront.col] == 0) {
			if (mark[pointFront.row + 1][pointFront.col] == Point()) {
				mark[pointFront.row + 1][pointFront.col] = pointFront;
				queuePoint.push(Point(pointFront.row + 1, pointFront.col));
				if (Point(pointFront.row + 1, pointFront.col) == endP) {
					break;
				}
			}
		}

		if (pointFront.col - 1 >= 0 && maze2d[pointFront.row][pointFront.col - 1] == 0) {
			if (mark[pointFront.row][pointFront.col - 1] == Point()) {
				mark[pointFront.row][pointFront.col - 1] = pointFront;
				queuePoint.push(Point(pointFront.row, pointFront.col - 1));
				if (Point(pointFront.row, pointFront.col - 1) == endP) {
					break;
				}
			}
		}
	}
	if (queuePoint.empty() == false) {
		int row = endP.row;
		int col = endP.col;
		shortestPath.push_back(endP);
		while (!(mark[row][col] == startP)) {
			shortestPath.push_back(mark[row][col]);
			row = mark[row][col].row;
			col = mark[row][col].col;
		}
		shortestPath.push_back(startP);
	}
}
int main() {
	cv::VideoCapture cap(1);
	cv::Mat img;
	cap >> img;
	cv::Point2f src[4] = { {185, 75}, {565, 35}, {220, 420}, {570, 410} };
	cv::Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };
	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, cv::Point(w, h));
	cvtColor(imgWarp, imgHSV, cv::COLOR_BGR2HSV);
	imshow("Image", img);
	cv::Scalar lower(hmin, smin, vmin);
	cv::Scalar upper(hmax, smax, vmax);
	inRange(imgHSV, lower, upper, mask);
	imshow("bineryImage", mask);
	Point startP(0, 0);
	Point endP(5, 5);
	vector<Point> vecPath;
	mazePath(maze, 6, 6, startP, endP, vecPath);
	if (vecPath.empty() == true)
		cout << "no right path" << endl;
	else {
		cout << "shortest path:";
		for (auto i = vecPath.rbegin(); i != vecPath.rend(); ++i)
			printf("(%d,%d) ", i->row, i->col);
	}
	cv::waitKey(0);
	return 0;
}
