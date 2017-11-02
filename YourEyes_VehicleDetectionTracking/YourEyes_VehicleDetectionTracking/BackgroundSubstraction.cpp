#include "BackgroundSubstraction.h"

void backgroundSubstraction(const Mat& srcImage1, const Mat& srcImage2, Mat& diffImage, Mat& threshImage)
{
	//����������֡��ͼ��
	Mat cy_srcImage1 = srcImage1.clone();
	Mat cy_srcImage2 = srcImage2.clone();

	//��ԭͼת��Ϊ�Ҷ�ͼ��
	cvtColor(cy_srcImage1, cy_srcImage1, COLOR_BGR2GRAY);
	cvtColor(cy_srcImage2, cy_srcImage2, COLOR_BGR2GRAY);

	//��˹ģ������
	GaussianBlur(cy_srcImage1, cy_srcImage1, Size(3, 3), 0, 0);
	GaussianBlur(cy_srcImage2, cy_srcImage2, Size(3, 3), 0, 0);

	//�������
	absdiff(cy_srcImage1, cy_srcImage2, diffImage);

	//����ͼ���ֵ��
	threshold(diffImage, threshImage, 100, 255, THRESH_BINARY);
}