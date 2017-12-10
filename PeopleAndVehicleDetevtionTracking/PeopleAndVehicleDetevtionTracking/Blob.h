// Blob.h

#ifndef MY_BLOB
#define MY_BLOB

#include <opencv2\opencv.hpp>
#include <vector>
#include <windows.h> 
#include <stdio.h> 
using namespace std;
using namespace cv;

class Blob {
public:
    /*成员变量*/
    vector<Point> currentContour;//当前团块轮廓集
    vector<Point> centerPositions;//当前团块运动质心
    Rect currentBoundingRect;//最小外接矩形
    double dblCurrentDiagonalSize;//对角线
    double dblCurrentAspectRatio;//宽高比例
    bool blnCurrentMatchFoundOrNewBlob;//轮廓是否已存在
    bool blnStillBeingTracked;//团块是否仍被追踪
	bool blnIsVehicle;//团块是车辆
	bool blnIsPeople;//团块是行人
    int intNumOfConsecutiveFramesWithoutAMatch;//全局下的被检测次数
    Point predictedNextPosition;//预测的下一个位置
	/*------------------------------Beta新增--------------------------------*/
	double dblCarSpeed;//车速

    /*成员函数*/
    Blob(vector<Point> _contour);//团块初始化
    void predictNextPosition(void);//预测团块的下一个位置
	void judgeVehicle(Blob &possibleBlob);//判断是否为车辆
	void judgePeople(Blob &possibleBlob);//判断是否为行人
	/*------------------------------Beta新增--------------------------------*/
	void evaluateSpeed();//估测速率
	//vector<double> currentTime;//时间戳
};

#endif    // MY_BLOB

