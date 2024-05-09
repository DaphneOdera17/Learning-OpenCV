#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat img;

void drawMoments(vector<Point> contours)
{
    Moments M;
    M = moments(contours);
    double cX = double(M.m10 / M.m00);
    double cY = double(M.m01 / M.m00);

    circle(img, Point2d(cX, cY), 1, Scalar(0, 255, 0), 2, 8);
    putText(img, "center", Point2d(cX - 20, cY - 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1, 8);
    cout << cX << " " << cY << endl;
}

void getContours(Mat imgDil, Mat img)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

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

            drawMoments(contours[i]);

            cout << conPoly[i].size() << endl;

            boundRect[i] = boundingRect(conPoly[i]);
            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);

            int objCor = (int)conPoly[i].size();

            if(objCor == 3) {
                objectType = "Tri";
            }
            else if(objCor == 4) {
                // objectType = "Rect";
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
}

int main()
{
    string path = "../image/color.jpg";
    img = imread(path, IMREAD_COLOR);

    vector<Mat> channels;
    split(img, channels);

    Mat binaryR, binaryG, binaryB;
    threshold(channels[0], binaryB, 128, 255, THRESH_BINARY);
    threshold(channels[1], binaryG, 128, 255, THRESH_BINARY);
    threshold(channels[2], binaryR, 128, 255, THRESH_BINARY);
    
    Mat subtracted;
    subtract(binaryR, binaryG, subtracted);
    Mat result;
    result = subtracted;

    Mat imgGray, imgBlur, imgCanny, imgDil, imgErode; 

    // Preprocessing
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(imgCanny, imgDil, kernel);

    getContours(imgDil, img);

    // drawMoments();

    imshow("Img", img);
    imshow("Binary Image", result);

    waitKey(0);

    return 0;
}