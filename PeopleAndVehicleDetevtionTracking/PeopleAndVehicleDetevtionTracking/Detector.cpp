#include "Detector.h"

Detector::Detector(string videoPath)
{
	carCount = 0;
	peopleCount = 0;
	frameCount = 2;

	capVideo.open(videoPath);
	if (!capVideo.isOpened())
	{
		cout << "error reading video file" << endl << endl;
		return;
	}
	if (capVideo.get(CV_CAP_PROP_FRAME_COUNT) < 2)
	{
		cout << "error: video file must have at least two frames";
		return;
	}
	capVideo.read(imgFrame1);
	capVideo.read(imgFrame2);

	//定义计数横线位置
	intHorizontalLinePosition = (int)round((double)imgFrame1.rows * 0.35);
	crossingLine[0].x = 0;
	crossingLine[0].y = intHorizontalLinePosition;
	crossingLine[1].x = imgFrame1.cols - 1;
	crossingLine[1].y = intHorizontalLinePosition;

	blnFirstFrame = true;
}

void Detector::detectingCar()
{
	char chCheckForEscKey = 0;

	while (capVideo.isOpened() && chCheckForEscKey != 27)
	{
		vector<Blob> currentFrameBlobs;

		Mat imgFrame1Copy = imgFrame1.clone();
		Mat imgFrame2Copy = imgFrame2.clone();
		Mat imgDifference;
		Mat imgThresh;

		cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
		cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);
		GaussianBlur(imgFrame1Copy, imgFrame1Copy, Size(5, 5), 0);
		GaussianBlur(imgFrame2Copy, imgFrame2Copy, Size(5, 5), 0);
		absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);
		threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);
		imshow("imgThresh", imgThresh);

		Mat structuringElement3x3 = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat structuringElement5x5 = getStructuringElement(MORPH_RECT, Size(5, 5));
		Mat structuringElement7x7 = getStructuringElement(MORPH_RECT, Size(7, 7));
		Mat structuringElement15x15 = getStructuringElement(MORPH_RECT, Size(15, 15));

		for (unsigned int i = 0; i < 2; i++)
		{
			dilate(imgThresh, imgThresh, structuringElement5x5);
			dilate(imgThresh, imgThresh, structuringElement5x5);
			erode(imgThresh, imgThresh, structuringElement5x5);
		}

		Mat imgThreshCopy = imgThresh.clone();

		vector<vector<Point> > contours;
		findContours(imgThreshCopy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawAndShowContours(imgThresh.size(), contours, "imgContours");

		vector<vector<Point> > convexHulls(contours.size());
		for (unsigned int i = 0; i < contours.size(); i++)
			convexHull(contours[i], convexHulls[i]);

		drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

		for (auto &convexHull : convexHulls)
		{
			Blob possibleBlob(convexHull);
			possibleBlob.judgeVehicle(possibleBlob);
			if (possibleBlob.blnIsVehicle)
				currentFrameBlobs.push_back(possibleBlob);
		}
		drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

		if (blnFirstFrame == true)
			for (auto &currentFrameBlob : currentFrameBlobs)
				blobs.push_back(currentFrameBlob);
		else
			matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
		drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

		// get another copy of frame 2 since we changed the previous frame 2 copy in the processing above
		imgFrame2Copy = imgFrame2.clone();
		drawBlobInfoOnImage(blobs, imgFrame2Copy);

		bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, intHorizontalLinePosition, carCount);
		if (blnAtLeastOneBlobCrossedTheLine == true)
			line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
		else
			line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
		drawCarCountOnImage(carCount, imgFrame2Copy);
		imshow("imgFrame2Copy", imgFrame2Copy);

		imgCar = imgFrame2Copy.clone();

		// now we prepare for the next iteration
		currentFrameBlobs.clear();
		imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is
		if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT))
		{
			capVideo.read(imgFrame2);
		}
		else
		{
			cout << "end of video\n";
			break;
		}

		blnFirstFrame = false;
		frameCount++;
		chCheckForEscKey = waitKey(1);
	}

	if (chCheckForEscKey != 27)		        // if the user did not press esc (i.e. we reached the end of the video)
		waitKey(1);
}

void Detector::detectingPeople()
{
	char chCheckForEscKey = 0;

	while (capVideo.isOpened() && chCheckForEscKey != 27)
	{
		vector<Blob> currentFrameBlobs;

		Mat imgFrame1Copy = imgFrame1.clone();
		Mat imgFrame2Copy = imgFrame2.clone();
		Mat imgDifference;
		Mat imgThresh;

		cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
		cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);
		GaussianBlur(imgFrame1Copy, imgFrame1Copy, Size(5, 5), 0);
		GaussianBlur(imgFrame2Copy, imgFrame2Copy, Size(5, 5), 0);
		absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);
		threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);
		imshow("imgThresh", imgThresh);

		Mat structuringElement3x3 = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat structuringElement5x5 = getStructuringElement(MORPH_RECT, Size(5, 5));
		Mat structuringElement7x7 = getStructuringElement(MORPH_RECT, Size(7, 7));
		Mat structuringElement15x15 = getStructuringElement(MORPH_RECT, Size(15, 15));

		for (unsigned int i = 0; i < 2; i++)
		{
			dilate(imgThresh, imgThresh, structuringElement5x5);
			dilate(imgThresh, imgThresh, structuringElement5x5);
			erode(imgThresh, imgThresh, structuringElement5x5);
		}

		Mat imgThreshCopy = imgThresh.clone();

		vector<vector<Point> > contours;
		findContours(imgThreshCopy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawAndShowContours(imgThresh.size(), contours, "imgContours");

		vector<vector<Point> > convexHulls(contours.size());
		for (unsigned int i = 0; i < contours.size(); i++)
			convexHull(contours[i], convexHulls[i]);

		drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

		for (auto &convexHull : convexHulls)
		{
			Blob possibleBlob(convexHull);
			possibleBlob.judgePeople(possibleBlob);
			if (possibleBlob.blnIsPeople)
				currentFrameBlobs.push_back(possibleBlob);
		}
		drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

		if (blnFirstFrame == true)
			for (auto &currentFrameBlob : currentFrameBlobs)
				blobs.push_back(currentFrameBlob);
		else
			matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
		drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");

		// get another copy of frame 2 since we changed the previous frame 2 copy in the processing above
		imgFrame2Copy = imgFrame2.clone();
		drawBlobInfoOnImage(blobs, imgFrame2Copy);

		bool blnAtLeastOneBlobCrossedTheLine = checkIfBlobsCrossedTheLine(blobs, intHorizontalLinePosition, peopleCount);
		if (blnAtLeastOneBlobCrossedTheLine == true)
			line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_GREEN, 2);
		else
			line(imgFrame2Copy, crossingLine[0], crossingLine[1], SCALAR_RED, 2);
		drawPeopleCountOnImage(peopleCount, imgFrame2Copy);
		imshow("imgFrame2Copy", imgFrame2Copy);

		imgPeople = imgFrame2Copy.clone();

		// now we prepare for the next iteration
		currentFrameBlobs.clear();
		imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is
		if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT))
		{
			capVideo.read(imgFrame2);
		}
		else
		{
			cout << "end of video\n";
			break;
		}

		blnFirstFrame = false;
		frameCount++;
		chCheckForEscKey = waitKey(0);
	}

	if (chCheckForEscKey != 27)		        // if the user did not press esc (i.e. we reached the end of the video)
		waitKey(1);
}