#include <iostream>
#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp" 
using namespace cv;
using namespace std;


Mat GrabCut(Mat img, int x1, int y1, int x2, int y2) {
	// grabcut을 수행할 직사각형 범위
	Rect rect = Rect(Point(x1, y1), Point(x2, y2)); 
	
	Mat result, bg_model, fg_model;
	grabCut(img, result,
		rect,
		bg_model, fg_model,
		5,
		GC_INIT_WITH_RECT);
	
	compare(result, GC_PR_FGD, result, CMP_EQ);
	// GC_PR_FGD : GrabCut class foreground 픽셀
	// CMP_EQ : compare 옵션(equal)

	// 이미지 크기와 동일한 흰색 마스크 Mat 생성
	Mat mask(img.size(), CV_8UC3, cv::Scalar(255, 255, 255)); 
	img.copyTo(mask, result);

	return result;
}

int main() {
	//// ex1 햄스터
	//Mat src_img = imread("C://Users//82107//OneDrive//바탕 화면//8nd Week Lab Session Materials//8nd Week Lab Session Materials//hamster.jpg", 1);
	//Mat dst_img(src_img.rows, src_img.cols, CV_8UC1);
	//dst_img = GrabCut(src_img,374, 76, 802, 433);

	//// ex2 해변
	//Mat src_img = imread("C://Users//82107//OneDrive//바탕 화면//8nd Week Lab Session Materials//8nd Week Lab Session Materials//beach.jpg", 1);
	//Mat dst_img(src_img.rows, src_img.cols, CV_8UC1);
	//dst_img = GrabCut(src_img, 451,281 ,772 , 460);

	// ex3 돼지
	Mat src_img = imread("C://Users//82107//OneDrive//바탕 화면//8nd Week Lab Session Materials//8nd Week Lab Session Materials//pigg.jpg", 1);
	Mat dst_img(src_img.rows, src_img.cols, CV_8UC1);
	dst_img = GrabCut(src_img, 486, 133, 758, 653);

	imshow("Grabcut", dst_img);
	waitKey();
	destroyAllWindows();

	return 0;
}