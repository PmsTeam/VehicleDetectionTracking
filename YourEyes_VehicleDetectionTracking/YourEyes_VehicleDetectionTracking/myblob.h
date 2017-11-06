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
	vector<Point> currentContour;//��ǰ������
	Rect currentBoundingRect;//��С��Ӿ���
	vector<Point> centerPositions;//����λ��
	double currentDiagonalSize;//�Խ���
	double currentAspectRatio;//����
	bool currentMatchFoundOrNewBlob;//����blob����blob
	bool stillBeingTracked;//�Ա�����
	bool isVehicle;//�Ƿ�Ϊ����
	int numOfConsecutiveFramesWithoutAMatch;//???
	Point predictedNextPosition;//��һ��λ��

	/* function prototypes */
	Blob(vector<Point> _contour);//���캯��
	void predictNextPosition();//Ԥ����һ��λ��
	void judgeVehicle();//�ж��Ƿ�Ϊ����

};

#endif
