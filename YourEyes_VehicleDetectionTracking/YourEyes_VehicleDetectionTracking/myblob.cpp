#include "myblob.h"

Blob::Blob(vector<Point> _contours)
{
	currentContour = _contours;
	currentBoundingRect = boundingRect(currentContour);

	Point currentCenter;
	currentCenter.x = (currentBoundingRect.x + currentBoundingRect.x + currentBoundingRect.width) / 2;
	currentCenter.y = (currentBoundingRect.y + currentBoundingRect.y + currentBoundingRect.height) / 2;
	centerPositions.push_back(currentCenter);

	currentAspectRatio = sqrt(pow(currentBoundingRect.width, 2) 
							+ pow(currentBoundingRect.height, 2));
	currentAspectRatio = (float)currentBoundingRect.width / currentBoundingRect.height;
	stillBeingTracked = true;
	currentMatchFoundOrNewBlob = true;
	numOfConsecutiveFramesWithoutAMatch = 0;
}

void Blob::predictNextPosition()
{
	int positionNum = (int)centerPositions.size();

	if (positionNum == 1)
	{
		predictedNextPosition.x = centerPositions.back().x;
		predictedNextPosition.y = centerPositions.back().y;
	}
	else if (positionNum == 2)
	{
		int dx = centerPositions[1].x - centerPositions[0].x;
		int dy = centerPositions[1].y - centerPositions[0].y;
		predictedNextPosition.x = centerPositions.back().x + dx;
		predictedNextPosition.y = centerPositions.back().y + dy;
	}
	else if (positionNum > 2 || positionNum < 6)
	{
		int xChangeSum;
		int yChangeSum;
		for (int i = positionNum - 1; i > -1; i--)
		{
			xChangeSum += (centerPositions[i].x - centerPositions[i - 1].x) * (i - 1);
			yChangeSum += (centerPositions[i].y - centerPositions[i - 1].y) * (i - 1);
		}
		//取窗口均值？
		//round()获得浮点数最近的整数，对0.5向下舍去
		int index = (positionNum * (positionNum - 1)) / 2;
		int dx = (int)round((float)xChangeSum / index);
		int dy = (int)round((float)yChangeSum / index);
		predictedNextPosition.x = centerPositions.back().x + dx;
		predictedNextPosition.y = centerPositions.back().y + dy;
	}
	else
		return;//超出搜索窗口
}

void Blob::judgeVehicle()
{

}