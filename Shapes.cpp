#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

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
    string path = "../image/shapes.png";

    Mat img = imread(path);
    Mat imgGray, imgBlur, imgCanny, imgDil, imgErode; 

    // Preprocessing
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(imgCanny, imgDil, kernel);

    getContours(imgDil, img);

    imshow("Image", img);
    // imshow("Image Gray", imgGray);
    // imshow("Image Blur", imgBlur);
    // imshow("Image Canny", imgCanny);
    // imshow("Image Dil", imgDil);

    waitKey(0);

    return 0;
}