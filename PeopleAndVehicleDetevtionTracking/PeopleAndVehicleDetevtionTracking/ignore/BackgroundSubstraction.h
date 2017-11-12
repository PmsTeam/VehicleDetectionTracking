#pragma once
#ifndef __BACKGROUNDSUBSTRACTION_H_
#define __BACKGROUNDSUBSTRACTION_H_
#include <opencv2/opencv.hpp>
using namespace cv;

void backgroundSubstraction(const Mat& cy_srcImage1, const Mat& cy_srcImage2, Mat& threshImage);

#endif
