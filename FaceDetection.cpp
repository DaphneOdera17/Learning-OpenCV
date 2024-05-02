#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    string path = "../image/test.png";

    Mat img = imread(path);

    CascadeClassifier faceCascade;
    faceCascade.load("../haarcascade_frontalface_default.xml");

    if(faceCascade.empty())
        puts("None!");
    
    vector<Rect> faces;
    faceCascade.detectMultiScale(img, faces, 1.1, 10);

    for(int i = 0; i < faces.size(); i ++)
    {
        rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);
    }

    imshow("Image", img);
    waitKey(0);

    return 0;
}