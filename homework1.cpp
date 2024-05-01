#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    string path = "../image/color.jpg";
    Mat img = imread(path, IMREAD_COLOR);

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

    imshow("Img", img);
    imshow("Binary Image", result);

    waitKey(0);

    return 0;
}