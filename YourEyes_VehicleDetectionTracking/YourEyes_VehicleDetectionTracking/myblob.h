#pragma once
#ifndef __MYBLOB_H_
#define __MYBLOB_H_

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;

class Blob {
public:
	/* member variables */
	vector<Point> currentContour;
	Rect currentBoundingRect;
	vector<Point> centerPositions;
	double currentDiagonalSize;
	double currentAspectRatio;
	bool currentMatchFoundOrNewBlob;
	bool stillBeingTracked;
	bool isVehicle;
	int numOfConsecutiveFramesWithoutAMatch;
	Point predictedNextPosition;

	/* function prototypes */
	Blob(vector<Point> _contour);
	void predictNextPosition();

};

#endif
