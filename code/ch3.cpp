#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    string path = "../dog.jpeg";
    Mat img = imread(path);
    Mat imgResize;

    cout << img.size() << endl;

    resize(img, imgResize, Size(), 0.5, 0.5);

    imshow("Image", img);
    imshow("Image Resize", imgResize);

    waitKey(0);

    return 0;
}