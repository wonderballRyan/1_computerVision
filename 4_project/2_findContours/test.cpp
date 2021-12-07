#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

/*
int main()
{
    Mat moveLightDiff(Mat src, int radius);
    Mat src = imread("F:\\img_process\\imgs\\alm.png");
    imshow("原始图", src);
    Mat src_hsv, src_h, src_s, src_v, src_tophat, src_bin;
    cvtColor(src, src_hsv, COLOR_RGB2HSV);
    vector<Mat> rgb_planes;
    split(src_hsv, rgb_planes);
    src_h = rgb_planes[0];
    imshow("H通道", src_h);
    waitKey();
    Mat kernel = getStructuringElement(MORPH_RECT, Size(30, 30), Point(-1, -1));
    morphologyEx(src_h, src_h, MORPH_BLACKHAT, kernel, Point(-1, -1));
    imshow("BLACKPHAT", src_h);
    threshold(src_h, src_bin, 100, 255, THRESH_OTSU);
    imshow("二值化", src_bin);
    int max = 0;
    int index = -1;
    vector<vector<Point> >contours;
    findContours(src_bin, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); i++) {
        int area = contourArea(contours[i]);
        if (max < area) {
            index = i;
            max = area;
        }
    }
    drawContours(src, contours, index, Scalar(0, 0, 255), 2);
    imshow("显示轮廓", src);
    waitKey();
}
*/