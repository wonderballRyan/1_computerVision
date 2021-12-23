#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;


double calLineAngle(Point& ptStrat, Point& ptEnd)
{
    const double PI = 3.1415926;
    double angle = 0;
    if (ptStrat.x == ptEnd.x)
    {
        angle = 90;
    }
    else if (ptStrat.y == ptEnd.y)
    {
        angle = 0;
    }
    else
    {
        angle = atan((double)(ptEnd.y - ptStrat.y) / (ptEnd.x - ptStrat.x)) * (180 / PI);
        if (angle < 0)
        {
            angle = abs(angle);
        }
        else if (angle > 0)
        {
            angle = 180 - angle;
        }
        if (ptEnd.y - ptStrat.y > 0 && ptEnd.x - ptStrat.x)
        {
            angle = angle - 180;
        }

    }
    return angle;
}

void drawArrow(Mat& img, Point pStart, Point pEnd, int len, int alpha, Scalar color, int thickness, int lineType)
{
    const double PI = 3.1415926;
    Point arrow;
    //计算theat角
    double angel = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));
    line(img, pStart, pEnd, color, thickness, lineType);
    //计算箭角边的另一端的端点位置（上面的还是下面的要看箭头的指向，即pStart和pEnd的位置）
    arrow.x = pEnd.x + len * cos(angel + PI * alpha / 180);
    arrow.y = pEnd.y + len * sin(angel + PI * alpha / 180);
    line(img, pEnd, arrow, color, thickness, lineType);
    arrow.x = pEnd.x + len * cos(angel - PI * alpha / 180);
    arrow.y = pEnd.y + len * sin(angel - PI * alpha / 180);
    line(img, pEnd, arrow, color, thickness, lineType);
}

int main(int argc, char** argv)
{
    double pointToLineDist(Point & pt, Point & lineStart, Point & lineEnd);
    double calLineAngle(Point & ptStrat, Point & ptEnd);
    void drawArrow(Mat & img, Point pStart, Point pEnd, int len, int alpha, Scalar color, int thickness, int lineType);


    Mat src = imread("F:\\img_process\\imgs\\clip.png");
    imshow("输入图像", src);
    waitKey(0);
    Mat gray, binary;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    //阈值处理
    threshold(gray, binary, 150, 255, THRESH_BINARY_INV);
    imshow("二值化", binary);
    waitKey(0);
    //寻找轮廓
    vector<vector<Point> > contours;
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
    cout << contours.size() << endl;
    //轮廓分析，找到工件
    for (size_t i = 0; i < contours.size(); ++i)
    {
        //计算轮廓大小
        double area = contourArea(contours[i]);
        if (area < 2000)continue;
        double    max_dist = 0;
        Moments mm = moments(contours[i], false);
        Point ptCenter = Point(mm.m10 / mm.m00, mm.m01 / mm.m00);
        Point ptFarthest;
        for (int j = 0; j < contours[i].size(); j++)
        {
            double distance = sqrt(pow(contours[i][j].x - ptCenter.x, 2) + pow(contours[i][j].y - ptCenter.y, 2));
            if (distance > max_dist)
            {
                ptFarthest = Point(contours[i][j].x, contours[i][j].y);
                max_dist = distance;
            }
        }
        cout << max_dist << endl;
        drawContours(src, contours, i, Scalar(0, 0, 255), 5, 8);
        //imshow("contours", src);
        //waitKey(0);
        drawArrow(src, ptCenter, ptFarthest, 25, 30, Scalar(255, 0, 255), 3, 8);
        //imshow("ptCenter", src);
        //waitKey(0);
        double angle = calLineAngle(ptCenter, ptFarthest);
        cout << angle << endl;
        char strAngle[20];
        sprintf_s(strAngle, "%0.1fdegree", angle);
        putText(src, strAngle, ptCenter, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 255), 2, 8);
    }
    imshow("结果", src);
    waitKey(0);
    return 0;
}
