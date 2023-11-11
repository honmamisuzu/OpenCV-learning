#include <iostream>
#include <queue>
#include <stack>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include<vector>
using namespace std;
cv::Mat imgHSV, mask;
int hmin = 0, smin = 0, vmin = 116;
int hmax = 111, smax = 175, vmax = 2555;
float w = 600, h = 600;
cv::Mat matrix, imgWarp;
struct Point {
	int row;
	int col;
	Point(int x, int y) {
		this->row = x;
		this->col = y;
	}

	bool operator!=(const Point& rhs) {
		if (this->row != rhs.row || this->col != rhs.col)
			return true;
		return false;
	}

	bool operator==(const Point& rhs) const {
		if (this->row == rhs.row && this->col == rhs.col)
			return true;
		return false;
	}
};
Point getAdjacentNotVisitedNode(int** mark, Point point, int m, int n, Point endP) {
	Point resP(-1, -1);

	if (point.row - 1 >= 0) {
		if (mark[point.row - 1][point.col] == 0 || mark[point.row][point.col] + 1 < mark[point.row - 1][point.col]) {
			resP.row = point.row - 1;
			resP.col = point.col;
			return resP;
		}
	}
	if (point.col + 1 < n) {
		if (mark[point.row][point.col + 1] == 0 || mark[point.row][point.col] + 1 < mark[point.row][point.col + 1])
		{
			resP.row = point.row;
			resP.col = point.col + 1;
			return resP;
		}
	}
	if (point.row + 1 < m) {
		if (mark[point.row + 1][point.col] == 0 || mark[point.row][point.col] + 1 < mark[point.row + 1][point.col]) {
			resP.row = point.row + 1;
			resP.col = point.col;
			return resP;
		}
	}
	if (point.col - 1 >= 0) {
		if (mark[point.row][point.col - 1] == 0 || mark[point.row][point.col] + 1 < mark[point.row][point.col - 1]) {
			resP.row = point.row;
			resP.col = point.col - 1;
			return resP;
		}
	}
	return resP;
}
void mazePath(void* maze, int m, int n, Point& startP, Point endP, stack<Point>& pointStack, vector<Point>& vecPath)
{
	int** maze2d = new int* [m];
	for (int i = 0; i < m; ++i)
	{
		maze2d[i] = (int*)maze + i * n;
	}
	if (maze2d[startP.row][startP.col] == -1 || maze2d[endP.row][endP.col] == -1) {
		return;
	}
	int** mark = new int* [m];
	for (int i = 0; i < m; ++i) {
		mark[i] = new int[n];
	}
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			mark[i][j] = *((int*)maze + i * n + j);
		}
	}
	if (startP == endP) {
		vecPath.push_back(startP);
		return;
	}
	vector<Point> visitedEndPointPreNodeVec;
	pointStack.push(startP);
	mark[startP.row][startP.col] = 1;
	while (pointStack.empty() == false) {
		Point adjacentNotVisitedNode = getAdjacentNotVisitedNode(mark, pointStack.top(), m, n, endP);
		if (adjacentNotVisitedNode.row == -1) {
			pointStack.pop();
			continue;
		}
		if (adjacentNotVisitedNode == endP) {
			mark[adjacentNotVisitedNode.row][adjacentNotVisitedNode.col] = mark[pointStack.top().row][pointStack.top().col] + 1;
			pointStack.push(endP);
			stack<Point> pointStackTemp = pointStack;
			vecPath.clear();
			while (pointStackTemp.empty() == false) {
				vecPath.push_back(pointStackTemp.top());
				pointStackTemp.pop();
			}
			pointStack.pop();
			continue;
		}
		mark[adjacentNotVisitedNode.row][adjacentNotVisitedNode.col] = mark[pointStack.top().row][pointStack.top().col] + 1;
		pointStack.push(adjacentNotVisitedNode);
	}
}
int maze[6][6] = {
{0, 1, 1, 1, 1, 1},
{0, 1, 0, 0, 0, 0},
{0, 1, 0, 1 ,1, 0},
{0, 1, 0, 1, 0, 0},
{0, 0, 0, 1, 0, 1},
{1, 1, 1, 1, 0, 0},
};

int main() {
	cv::VideoCapture cap(0);
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
	imshow("bineryimage", mask);
	Point startP(5, 5);
	Point endP(0, 0);
	stack<Point>  pointStack;
	vector<Point> vecPath;
	mazePath(maze, 6, 6, startP, endP, pointStack, vecPath);

	if (vecPath.empty() == true) {
		cout << "no right path" << endl;
	}
	else {
		cout << "shortest path:";
		for (auto i = vecPath.rbegin(); i != vecPath.rend(); ++i) {
			printf("(%d,%d) ", i->row, i->col);
			cv::circle(mask, cv::Point(100 * (i->col) + 50, 100 * (i->row) + 50), 0, cv::Scalar(255, 0, 0), 40);
		}
	}
	imshow("path", mask);
	cv::waitKey(0);
	return 0;
}