#include <opencv2\opencv.hpp>
#include <iostream>
#include <string.h>
#include<iostream>
#include "Blob.h"
#include "DetectionTracking.h"
#include "VehicleDetectionTracking.h"
#include "PeopleDetectionTracking.h"
//#include "Detector.h"
using namespace std;
using namespace cv;

int main(int argc, char* argv[]) 
{
	string sourcePath = argv[2];
	cout << sourcePath << endl;
	string outputPath;

	for (int i = 0; i < sourcePath.size(); i++)
	{
		outputPath.push_back(sourcePath[i]);
		if (sourcePath[i] == '/' || sourcePath[i] == '\\')
			outputPath.clear();
	}	
	cout << outputPath << endl;
	for (int i = outputPath.size() - 1; i > -1; i--)
	{
		if (outputPath[i] != '.')
		{
			outputPath.pop_back();
			continue;
		}
		outputPath.pop_back();
		outputPath = "./cache/" + outputPath + "-out.avi";
		cout << outputPath << endl;
		break;
	}
	
	if (strcmp(argv[1], "-c") == 0)
	{
		//只检测车
		//string outputPath = "./CarsDrivingUnderBridge.avi";
		vehicleDetectionTracking(sourcePath, outputPath);
	}
	else if (strcmp(argv[1], "-p") == 0)
	{
		//只检测人
		peopleDetectionTracking(sourcePath);
	}
	else if (strcmp(argv[1], "-d") == 0)
	{
		//人车同时检测（可能需要再写一个头文件和源文件，需要测试一下）
	}
	else
	{
		cout << "参数不存在" << endl;
	}
	//Detector carDetector(videoPath1);
	//carDetector.detectingCar();

	//string videoPath2 = "768x576.avi";
	//peopleDetectionTracking(vediopath2);
	//Detector peopleDetector(videoPath2);
	//carDetector.detectingPeople();

    return 0;
}