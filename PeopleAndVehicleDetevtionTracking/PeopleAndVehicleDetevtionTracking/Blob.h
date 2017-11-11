// Blob.h

#ifndef MY_BLOB
#define MY_BLOB

#include <opencv2\opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

///////////////////////////////////////////////////////////////////////////////////////////////////
class Blob {
public:
    // member variables ///////////////////////////////////////////////////////////////////////////
    vector<Point> currentContour;
    vector<Point> centerPositions;
    Rect currentBoundingRect;
    double dblCurrentDiagonalSize;
    double dblCurrentAspectRatio;
    bool blnCurrentMatchFoundOrNewBlob;
    bool blnStillBeingTracked;
	bool blnIsVehicle;
	bool blnIsPeople;
    int intNumOfConsecutiveFramesWithoutAMatch;
    Point predictedNextPosition;

    // function prototypes ////////////////////////////////////////////////////////////////////////
    Blob(vector<Point> _contour);
    void predictNextPosition(void);
	void judgeVehicle(Blob &possibleBlob);
	void judgePeople(Blob &possibleBlob);
};

#endif    // MY_BLOB

