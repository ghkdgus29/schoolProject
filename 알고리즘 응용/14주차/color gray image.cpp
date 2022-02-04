#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

void main() {
	Mat imgColor = imread("test.jpg", 1);
	Mat imgGray = imread("test.jpg", 0);

	imshow("color", imgColor);
	imshow("gray", imgGray);

	waitKey(5000);
}