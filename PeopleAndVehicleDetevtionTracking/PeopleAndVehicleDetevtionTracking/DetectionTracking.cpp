#include "DetectionTracking.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(vector<Blob> &existingBlobs, vector<Blob> &currentFrameBlobs) 
{
	for (auto &existingBlob : existingBlobs) 
	{
		existingBlob.blnCurrentMatchFoundOrNewBlob = false;

		existingBlob.predictNextPosition();
	}

	for (auto &currentFrameBlob : currentFrameBlobs) 
	{
		int intIndexOfLeastDistance = 0;
		double dblLeastDistance = 100000.0;

		for (unsigned int i = 0; i < existingBlobs.size(); i++) 
		{
			if (existingBlobs[i].blnStillBeingTracked == true) 
			{
				double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

				if (dblDistance < dblLeastDistance)
				{
					dblLeastDistance = dblDistance;
					intIndexOfLeastDistance = i;
				}
			}
		}

		if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 0.5)
			addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
		else
			addNewBlob(currentFrameBlob, existingBlobs);
	}

	for (auto &existingBlob : existingBlobs) 
	{
		if (existingBlob.blnCurrentMatchFoundOrNewBlob == false)
			existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;

		if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5)
			existingBlob.blnStillBeingTracked = false;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//extern vector<double> frameTime;
void addBlobToExistingBlobs(Blob &currentFrameBlob, vector<Blob> &existingBlobs, int &intIndex)
{
	existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
	existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;

	existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());

	existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
	existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;

	existingBlobs[intIndex].blnStillBeingTracked = true;
	existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;

	/*------------------------------BetaÐÂÔö--------------------------------*/
	//existingBlobs[intIndex].currentTime.push_back(frameTime.back());
	existingBlobs[intIndex].evaluateSpeed();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addNewBlob(Blob &currentFrameBlob, vector<Blob> &existingBlobs) 
{
	currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;
	existingBlobs.push_back(currentFrameBlob);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double distanceBetweenPoints(Point point1, Point point2) 
{
	int intX = abs(point1.x - point2.x);
	int intY = abs(point1.y - point2.y);

	return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(Size imageSize, vector<vector<Point> > contours, string strImageName) 
{
	Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

	drawContours(image, contours, -1, SCALAR_WHITE, -1);

	imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(Size imageSize, vector<Blob> blobs, string strImageName) 
{
	Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

	vector<vector<Point> > contours;

	for (auto &blob : blobs) 
	{
		if (blob.blnStillBeingTracked == true)
			contours.push_back(blob.currentContour);
	}

	drawContours(image, contours, -1, SCALAR_WHITE, -1);

	imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedTheLine(vector<Blob> &blobs, int &intHorizontalLinePosition, int &carCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;

	for (auto blob : blobs)
	{
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2)
		{
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;

			if (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition
				&& blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition)
			{
				carCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
			}
		}
	}

	return blnAtLeastOneBlobCrossedTheLine;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawBlobInfoOnImage(vector<Blob> &blobs, Mat &imgFrame2Copy) 
{
	for (unsigned int i = 0; i < blobs.size(); i++) 
	{
		if (blobs[i].blnStillBeingTracked == true) 
		{
			rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_RED, 2);

			int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
			double dblFontScale = blobs[i].dblCurrentDiagonalSize / 120.0;
			int intFontThickness = (int)round(dblFontScale * 2.0);

			//putText(imgFrame2Copy, to_string(i), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);

			string strSpeed = to_string(blobs[i].dblCarSpeed);
			string subString = strSpeed.substr(0, strSpeed.size() - 5);
			putText(imgFrame2Copy, subString, blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawCarCountOnImage(int &carCount, Mat &imgFrame2Copy) {

	int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
	double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
	int intFontThickness = (int)round(dblFontScale * 1.5);

	Size textSize = getTextSize(to_string(carCount), intFontFace, dblFontScale, intFontThickness, 0);

	Point ptTextBottomLeftPosition;

	ptTextBottomLeftPosition.x = imgFrame2Copy.cols - 1 - (int)((double)textSize.width * 1.25);
	ptTextBottomLeftPosition.y = (int)((double)textSize.height * 1.25);

	putText(imgFrame2Copy, to_string(carCount), ptTextBottomLeftPosition, intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawPeopleCountOnImage(int &peopleCount, Mat &imgFrame2Copy) {

	int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
	double dblFontScale = (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
	int intFontThickness = (int)round(dblFontScale * 1.5);

	Size textSize = getTextSize(to_string(peopleCount), intFontFace, dblFontScale, intFontThickness, 0);

	Point ptTextBottomLeftPosition;

	ptTextBottomLeftPosition.x = imgFrame2Copy.cols - 1 - (int)((double)textSize.width * 1.25);
	ptTextBottomLeftPosition.y = (int)((double)textSize.height * 1.25);

	putText(imgFrame2Copy, to_string(peopleCount), ptTextBottomLeftPosition, intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);

}