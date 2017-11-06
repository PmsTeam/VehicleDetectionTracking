#pragma once
#ifndef __MYBLOB_H_
#define __MYBLOB_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;

class Blob {
public:
	/* member variables */
	vector<Point> currentContour;//当前轮廓集
	Rect currentBoundingRect;//最小外接矩形
	vector<Point> centerPositions;//质心位置
	double currentDiagonalSize;//对角线
	double currentAspectRatio;//比例
	bool currentMatchFoundOrNewBlob;//已有blob或新blob
	bool stillBeingTracked;//仍被跟踪
	bool isVehicle;//是否为车辆
	int numOfConsecutiveFramesWithoutAMatch;//???
	Point predictedNextPosition;//下一个位置

	/* function prototypes */
	Blob(vector<Point> _contour);//构造函数
	void predictNextPosition();//预测下一个位置
	void judgeVehicle();//判断是否为车辆

};

#endif
