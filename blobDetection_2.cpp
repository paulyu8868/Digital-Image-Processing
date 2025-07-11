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
//// harris cv
//Mat cvHarrisCorner(Mat src_img) {
//	Mat img = src_img;
//	if (img.empty()) {
//		cout << "Empty image!\n";
//		exit(-1);
//	}
//	resize(img, img, Size(500, 500), 0, 0, INTER_CUBIC);
//
//	Mat gray;
//	cvtColor(img, gray, COLOR_BGR2GRAY);
//
//	// <Do Harris corner detection>
//	Mat harr;
//	cornerHarris(gray, harr, 4, 3, 0.04, BORDER_DEFAULT); // blocksize,ksize,k
//	normalize(harr, harr, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
//
//	// < Get abs for Harris visualization>
//	Mat harr_abs;
//	convertScaleAbs(harr, harr_abs);
//
//	// < Print corners >
//	int thresh = 70;
//	Mat result = img.clone();
//	int cnt = 0;
//	for (int y = 0; y < harr.rows; y += 1) {
//		for (int x = 0; x < harr.cols; x += 1) {
//			if ((int)harr.at<float>(y, x) > thresh) {
//				circle(result, Point(x, y), 40, Scalar(0, 255, 0), 0, 4, 0);
//				cnt++;
//			}
//		}
//	}
//	cout<<"감지된 코너의 개수는"<<cnt<<"개 입니다." << endl;
//	/*imshow("Source", img);
//	imshow("Harris", harr_abs);
//	imshow("Target", result);
//	waitKey(0);
//	destroyAllWindows();*/
//	return result;
//
//}
//
//void cvBlobDetection(Mat src_img) {
//	
//	Mat img = cvHarrisCorner(src_img);
//
//	// <set param>
//	SimpleBlobDetector::Params params;
//	params.minThreshold = 10;
//	params.maxThreshold = 300;
//	params.filterByArea = true;
//	params.minArea = 100;
//	params.maxArea = 5000;
//	params.filterByCircularity = true;
//	params.minCircularity = 0.5;
//	params.filterByConvexity = true;
//	params.minConvexity = 0.1;
//	params.filterByInertia = true;
//	params.minInertiaRatio = 0.001;
//
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
//	//imshow("src", src_img);
//	//resize(result, result, Size(500, 500), 0, 0, INTER_CUBIC);
//	imshow("keypoints", result);
//	cout << "이 도형은 : " << keypoints.size()<<"각형입니다." << endl;
//	waitKey(0);
//	destroyAllWindows();
//}
//
//int main() {
//	// 삼각형
//	Mat src_img = imread("C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/9nd Week Lab Session Materials/9nd Week Lab Session/triangle.png");
//	cvBlobDetection(src_img);
//	// 사각형
//	Mat src_img2 = imread("C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/9nd Week Lab Session Materials/9nd Week Lab Session/square.png");
//	cvBlobDetection(src_img2);
//	// 오각형
//	Mat src_img3 = imread("C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/9nd Week Lab Session Materials/9nd Week Lab Session/pentagon.png");
//	cvBlobDetection(src_img3);
//	// 육각형
//	Mat src_img4 = imread("C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/9nd Week Lab Session Materials/9nd Week Lab Session/hexagon.png");
//	cvBlobDetection(src_img4);
//}
//
//
