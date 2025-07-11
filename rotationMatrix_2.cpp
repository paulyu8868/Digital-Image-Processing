#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
#include <opencv2/objdetect/objdetect.hpp> 
#include <opencv2/features2d.hpp>

using namespace cv;
using namespace std;


void cvHarrisCorner(Mat src_img) {
	Mat img = src_img;
	if (img.empty()) {
		cout << "Empty image!\n";
		exit(-1);
	}
	resize(img, img, Size(500, 500), 0, 0, INTER_CUBIC);

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

    Mat gray2white; // gray scale 한 이미지의 전경을 모두 흰색으로 ( 전경과 배경의 경계선만 corner detection 하기 위함 )
    threshold(gray, gray2white, 40, 255, THRESH_BINARY);

	// <Do Harris corner detection>
	Mat harr;
	cornerHarris(gray2white, harr, 4, 3, 0.05, BORDER_DEFAULT); // blocksize,ksize,k
	normalize(harr, harr, 0, 255, NORM_MINMAX, CV_32FC1, Mat());

	// < Get abs for Harris visualization>
	Mat harr_abs;
	convertScaleAbs(harr, harr_abs);


	// < Print corners >
	int thresh = 100;
	Mat result = img.clone();
	int corner_x=0;
	int corner_y=0;
	for (int y = 0; y < harr.rows; y += 1) {
		for (int x = 0; x < harr.cols; x += 1) {
			if ((int)harr.at<float>(y, x) > thresh) {
				circle(result, Point(x, y), 40, Scalar(0, 255, 0), 0, 4, 0);
				if (corner_x == 0 && corner_y == 0) { // 초기값 할당
					corner_x = x;
					corner_y = y;
					cout << x << " " << y << endl;
				}
				else {
					if (corner_x - 20 > x || x > corner_x + 20) {
						corner_x = x;
						corner_y = y;
						cout << x << " " << y << endl;
					}
				}
			}
		}
	}

	imshow("Source", img);
	imshow("Harris", harr_abs);
	imshow("Target", result);
	waitKey(0);
	destroyAllWindows();
	
}

void cvPerspective1(Mat src_img)
{
	Mat src = src_img;
	Mat dst, matrix;

	// (0,125,134), (2,375,180), (3,425,320), (1,73,352)
	Point2f srcQuad[4];
	srcQuad[0] = Point2f(125, 134);
	srcQuad[1] = Point2f(73, 352);
	srcQuad[2] = Point2f(374, 180);
	srcQuad[3] = Point2f(425, 320);

	Point2f dstQuad[4];
	dstQuad[0] = Point2f(73, 73);
	dstQuad[1] = Point2f(73	, 352);
	dstQuad[2] = Point2f(425, 73);
	dstQuad[3] = Point2f(425, 352);

	matrix = getPerspectiveTransform(srcQuad, dstQuad);
	warpPerspective(src, dst, matrix, src.size());

	imwrite("nonper.jpg", src);
	imwrite("per.jpg", dst);

	imshow("nonper", src);
	imshow("per", dst);
	waitKey(0);

	destroyAllWindows();
}


int main() {
    Mat img = imread("C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/10nd Week Lab Session Materials/10nd Week Lab Session Materials/card_per.png");
    //cvHarrisCorner(img);
    cvPerspective1(img);
  
}