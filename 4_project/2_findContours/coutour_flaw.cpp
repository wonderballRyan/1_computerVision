#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat src = imread("F:\\img_process\\imgs\\flaw.png");
    imshow("输入图像", src);
    Mat gray, gray_mean, dst, binary1, binary2, binary;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray_mean, 201);
    imshow("中值滤波", gray_mean);
    addWeighted(gray, -1, gray_mean, 1, 0, dst);
    imshow("做差", dst);
    //阈值提取
    threshold(dst, binary1, 10, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("二值化", binary1);
    Mat src_open, src_close;
    //形态学
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7), Point(-1, -1));
    morphologyEx(binary1, src_open, MORPH_OPEN, kernel, Point(-1, -1));
    imshow("开运算", src_open);
    morphologyEx(src_open, src_close, MORPH_CLOSE, kernel, Point(-1, -1));
    imshow("闭运算", src_close);
    vector<vector<Point>>contours;
    findContours(src_close, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
    for (int i = 0; i < contours.size(); i++)
    {
        float area = contourArea(contours[i]);
        cout << area << endl;
        if (area > 1000)
        {
            drawContours(src, contours, i, Scalar(0, 0, 255), 2, 8);
        }
    }
    imshow("结果", src);
    waitKey(0);
    return 0;
}
