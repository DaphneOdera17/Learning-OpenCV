#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    string path = "../Megamind.avi";

    VideoCapture cap(path);

    Mat img;

    while(1)
    {
        cap.read(img);
        
        imshow("Image", img);

        waitKey(3);
    }

    return 0;
}