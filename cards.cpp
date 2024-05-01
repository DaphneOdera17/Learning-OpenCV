#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{   
    string path = "../image/cards.jpg";

    float w = 250, h = 350;

    Mat img = imread(path);
    Mat matrix, imgWarp;

    Point2f src[4] = {{529, 142}, {779, 190}, {405, 395}, {674, 457}};
    Point2f dst[4] = {{0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h}};

    matrix = getPerspectiveTransform(src, dst);
    warpPerspective(img, imgWarp, matrix, Point(w, h));
 
    for(int i = 0; i < 4; i ++)
    {
        circle(img, src[i], 10, Scalar(0, 0, 255), FILLED);
    }

    imshow("Image", img);
    imshow("Image Warp", imgWarp);

    waitKey(0);

    return 0;
}