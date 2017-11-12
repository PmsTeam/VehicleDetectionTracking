#pragma once
#ifndef __DETECTOR_H_
#define __DETECTOR_H_
#include <vector>
#include <opencv2\opencv.hpp>
#include <iostream>
#include "Blob.h"
#include "DetectionTracking.h"
using namespace cv;
using namespace std;

class Detector{
public:
	VideoCapture capVideo;
	Mat imgFrame1;
	Mat imgFrame2;
	Mat imgCar;
	Mat imgPeople;
	vector<Blob> blobs;
	Point crossingLine[2];
	int carCount;
	int peopleCount;
	int frameCount;
	int intHorizontalLinePosition;
	bool blnFirstFrame;

	Detector(string videoPath);
	void detectingCar();
	void detectingPeople();
};

#endif
