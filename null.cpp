#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat img;

// 100 50 50 124 255 255    

vector<vector<int>> myColors {{100, 50, 50, 124, 255, 255}, // Blue
                              {35, 81, 131, 52, 255, 214}}; // Green

Point getContours(Mat imgDil)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Point newPoint = {0, 0};

    for(int i = 0; i < contours.size(); i ++)
    {
        int area = contourArea(contours[i]);
        cout << area << endl;

        vector<vector<Point>> conPoly(contours.size());
        vector<Rect> boundRect(contours.size());
        string objectType;

        if(area > 1000) 
        {
            float peri = arcLength(contours[i], true);

            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);

            boundRect[i] = boundingRect(conPoly[i]);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
        
            int objCor = (int)conPoly[i].size();

            if(objCor == 3) {
                objectType = "Tri";
            }
            else if(objCor == 4) {
                float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
                if(aspRatio > 0.95 && aspRatio < 1.05)
                    objectType = "Square";
                else
                    objectType = "Rectangle";
            }
            else {
                objectType = "Circle";
            }

            putText(img, objectType, {boundRect[i].x, boundRect[i].y - 5}, FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 69, 255), 2);
        }
    }

    return newPoint;
}

void findColor(Mat img)
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
    }
}

int main()
{
    VideoCapture cap(0);

    while(1)
    {
        cap.read(img);
        
        findColor(img);

        imshow("Image", img);

        waitKey(1);
    }

    return 0;
}