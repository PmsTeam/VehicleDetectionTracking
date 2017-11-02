#pragma once
#ifndef __BACKGROUNDSUBSTRACTION_H_
#define __BACKGROUNDSUBSTRACTION_H_
#include <opencv2/opencv.hpp>
using namespace cv;

void backgroundSubstraction(const Mat& srcImage1, const Mat& srcImage2, Mat& diffImage, Mat& threshImage);

#endif
