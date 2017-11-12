#include <opencv2\opencv.hpp>
#include<iostream>
#include "Blob.h"
#include "DetectionTracking.h"
#include "VehicleDetectionTracking.h"
#include "PeopleDetectionTracking.h"
//#include "Detector.h"
using namespace std;
using namespace cv;

int main() 
{
	string videoPath1 = "./CarsDrivingUnderBridge.mp4";
	vehicleDetectionTracking(videoPath1);
	//Detector carDetector(videoPath1);
	//carDetector.detectingCar();

	//string videoPath2 = "768x576.avi";
	//peopleDetectionTracking(vediopath2);
	//Detector peopleDetector(videoPath2);
	//carDetector.detectingPeople();

    return 0;
}