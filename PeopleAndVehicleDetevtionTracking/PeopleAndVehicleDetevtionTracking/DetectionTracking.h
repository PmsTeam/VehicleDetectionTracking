#pragma once
#ifndef __DETECTIONTRACKING_H_
#define __DETECTIONTRACKING_H_
#include <opencv2\opencv.hpp>
#include <iostream>
#include "Blob.h"
using namespace cv;
using namespace std;

// global variables ///////////////////////////////////////////////////////////////////////////////
const Scalar SCALAR_BLACK = Scalar(0.0, 0.0, 0.0);
const Scalar SCALAR_WHITE = Scalar(255.0, 255.0, 255.0);
const Scalar SCALAR_YELLOW = Scalar(0.0, 255.0, 255.0);
const Scalar SCALAR_GREEN = Scalar(0.0, 200.0, 0.0);
const Scalar SCALAR_RED = Scalar(0.0, 0.0, 255.0);

// function prototypes ////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(vector<Blob> &existingBlobs, vector<Blob> &currentFrameBlobs);
void addBlobToExistingBlobs(Blob &currentFrameBlob, vector<Blob> &existingBlobs, int &intIndex);
void addNewBlob(Blob &currentFrameBlob, vector<Blob> &existingBlobs);
double distanceBetweenPoints(Point point1, Point point2);
void drawAndShowContours(Size imageSize, vector<vector<Point> > contours, string strImageName);
void drawAndShowContours(Size imageSize, vector<Blob> blobs, string strImageName);
bool checkIfBlobsCrossedTheLine(vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount);
void drawBlobInfoOnImage(vector<Blob> &blobs, Mat &imgFrame2Copy);
void drawCarCountOnImage(int &carCount, Mat &imgFrame2Copy);
void drawPeopleCountOnImage(int &carCount, Mat &imgFrame2Copy);

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif
