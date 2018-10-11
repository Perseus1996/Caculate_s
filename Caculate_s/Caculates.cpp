// Caculates.cpp: 定义控制台应用程序的入口点。
//
#include"stdafx.h"
#include <opencv2/opencv.hpp>
#include<opencv\cv.hpp>
#include<opencv2\core\core.hpp>
#include<highgui.hpp>
#include<imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
int main()
{
	Mat src_image = imread("C:\\Users\\29970\\Desktop\\testpic.png");
	if (!src_image.data)
	{
		cout << "src image load failed!" << endl;
		return -1;
	}
	namedWindow("src image", WINDOW_NORMAL);
	imshow("src image", src_image);
	//*此处高斯去燥有助于后面二值化处理的效果*//
	Mat blur_image;
	GaussianBlur(src_image, blur_image, Size(15, 15), 0, 0);
	//imshow("GaussianBlur", blur_image);
	//*灰度变换与二值化*//
	Mat gray_image, binary_image;
	cvtColor(blur_image, gray_image, COLOR_BGR2GRAY);
	threshold(gray_image, binary_image,145, 255, THRESH_BINARY_INV | THRESH_TRIANGLE);
	//imshow("binary", binary_image);
	//*形态学闭操作*//
	Mat morph_image;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(binary_image, morph_image, MORPH_CLOSE, kernel, Point(-1, -1), 2);
	//imshow("morphology", morph_image);
	//*查找外轮廓*//
	vector< vector<Point> > contours;
	vector<Vec4i> hireachy;
	findContours(morph_image, contours, hireachy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	vector<Moments> m(contours.size());
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		m[i] = moments(contours[i], false);
	} 
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2d(m[i].m10 /m[i].m00, m[i].m01 / m[i].m00);
	}
	Mat result_image = Mat::zeros(src_image.size(), CV_8UC3);
	Mat drawing = Mat::zeros(src_image.size(), CV_8UC3);
	double a = 0;
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	double area = contourArea(contours[i]);
	//	double a = std::max(a, area);
	//	return a;
	//	//cout << "a:" << a << endl;
	//}
	for (int i = 0; i< contours.size(); i++)
	{
		double area = contourArea(contours[i]);	
		if (area <500)
			continue;
		Scalar color = Scalar(255, 0, 0);
		//drawContours(drawing, contours, i, color, 2, 8, hireachy, 0, Point());
		drawContours(result_image, contours, static_cast<int>(i), Scalar(0, 100, 255), 2, 8, hireachy);
		circle(result_image, mc[i], 5, Scalar(0, 0, 255), -1, 8, 0);
		rectangle(result_image, boundingRect(contours.at(i)), cvScalar(0, 255, 0));
		char tam[1000];
		sprintf_s(tam, "(%0.0f,%0.0f)", mc[i].x, mc[i].y);
		putText(result_image, tam, Point(mc[i].x, mc[i].y), FONT_HERSHEY_SIMPLEX, 0.4, cvScalar(255, 0, 255), 1);
	}
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", result_image);
	imwrite("C:\\Users\\29970\\Desktop\\testpic_result.png", result_image);
	//for (int i= 0; i < contours.size(); i++)
	//{
	//	//*过滤掉小的干扰轮廓*//
	//	/*Rect rect = boundingRect(contours[t]);
	//	if (rect.width < src_image.cols / 2)
	//		continue;
	//	if (rect.width >(src_image.cols - 20))
	//		continue;*/
	//	double area = contourArea(contours[i]);
	//	//double len = arcLength(contours[t], true);
	//	drawContours(result_image, contours, static_cast<int>(i), Scalar(0, 100, 255), 2, 8, hireachy);
	//	cout << "area of start cloud: " << area << endl;
	//	//cout << "len of start cloud: " << len << endl;
	//}
	//imshow("result image", result_image);
	waitKey(0);
}