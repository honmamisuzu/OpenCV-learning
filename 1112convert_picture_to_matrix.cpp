#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;
int standardize(int x)
{
    if (x % 100 >= 40 && x % 100 <= 60) x = (x / 100) * 100 + 50;
    else if (x % 100 >= 90) x = (x + 10) / 100 * 100;
    else if (x % 100 <= 10) x = x / 100 * 100;
    return x;
}
int main() {
    Mat im = imread("C:/opencv/pictures/31df826b4443b77cf1b775bbb7fd93b.png");
    int num = 0;
    int maze[6][6] = { 0 };
    resize(im, im, Size(600, 600), 0.5, 0.5);
    Mat gray;
    cvtColor(im, gray, COLOR_BGR2GRAY);

    Mat imBin;
    threshold(gray, imBin, 125, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    findContours(imBin, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    //drawContours(im, contours, -1, Scalar(255, 0, 255),2,8);
    //cout << standardize(49) << endl;
    for (int i = 0; i < contours.size(); ++i) {
        double area = contourArea(contours[i]);
        double length = arcLength(contours[i], true);
        //cout << "area: " << area << " " << "length: " << length << endl;
        if (length > 320 && length < 1000)
        {
            drawContours(im, contours, i, Scalar(255, 0, 255), 2, 2);
            num += 1;
            Moments mu = moments(contours[i]);
            Point2f center(mu.m10 / mu.m00, mu.m01 / mu.m00);
            //cout <<(standardize((int)(mu.m10 / mu.m00))-50)/100 << ' ' <<(standardize((int)(mu.m01 / mu.m00))-50)/100 << endl;
            maze[(standardize((int)(mu.m01 / mu.m00)) - 50) / 100][(standardize((int)(mu.m10 / mu.m00)) - 50) / 100] = 1;
        }
    }
    //circle(im, Point(50,50), 0,Scalar(255, 0, 0), 40);
    imshow("im", im);
    cout <<"Numbers of closed areas:"<< num<<endl;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            cout << maze[i][j] << ' ';
        }
        cout << endl;
    }
    waitKey(0);
    destroyAllWindows();

    return 0;

}