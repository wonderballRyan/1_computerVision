#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 4. ������������
void drawMyContours(string winName, Mat& image, vector<vector<Point>> contours, bool draw_on_blank)
{
	Mat temp;
	if (draw_on_blank) // �ڰ׵��ϻ�������
	{
		temp = Mat(image.size(), CV_8U, Scalar(255));
		drawContours(
			temp,
			contours,
			-1,//��ȫ������
			0, //�ú�ɫ��
			2);//���Ϊ2
	}
	else // ��ԭͼ�ϻ�������
	{
		temp = image.clone();
		drawContours(
			temp,
			contours,
			-1,//��ȫ������
			Scalar(0, 0, 255), //��red��
			1);//���Ϊ2
	}
	imshow(winName, temp);
	waitKey();
}

int main() {
	// 1.����ͼ��
	Mat image = imread("F:\\img_process\\imgs\\spaceX2.png", 1);
	imshow("original", image);
	waitKey(0);

	// 2.Ԥ����
	Mat gray, binary, element; // ��ʱ����
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	GaussianBlur(gray, gray, Size(3, 3), 1);
	threshold(gray, binary, 80, 255, THRESH_BINARY_INV);
	imshow("binary", binary);

	element = getStructuringElement(MORPH_RECT, Size(3, 3));//33ȫ1�ṹԪ��
	morphologyEx(binary, binary, cv::MORPH_CLOSE, element);
	imshow("morphology", binary);
	waitKey();

	// 3.��������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(
		binary,               // �����ֵͼ
		contours,             // �洢����������
		hierarchy,            // ���������Ϣ
		RETR_TREE,            // ���������������ؽ�Ƕ��������������νṹ
		CHAIN_APPROX_NONE);   // ÿ��������ȫ������
	printf("find %d contours", contours.size());

	// 4.����ԭʼ����
	drawMyContours("contours", image, contours, true);

	// 5.ɸѡ����
	// ��ʼ��������
	vector<vector<Point>>::iterator itc = contours.begin();
	vector<Vec4i>::iterator itc_hierarchy = hierarchy.begin();

	// 5.1ʹ�ò㼶�ṹɸѡ����
	int i = 0;
	while (itc_hierarchy != hierarchy.end())
	{
		//��֤������С
		if (hierarchy[i][2] > 0 || hierarchy[i][3] > 0) // ����������/������
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
	// ���Ƽ���ɸѡ�������
	drawMyContours("contours after hierarchy filtering", image, contours, true);

	// 5.2ʹ�����������˲�
	int min_size = 200;
	int max_size = 400;

	// �����������
	itc = contours.begin();
	itc_hierarchy = hierarchy.begin();
	while (itc != contours.end())
	{
		//��֤������С
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
	// ���Ƴ���ɸѡ�������
	drawMyContours("contours after length filtering", image, contours, true);

	// 6.��״������
	// ��С���Ǿ���
	Mat result = image.clone();
	//Rect rect = boundingRect(contours[0]);//����1
	//rectangle(result, rect, Scalar(0, 255, 255), 1);//������
	// ͹��
	vector<Point> hull;
	convexHull(contours[0], hull);//����2
	polylines(result, hull, true, Scalar(0, 255, 0), 1);//�������
	imshow("bounding", result);
	waitKey();

	// 7.���������أ�������
	itc = contours.begin();
	while (itc != contours.end()) {
		// ����ȫ��������
		Moments mom = moments(Mat(*itc++));
		// ������
		Point pt = Point(mom.m10 / mom.m00, mom.m01 / mom.m00);    //ʹ��ǰ������m00, m01��m10��������
		circle(result, pt, 2, Scalar(0, 0, 255), 2);//�����
				// ��ע��������ֵ
		string text_x = to_string(pt.x);
		string text_y = to_string(pt.y);
		string text = "(" + text_x + ", " + text_y + ")";
		putText(result, text, Point(pt.x + 10, pt.y + 10), FONT_HERSHEY_PLAIN, 1.5, Scalar::all(255), 1, 8, 0);
	}
	imshow("center", result);
	waitKey();
}
