//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include "opencv2/core/core.hpp" 
//#include "opencv2/highgui/highgui.hpp" 
//#include "opencv2/imgproc/imgproc.hpp" 
//
//#include <opencv2/objdetect/objdetect.hpp> 
//#include <opencv2/features2d.hpp>
//
//using namespace cv;
//using namespace std;
//using namespace cv;
//using namespace std;
//
//void cvBlobDetection() {
//	Mat img = imread("C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/9nd Week Lab Session Materials/9nd Week Lab Session/coin.png", IMREAD_COLOR);
//
//	// <set param>
//	SimpleBlobDetector::Params params;
//	params.minThreshold = 10;
//	params.maxThreshold = 300;
//	params.filterByArea = true;
//	params.minArea = 100;
//	params.maxArea = 8000;
//	params.filterByCircularity = true;
//	params.minCircularity = 0.3;
//	params.filterByConvexity = true;
//	params.minConvexity = 0.9;
//	params.filterByInertia = true;
//	params.minInertiaRatio = 0.01;
//
//	// <set blob detector>
//	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
//
//	// <Detect blobs>
//	vector<KeyPoint> keypoints;
//	detector->detect(img, keypoints);
//
//	//<Draw blobs>
//	Mat result;
//	drawKeypoints(img, keypoints, result,
//		Scalar(255, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//	imshow("keypoints", result);
//	cout << "동전갯수 : " << keypoints.size() << endl;
//	waitKey(0);
//	destroyAllWindows();
//}
//
//int main() {
//	cvBlobDetection();
//}