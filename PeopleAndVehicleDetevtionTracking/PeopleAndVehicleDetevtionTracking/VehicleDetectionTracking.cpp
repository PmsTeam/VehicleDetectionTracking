#include "VehicleDetectionTracking.h"

Mat imgCar;
//vector<double> frameTime;//��Ƶ֡��ǰʱ��

void vehicleDetectionTracking(const string videoPath)
{
	VideoCapture capVideo;

	capVideo.open(videoPath);

	if (!capVideo.isOpened())
	{
		cout << "����û�ж�ȡ����Ƶ�ļ���" << endl;
		return;
	}

	if (capVideo.get(CV_CAP_PROP_FRAME_COUNT) < 2)
	{
		cout << "������Ƶ֡��С��2��" << endl;
		return;
	}

	Mat imgFrame1;
	Mat imgFrame2;
	capVideo.read(imgFrame1);
	capVideo.read(imgFrame2);

	//�����������λ��
	Point crossingLine[2];
	int intHorizontalLinePosition = (int)round((double)imgFrame1.rows * 0.35);
	crossingLine[0].x = 0;
	crossingLine[0].y = intHorizontalLinePosition;
	crossingLine[1].x = imgFrame1.cols - 1;
	crossingLine[1].y = intHorizontalLinePosition;

	char chCheckForEscKey = 0;
	bool blnFirstFrame = true;
	int frameCount = 2;
	int carCount = 0;

	//int frame_count = capVideo.get(CV_CAP_PROP_FRAME_COUNT);
	//fps = capVideo.get(CV_CAP_PROP_FPS);

	vector<Blob> blobs;

	while (capVideo.isOpened() && chCheckForEscKey != 27)
	{
		//��ȡ��ǰ֡�ľ�����Ƶ��ʼ��ʱ��λ�ã�ms��
		//frameTime.push_back(capVideo.get(CV_CAP_PROP_POS_MSEC));

		Mat imgFrame1Copy = imgFrame1.clone();
		Mat imgFrame2Copy = imgFrame2.clone();
		Mat imgDifference;
		Mat imgThresh;

		//������������ģ�����±���
		cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
		cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);
		GaussianBlur(imgFrame1Copy, imgFrame1Copy, Size(5, 5), 0);
		GaussianBlur(imgFrame2Copy, imgFrame2Copy, Size(5, 5), 0);
		absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);
		
		//��ö�ֵ��ͼ��
		threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);
		imshow("imgThresh", imgThresh);

		//��ȡ���õĽṹԪ�ص���״
		Mat structuringElement3x3 = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat structuringElement5x5 = getStructuringElement(MORPH_RECT, Size(5, 5));
		Mat structuringElement7x7 = getStructuringElement(MORPH_RECT, Size(7, 7));
		Mat structuringElement15x15 = getStructuringElement(MORPH_RECT, Size(15, 15));

		//�������ų�С�ͺڶ�
		for (unsigned int i = 0; i < 2; i++)
		{
			dilate(imgThresh, imgThresh, structuringElement5x5);
			dilate(imgThresh, imgThresh, structuringElement5x5);
			erode(imgThresh, imgThresh, structuringElement5x5);
		}

		Mat imgThreshCopy = imgThresh.clone();

		//Ѱ�Ҳ�����������ÿ�������洢Ϊһ��������
		vector<vector<Point> > contours;
		findContours(imgThreshCopy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawAndShowContours(imgThresh.size(), contours, "imgContours");

		//Ѱ�Ҳ�����͹��
		vector<vector<Point> > convexHulls(contours.size());
		for (unsigned int i = 0; i < contours.size(); i++)
			convexHull(contours[i], convexHulls[i]);
		drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

		vector<Blob> currentFrameBlobs;//��ǰ֡�е��ſ�

		//����ſ��Ƿ�Ϊ����
		for (auto &convexHull : convexHulls)
		{
			Blob possibleBlob(convexHull);
			possibleBlob.judgeVehicle(possibleBlob);
			if (possibleBlob.blnIsVehicle)
				currentFrameBlobs.push_back(possibleBlob);
		}
		drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

		//����ǰ�����ſ�����ܳ����ſ鼯��
		if (blnFirstFrame == true)
			for (auto &currentFrameBlob : currentFrameBlobs)
				blobs.push_back(currentFrameBlob);
		else
			matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
		drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

		// get another copy of frame 2 since we changed the previous frame 2 copy in the processing above
		imgFrame2Copy = imgFrame2.clone();
		drawBlobInfoOnImage(blobs, imgFrame2Copy);

		//�жϳ����Ƿ�Խ������
		bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, intHorizontalLinePosition, carCount);
		if (blnAtLeastOneBlobCrossedTheLine == true)
			line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
		else
			line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
		drawCarCountOnImage(carCount, imgFrame2Copy);
		imshow("imgFrame2Copy", imgFrame2Copy);

		imgCar = imgFrame2Copy.clone();

		//��ȡ��һ֡ͼ��
		currentFrameBlobs.clear();
		imgFrame1 = imgFrame2.clone();
		if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT))
		{
			capVideo.read(imgFrame2);
		}
		else
		{
			cout << "��Ƶ����" << endl;
			break;
		}

		blnFirstFrame = false;
		frameCount++;
		chCheckForEscKey = waitKey(1);
	}

	if (chCheckForEscKey != 27)
		waitKey(1); 
}