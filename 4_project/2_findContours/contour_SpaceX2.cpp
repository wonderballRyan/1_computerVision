#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 4. 绘制轮廓函数
void drawMyContours(string winName, Mat& image, vector<vector<Point>> contours, bool draw_on_blank)
{
	Mat temp;
	if (draw_on_blank) // 在白底上绘制轮廓
	{
		temp = Mat(image.size(), CV_8U, Scalar(255));
		drawContours(
			temp,
			contours,
			-1,//画全部轮廓
			0, //用黑色画
			2);//宽度为2
	}
	else // 在原图上绘制轮廓
	{
		temp = image.clone();
		drawContours(
			temp,
			contours,
			-1,//画全部轮廓
			Scalar(0, 0, 255), //用red画
			1);//宽度为2
	}
	imshow(winName, temp);
	waitKey();
}

int main() {
	// 1.载入图像
	Mat image = imread("F:\\img_process\\imgs\\spaceX2.png", 1);
	imshow("original", image);
	waitKey(0);

	// 2.预处理
	Mat gray, binary, element; // 临时变量
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	GaussianBlur(gray, gray, Size(3, 3), 1);
	threshold(gray, binary, 80, 255, THRESH_BINARY_INV);
	imshow("binary", binary);

	element = getStructuringElement(MORPH_RECT, Size(3, 3));//33全1结构元素
	morphologyEx(binary, binary, cv::MORPH_CLOSE, element);
	imshow("morphology", binary);
	waitKey();

	// 3.查找轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(
		binary,               // 输入二值图
		contours,             // 存储轮廓的向量
		hierarchy,            // 轮廓层次信息
		RETR_TREE,            // 检索所有轮廓并重建嵌套轮廓的完整层次结构
		CHAIN_APPROX_NONE);   // 每个轮廓的全部像素
	printf("find %d contours", contours.size());

	// 4.绘制原始轮廓
	drawMyContours("contours", image, contours, true);

	// 5.筛选轮廓
	// 初始化迭代器
	vector<vector<Point>>::iterator itc = contours.begin();
	vector<Vec4i>::iterator itc_hierarchy = hierarchy.begin();

	// 5.1使用层级结构筛选轮廓
	int i = 0;
	while (itc_hierarchy != hierarchy.end())
	{
		//验证轮廓大小
		if (hierarchy[i][2] > 0 || hierarchy[i][3] > 0) // 存在子轮廓/父轮廓
		{
			itc = contours.erase(itc);
			itc_hierarchy = hierarchy.erase(itc_hierarchy);
		}
		else
		{
			++i;
			++itc;
			++itc_hierarchy;
		}
	}
	printf("%d contours remaining after hierarchy filtering", contours.size());
	// 绘制级别筛选后的轮廓
	drawMyContours("contours after hierarchy filtering", image, contours, true);

	// 5.2使用轮廓长度滤波
	int min_size = 200;
	int max_size = 400;

	// 针对所有轮廓
	itc = contours.begin();
	itc_hierarchy = hierarchy.begin();
	while (itc != contours.end())
	{
		//验证轮廓大小
		if (itc->size() < min_size || itc->size() > max_size)
		{
			itc = contours.erase(itc);
			itc_hierarchy = hierarchy.erase(itc_hierarchy);
		}
		else
		{
			++itc;
			++itc_hierarchy;
		}
	}
	printf("%d contours remaining after length filtering", contours.size());
	// 绘制长度筛选后的轮廓
	drawMyContours("contours after length filtering", image, contours, true);

	// 6.形状描述子
	// 最小覆盖矩形
	Mat result = image.clone();
	//Rect rect = boundingRect(contours[0]);//轮廓1
	//rectangle(result, rect, Scalar(0, 255, 255), 1);//画矩形
	// 凸包
	vector<Point> hull;
	convexHull(contours[0], hull);//轮廓2
	polylines(result, hull, true, Scalar(0, 255, 0), 1);//画多边形
	imshow("bounding", result);
	waitKey();

	// 7.计算轮廓矩，画重心
	itc = contours.begin();
	while (itc != contours.end()) {
		// 计算全部轮廓矩
		Moments mom = moments(Mat(*itc++));
		// 画重心
		Point pt = Point(mom.m10 / mom.m00, mom.m01 / mom.m00);    //使用前三个矩m00, m01和m10计算重心
		circle(result, pt, 2, Scalar(0, 0, 255), 2);//画红点
				// 标注重心坐标值
		string text_x = to_string(pt.x);
		string text_y = to_string(pt.y);
		string text = "(" + text_x + ", " + text_y + ")";
		putText(result, text, Point(pt.x + 10, pt.y + 10), FONT_HERSHEY_PLAIN, 1.5, Scalar::all(255), 1, 8, 0);
	}
	imshow("center", result);
	waitKey();
}
