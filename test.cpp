#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    /* Importing Images */
    string path = "../kaola.jpg";
    Mat img = imread(path);
    Mat imgCrop;
    
    cout << img.size() << endl;
    Rect roi(300, 300, 250, 150);

    imgCrop = img(roi);

    imshow("Image", img);
    imshow("Image Crop", imgCrop);

    waitKey(0);
}