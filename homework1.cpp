#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat img, imgOrigin, imgHSV, imgMask, imgBinary;
Mat imgGray, imgBlur, imgCanny, imgDil, imgErode; 
Mat blue_mask, green_mask, red_mask, red_mask_1, red_mask_2, blue_res, green_res, red_res;
Mat imgBlue, imgGreen, imgRed;
Mat bg, res;

void preprocessing()
{
    cvtColor(img, imgGray, COLOR_BGR2GRAY); 
    // threshold(imgGray, imgBinary, 128, 255, THRESH_BINARY); 
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(imgCanny, imgCanny, MORPH_CLOSE, kernel);
}

void drawMoments(vector<Point> contours)
{
    Moments M;
    M = moments(contours);
    double cX = double(M.m10 / M.m00);
    double cY = double(M.m01 / M.m00);

    circle(img, Point2d(cX, cY), 1, Scalar(0, 255, 0), 2, 8);
    putText(img, "center", Point2d(cX - 20, cY - 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1, 8);
}

void getContours()
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgCanny, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    for (size_t i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);
        if (area > 1000)
        {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            boundRect[i] = boundingRect(conPoly[i]);

            int objCor = (int)conPoly[i].size();
            string objectType;

            if (objCor == 3)
                objectType = "Triangle";
            else if (objCor == 4)
            {
                float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
                if (aspRatio > 0.95 && aspRatio < 1.05)
                    objectType = "Square";
                else
                    objectType = "Rectangle";
            }
            else
                objectType = "Circle";

            putText(img, objectType, {boundRect[i].x - 10, boundRect[i].y - 5}, FONT_HERSHEY_DUPLEX, 0.5, Scalar(0, 69, 255), 1);
            drawContours(img, contours, i, Scalar(255, 0, 255), 3);
            drawMoments(contours[i]);
        }
    }
}


void getColor()
{
    Scalar lower_blue = Scalar(100, 50, 50);
    Scalar upper_blue = Scalar(124, 255, 255);

    Scalar lower_green = Scalar(35, 50, 50);
    Scalar upper_green = Scalar(77, 255, 255);

    Scalar lower_red_1 = Scalar(0, 50, 50);
    Scalar upper_red_1 = Scalar(10, 255, 255);
    Scalar lower_red_2 = Scalar(156, 50, 50);
    Scalar upper_red_2 = Scalar(180, 255, 255);

    inRange(imgHSV, lower_blue, upper_blue, blue_mask);
    inRange(imgHSV, lower_green, upper_green, green_mask);
    inRange(imgHSV, lower_red_1, upper_red_1, red_mask_1);
    inRange(imgHSV, lower_red_2, upper_red_2, red_mask_2);

    red_mask = red_mask_1 + red_mask_2;

    bitwise_and(imgOrigin, imgOrigin, blue_res, blue_mask);
    bitwise_and(imgOrigin, imgOrigin, green_res, green_mask);
    bitwise_and(imgOrigin, imgOrigin, red_res, red_mask);

    bg = Mat::zeros(1000, 1900, CV_8UC3);

    res = blue_res + green_res + red_res;

    imgBlue = blue_res;
    imgGreen = green_res;
    imgRed = red_res;
}

void markColor()
{
    vector<vector<Point>> contoursRed;
    vector<Vec4i> hierarchyRed;
    findContours(red_mask, contoursRed, hierarchyRed, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    for(int i = 0; i < contoursRed.size(); i ++)
    {
        vector<vector<Point>> conPoly(contoursRed.size());
        vector<Rect> boundRect(contoursRed.size());
        if (contoursRed[i].size() > 0) {
            boundRect[i] = boundingRect(contoursRed[i]);
            putText(img, "Red", {boundRect[i].x - 60, boundRect[i].y + 50}, FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 0, 0), 2);
        }
    }

    vector<vector<Point>> contoursBlue;
    vector<Vec4i> hierarchyBlue;
    findContours(blue_mask, contoursBlue, hierarchyBlue, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    for(int i = 0; i < contoursBlue.size(); i ++)
    {
        vector<vector<Point>> conPoly(contoursBlue.size());
        vector<Rect> boundRect(contoursBlue.size());
        if (contoursBlue[i].size() > 0) {
            boundRect[i] = boundingRect(contoursBlue[i]);
            putText(img, "Blue", {boundRect[i].x - 60, boundRect[i].y + 50}, FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 0, 0), 2);
        }
    }

    vector<vector<Point>> contoursGreen;
    vector<Vec4i> hierarchyGreen;
    findContours(green_mask, contoursGreen, hierarchyGreen, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    for(int i = 0; i < contoursGreen.size(); i ++)
    {
        vector<vector<Point>> conPoly(contoursGreen.size());
        vector<Rect> boundRect(contoursGreen.size());
        if (contoursGreen[i].size() > 0) {
            boundRect[i] = boundingRect(contoursGreen[i]);
            putText(img, "Green", {boundRect[i].x - 60, boundRect[i].y + 50}, FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 0, 0), 2);
        }
    }
}

int main()
{
    string path = "../image/color.jpg";
    imgOrigin = imread(path, IMREAD_COLOR);
    img = imread(path, IMREAD_COLOR);

    preprocessing();

    double tickStart = (double)getTickCount();
    getContours();
    double tickEnd = (double)getTickCount(); 
    
    cout << "time: " << (tickEnd - tickStart) / (getTickFrequency()) << "s" << endl;

    getColor();
    markColor();

    int hmin = 1, smin = 1, vmin = 1;
    int hmax = 179, smax= 255, vmax = 255;
    
    namedWindow("Trackbars", (320, 200));
    createTrackbar("Hue Min", "Trackbars", &hmin, 179);
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 255);
    
    namedWindow("Image", WINDOW_NORMAL);
    imshow("Orginal Image", imgOrigin);
    imshow("res Image", res);
    imshow("Blue Image", imgBlue);
    imshow("Green Image", imgGreen);
    imshow("Red Image", imgRed);
    imshow("Image", img);
    resizeWindow("Image", 1000, 1000);

    while(1)
    {
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(imgHSV, lower, upper, imgMask);

        imshow("Image Mask", imgMask);
        waitKey(1);
    }

    waitKey(0);

    return 0;
}