#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat srcimg;
Mat img;

vector<vector<int>> newPoints;

vector<vector<int>> myColors {{118, 0, 171, 130, 255, 232}, // White
                              {35, 81, 131, 52, 255, 214}}; // Green

vector<Scalar> myColorValues{   {255, 0, 255}, // Purple
                                {0, 255, 0}}; // Green

Point getContours(Mat imgDil)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    Point myPoint(0, 0);

    for(int i = 0; i < contours.size(); i ++)
    {
        int area = contourArea(contours[i]);
        cout << area << endl;
        
        string objectType;

        if(area > 1000) 
        {
            float peri = arcLength(contours[i], true);

            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            cout << conPoly[i].size() << endl;

            boundRect[i] = boundingRect(conPoly[i]);
            myPoint.x = boundRect[i].x + boundRect[i].width / 2;
            myPoint.y = boundRect[i].y;

            drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
        }
    }

    return myPoint;
}

vector<vector<int>> findColor(Mat img)
{
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);
    
    for(int i = 0; i < myColors.size(); i ++)
    {
        Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
        Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);

        Mat mask;
        inRange(imgHSV, lower, upper, mask);

        imshow(to_string(i), mask);
        Point myPoint = getContours(mask);

        if(myPoint.x != 0 && myPoint.y != 0)
            newPoints.push_back({myPoint.x, myPoint.y, i});
    }
    return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
    for(int i = 0; i < newPoints.size(); i ++)
    {
        circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
    }
}


int main()
{
    VideoCapture cap(0);
    
    while(1) {
        cap.read(srcimg);
        Mat resultImage2;
	    flip(srcimg, img, 1);

        newPoints = findColor(img);
        drawOnCanvas(newPoints, myColorValues);

        imshow("Image", img);
        waitKey(1);
    }

    return 0;
}