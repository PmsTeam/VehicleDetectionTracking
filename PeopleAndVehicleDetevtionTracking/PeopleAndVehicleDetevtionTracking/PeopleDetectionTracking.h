#pragma once
#ifndef __PEOPLEDETECTIONTRACKING_H_
#define __PEOPLEDETECTIONTRACKING_H_
#include <opencv2\opencv.hpp>
#include <iostream>
#include "Blob.h"
#include "DetectionTracking.h"
using namespace cv;
using namespace std;

void peopleDetectionTracking(const string videoPath);

#endif
