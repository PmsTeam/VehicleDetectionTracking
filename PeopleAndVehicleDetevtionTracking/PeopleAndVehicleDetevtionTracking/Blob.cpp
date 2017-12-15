#include "Blob.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//extern vector<double> frameTime;
Blob::Blob(vector<Point> _contour) 
{
	currentContour = _contour;
    currentBoundingRect = boundingRect(currentContour);
 
	Point currentCenter;
    currentCenter.x = (currentBoundingRect.x + currentBoundingRect.x + currentBoundingRect.width) / 2;
    currentCenter.y = (currentBoundingRect.y + currentBoundingRect.y + currentBoundingRect.height) / 2;
    centerPositions.push_back(currentCenter);

    dblCurrentDiagonalSize = sqrt(pow(currentBoundingRect.width, 2) + pow(currentBoundingRect.height, 2));
    dblCurrentAspectRatio = (float)currentBoundingRect.width / (float)currentBoundingRect.height;

    blnStillBeingTracked = true;
    blnCurrentMatchFoundOrNewBlob = true;
	blnIsVehicle = false;
	blnIsPeople = false;

	/*------------------------------Beta新增--------------------------------*/
	dblCarSpeed = 0;
	//currentTime.push_back(frameTime.back());

    intNumOfConsecutiveFramesWithoutAMatch = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Blob::predictNextPosition(void) 
{
    int numPositions = (int)centerPositions.size();

	if (numPositions == 1)
	{
		predictedNextPosition.x = centerPositions.back().x;
		predictedNextPosition.y = centerPositions.back().y;

	}
	else if (numPositions == 2)
	{
		int deltaX = centerPositions[1].x - centerPositions[0].x;
		int deltaY = centerPositions[1].y - centerPositions[0].y;

		predictedNextPosition.x = centerPositions.back().x + deltaX;
		predictedNextPosition.y = centerPositions.back().y + deltaY;

	}
	else if (numPositions == 3)
	{
		int sumOfXChanges = ((centerPositions[2].x - centerPositions[1].x) * 2)
			+ ((centerPositions[1].x - centerPositions[0].x) * 1);
		int deltaX = (int)round((float)sumOfXChanges / 3.0);

		int sumOfYChanges = ((centerPositions[2].y - centerPositions[1].y) * 2)
			+ ((centerPositions[1].y - centerPositions[0].y) * 1);
		int deltaY = (int)round((float)sumOfYChanges / 3.0);

		predictedNextPosition.x = centerPositions.back().x + deltaX;
		predictedNextPosition.y = centerPositions.back().y + deltaY;
	}
	else if (numPositions == 4)
	{
		int sumOfXChanges = ((centerPositions[3].x - centerPositions[2].x) * 3)
			+ ((centerPositions[2].x - centerPositions[1].x) * 2)
			+ ((centerPositions[1].x - centerPositions[0].x) * 1);
		int deltaX = (int)round((float)sumOfXChanges / 6.0);

		int sumOfYChanges = ((centerPositions[3].y - centerPositions[2].y) * 3)
			+ ((centerPositions[2].y - centerPositions[1].y) * 2)
			+ ((centerPositions[1].y - centerPositions[0].y) * 1);
		int deltaY = (int)round((float)sumOfYChanges / 6.0);

		predictedNextPosition.x = centerPositions.back().x + deltaX;
		predictedNextPosition.y = centerPositions.back().y + deltaY;
	}
	else if (numPositions >= 5)
	{
		int sumOfXChanges = ((centerPositions[numPositions - 1].x - centerPositions[numPositions - 2].x) * 4)
			+ ((centerPositions[numPositions - 2].x - centerPositions[numPositions - 3].x) * 3)
			+ ((centerPositions[numPositions - 3].x - centerPositions[numPositions - 4].x) * 2)
			+ ((centerPositions[numPositions - 4].x - centerPositions[numPositions - 5].x) * 1);
		int deltaX = (int)round((float)sumOfXChanges / 10.0);

		int sumOfYChanges = ((centerPositions[numPositions - 1].y - centerPositions[numPositions - 2].y) * 4)
			+ ((centerPositions[numPositions - 2].y - centerPositions[numPositions - 3].y) * 3)
			+ ((centerPositions[numPositions - 3].y - centerPositions[numPositions - 4].y) * 2)
			+ ((centerPositions[numPositions - 4].y - centerPositions[numPositions - 5].y) * 1);
		int deltaY = (int)round((float)sumOfYChanges / 10.0);

		predictedNextPosition.x = centerPositions.back().x + deltaX;
		predictedNextPosition.y = centerPositions.back().y + deltaY;
	}
	else
		return;// should never get here
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Blob::judgeVehicle(Blob &possibleBlob)
{
	if (possibleBlob.currentBoundingRect.area() > 400 &&
		possibleBlob.dblCurrentAspectRatio > 0.2 &&
		possibleBlob.dblCurrentAspectRatio < 4.0 &&
		possibleBlob.currentBoundingRect.width > 30 &&
		possibleBlob.currentBoundingRect.height > 30 &&
		possibleBlob.dblCurrentDiagonalSize > 60.0 &&
		(contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50)
		possibleBlob.blnIsVehicle = true;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Blob::judgePeople(Blob &possibleBlob)
{
	if (possibleBlob.currentBoundingRect.area() > 800 &&
		possibleBlob.dblCurrentAspectRatio > 0.35 && possibleBlob.dblCurrentAspectRatio < 0.80 &&
		possibleBlob.currentBoundingRect.width > 20 &&
		possibleBlob.currentBoundingRect.height > 35 &&
		possibleBlob.dblCurrentDiagonalSize > 35 &&
		(contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.30)
		possibleBlob.blnIsPeople = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Blob::DjudgeVehicle(Blob &possibleBlob)
{
	if (possibleBlob.currentBoundingRect.area() > 7000 &&
		possibleBlob.dblCurrentAspectRatio > 1.5 &&
		possibleBlob.dblCurrentAspectRatio < 3.0 &&
		possibleBlob.currentBoundingRect.width > 95 &&
		possibleBlob.currentBoundingRect.height > 50 &&
		possibleBlob.dblCurrentDiagonalSize > 95 &&
		(contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.30)
		possibleBlob.blnIsVehicle = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Blob::DjudgePeople(Blob &possibleBlob)
{
	if (possibleBlob.currentBoundingRect.area() > 1000 &&
		possibleBlob.dblCurrentAspectRatio > 0.30 && possibleBlob.dblCurrentAspectRatio < 0.70 &&
		possibleBlob.currentBoundingRect.width > 20 &&
		possibleBlob.currentBoundingRect.height > 55 &&
		possibleBlob.dblCurrentDiagonalSize > 55 &&
		(contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.30)
		possibleBlob.blnIsPeople = true;
}


/*------------------------------Beta新增--------------------------------*/

///////////////////////////////////////////////////////////////////////////////////////////////////
void Blob::evaluateSpeed()
{
	int i = centerPositions.size();//质心数量
	if (blnStillBeingTracked == true && i >= 2)
	{
		//获得位移量
		//double dx = centerPositions[i - 1].x - centerPositions[i - 2].x;
		//double dy = centerPositions[i - 1].y - centerPositions[i - 2].y;
		//double ds = sqrt(dx*dx + dy*dy) * 16.86;
		//double dt = 33.3667;
		double x1 = centerPositions[i - 1].x;
		double x2 = centerPositions[i - 2].x;
		double dx = x1 - x2;
		double y1 = centerPositions[i - 1].y;
		double y2 = centerPositions[i - 2].y;
		double dy = y1 - y2;

		double ds = sqrt(dx*dx + dy*dy) * 350.00 * ((720 - y1) / 720) * ((720 - y1) / 720);//./CarsDrivingUnderBridge.mp4
		//double ds = sqrt(dx*dx + dy*dy) * 69.40;//./DSC_0068.mp4

		double dt = 33.3667;
		dblCarSpeed = ds / dt;
	}
}