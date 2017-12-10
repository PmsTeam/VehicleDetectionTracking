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
    /*��Ա����*/
    vector<Point> currentContour;//��ǰ�ſ�������
    vector<Point> centerPositions;//��ǰ�ſ��˶�����
    Rect currentBoundingRect;//��С��Ӿ���
    double dblCurrentDiagonalSize;//�Խ���
    double dblCurrentAspectRatio;//��߱���
    bool blnCurrentMatchFoundOrNewBlob;//�����Ƿ��Ѵ���
    bool blnStillBeingTracked;//�ſ��Ƿ��Ա�׷��
	bool blnIsVehicle;//�ſ��ǳ���
	bool blnIsPeople;//�ſ�������
    int intNumOfConsecutiveFramesWithoutAMatch;//ȫ���µı�������
    Point predictedNextPosition;//Ԥ�����һ��λ��
	/*------------------------------Beta����--------------------------------*/
	double dblCarSpeed;//����

    /*��Ա����*/
    Blob(vector<Point> _contour);//�ſ��ʼ��
    void predictNextPosition(void);//Ԥ���ſ����һ��λ��
	void judgeVehicle(Blob &possibleBlob);//�ж��Ƿ�Ϊ����
	void judgePeople(Blob &possibleBlob);//�ж��Ƿ�Ϊ����
	/*------------------------------Beta����--------------------------------*/
	void evaluateSpeed();//��������
	//vector<double> currentTime;//ʱ���
};

#endif    // MY_BLOB

