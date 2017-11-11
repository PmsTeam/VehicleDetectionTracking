#include "BackgroundSubstraction.h"

void backgroundSubstraction(const Mat& cy_srcImage1, const Mat& cy_srcImage2, Mat& threshImage)
{
	//将原图转化为灰度图像
	cvtColor(cy_srcImage1, cy_srcImage1, COLOR_BGR2GRAY);
	cvtColor(cy_srcImage2, cy_srcImage2, COLOR_BGR2GRAY);

	//高斯模糊降噪
	GaussianBlur(cy_srcImage1, cy_srcImage1, Size(3, 3), 0, 0);
	GaussianBlur(cy_srcImage2, cy_srcImage2, Size(3, 3), 0, 0);

	//背景相减
	Mat diffImage;
	absdiff(cy_srcImage1, cy_srcImage2, diffImage);

	//所得图像二值化
	threshold(diffImage, threshImage, 100, 255, THRESH_BINARY);

	imshow("imgThresh", threshImage);

}