#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat imgOriginal, imgGray, imgCanny, img, imgThre, imgBlur, imgDil, imgErode, imgWarp, imgCrop;
vector<Point> initialPoints, docPoints;

float w = 420, h = 596;

Mat preProcessing(Mat img)
{
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(imgCanny, imgDil, kernel);
    // erode(imgDil, imgErode, kernel);

    return imgDil;
}

vector<Point> getContours(Mat imgDil)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point> biggest;
    int maxArea = 0;

    for(int i = 0; i < contours.size(); i ++)
    {
        int area = contourArea(contours[i]);
        cout << area << endl;
        
        string objectType;

        if(area > 1000) 
        {
            float peri = arcLength(contours[i], true);

            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            if(area > maxArea && conPoly[i].size() == 4)
            {
                biggest = {conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3]};
                maxArea = area;
            }

            // drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            // rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
        }
    }

    return biggest;
}
void drawPoints(vector<Point> points, Scalar color)
{
    for(int i = 0; i < points.size(); i ++)
    {
        circle(img, points[i], 10, color, FILLED);
        putText(img, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color, 4);
    }
}

vector<Point> reorder(vector<Point> points)
{
    vector<Point> newPoints;
    vector<int> sumPoints, subPoints;
    
    for(int i = 0; i < 4; i ++)
    {
        sumPoints.push_back(points[i].x + points[i].y);
        subPoints.push_back(points[i].x - points[i].y);
    }    

    int min_index = min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin(); // 0
    int max_index = max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin(); // 3
    int thr_index = min_element(subPoints.begin(), subPoints.end()) - subPoints.begin(); // 2
    int sec_index = max_element(subPoints.begin(), subPoints.end()) - subPoints.begin(); // 1

    newPoints.push_back(points[min_index]);
    newPoints.push_back(points[sec_index]);
    newPoints.push_back(points[thr_index]);
    newPoints.push_back(points[max_index]);

    return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h)
{
    Point2f src[4] = {points[0], points[1], points[2], points[3]};
    Point2f dst[4] = {{0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h}};

    Mat matrix = getPerspectiveTransform(src, dst);
    warpPerspective(img, imgWarp, matrix, Point(w, h));

    return imgWarp;
}

int main()
{
    string path = "../image/paper.jpg";

    img = imread(path);

    // resize(imgOriginal, img, Size(), 0.5, 0.5);

    imgThre = preProcessing(img);

    initialPoints = getContours(imgThre);
    // drawPoints(initialPoints, Scalar(0, 0, 255));
    docPoints = reorder(initialPoints);
    // drawPoints(docPoints, Scalar(0, 255, 0));

    imgWarp = getWarp(img, docPoints, w, h);

    int cropVal = 10;
    Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
    imgCrop = imgWarp(roi);

    // imshow("Image Original", imgOriginal);
    imshow("Image After", img);
    imshow("Image Dil", imgDil);
    imshow("Image Warp", imgWarp);
    imshow("Image Crop", imgCrop);

    waitKey(0);

    return 0;
}