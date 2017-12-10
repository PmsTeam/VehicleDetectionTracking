#include "VehicleDetectionTracking.h"

Mat imgCar;
//vector<double> frameTime;//视频帧当前时间

void vehicleDetectionTracking(const string videoPath)
{
	VideoCapture capVideo;

	capVideo.open(videoPath);

	if (!capVideo.isOpened())
	{
		cout << "错误：没有读取到视频文件！" << endl;
		return;
	}

	if (capVideo.get(CV_CAP_PROP_FRAME_COUNT) < 2)
	{
		cout << "错误：视频帧数小于2！" << endl;
		return;
	}

	Mat imgFrame1;
	Mat imgFrame2;
	capVideo.read(imgFrame1);
	capVideo.read(imgFrame2);

	//定义计数横线位置
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
		//获取当前帧的距离视频开始的时间位置（ms）
		//frameTime.push_back(capVideo.get(CV_CAP_PROP_POS_MSEC));

		Mat imgFrame1Copy = imgFrame1.clone();
		Mat imgFrame2Copy = imgFrame2.clone();
		Mat imgDifference;
		Mat imgThresh;

		//减除法背景建模并更新背景
		cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
		cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);
		GaussianBlur(imgFrame1Copy, imgFrame1Copy, Size(5, 5), 0);
		GaussianBlur(imgFrame2Copy, imgFrame2Copy, Size(5, 5), 0);
		absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);
		
		//获得二值化图像
		threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);
		imshow("imgThresh", imgThresh);

		//获取常用的结构元素的形状
		Mat structuringElement3x3 = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat structuringElement5x5 = getStructuringElement(MORPH_RECT, Size(5, 5));
		Mat structuringElement7x7 = getStructuringElement(MORPH_RECT, Size(7, 7));
		Mat structuringElement15x15 = getStructuringElement(MORPH_RECT, Size(15, 15));

		//闭运算排除小型黑洞
		for (unsigned int i = 0; i < 2; i++)
		{
			dilate(imgThresh, imgThresh, structuringElement5x5);
			dilate(imgThresh, imgThresh, structuringElement5x5);
			erode(imgThresh, imgThresh, structuringElement5x5);
		}

		Mat imgThreshCopy = imgThresh.clone();

		//寻找并绘制轮廓，每个轮廓存储为一个点向量
		vector<vector<Point> > contours;
		findContours(imgThreshCopy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawAndShowContours(imgThresh.size(), contours, "imgContours");

		//寻找并绘制凸包
		vector<vector<Point> > convexHulls(contours.size());
		for (unsigned int i = 0; i < contours.size(); i++)
			convexHull(contours[i], convexHulls[i]);
		drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

		vector<Blob> currentFrameBlobs;//当前帧中的团块

		//检测团块是否为车辆
		for (auto &convexHull : convexHulls)
		{
			Blob possibleBlob(convexHull);
			possibleBlob.judgeVehicle(possibleBlob);
			if (possibleBlob.blnIsVehicle)
				currentFrameBlobs.push_back(possibleBlob);
		}
		drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

		//将当前车辆团块放入总车辆团块集合
		if (blnFirstFrame == true)
			for (auto &currentFrameBlob : currentFrameBlobs)
				blobs.push_back(currentFrameBlob);
		else
			matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
		drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

		// get another copy of frame 2 since we changed the previous frame 2 copy in the processing above
		imgFrame2Copy = imgFrame2.clone();
		drawBlobInfoOnImage(blobs, imgFrame2Copy);

		//判断车辆是否越过横线
		bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, intHorizontalLinePosition, carCount);
		if (blnAtLeastOneBlobCrossedTheLine == true)
			line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
		else
			line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
		drawCarCountOnImage(carCount, imgFrame2Copy);
		imshow("imgFrame2Copy", imgFrame2Copy);

		imgCar = imgFrame2Copy.clone();

		//读取下一帧图像
		currentFrameBlobs.clear();
		imgFrame1 = imgFrame2.clone();
		if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT))
		{
			capVideo.read(imgFrame2);
		}
		else
		{
			cout << "视频结束" << endl;
			break;
		}

		blnFirstFrame = false;
		frameCount++;
		chCheckForEscKey = waitKey(1);
	}

	if (chCheckForEscKey != 27)
		waitKey(1); 
}