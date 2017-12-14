#pragma once
#ifndef __VEHICLEDETECTIONTRACKING_H_
#define __VEHICLEDETECTIONTRACKING_H_
#include <opencv2\opencv.hpp>
#include <iostream>
#include "Blob.h"
#include "DetectionTracking.h"
using namespace cv;
using namespace std;

void vehicleDetectionTracking(const string sourcePath, const string outputPath);

#endif
