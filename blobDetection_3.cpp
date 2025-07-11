#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/core/core.hpp" // Mat class�� ���� data structure �� ��� ��ƾ�� �����ϴ� ���
#include "opencv2/highgui/highgui.hpp" // GUI�� ���õ� ��Ҹ� �����ϴ� ���(imshow ��)
#include "opencv2/imgproc/imgproc.hpp" // ���� �̹��� ó�� �Լ��� �����ϴ� ���

using namespace cv;
using namespace std;

void cvFeatureSIFT(Mat src_img) {
    Mat img = src_img;

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Ptr<SiftFeatureDetector> detector = SiftFeatureDetector::create();
	vector<KeyPoint>keypoints;
	detector->detect(gray, keypoints);

	Mat result;
	drawKeypoints(img, keypoints, result,
		Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	
	imwrite("sift_result.jpg",result);
	imshow("Sift result",result);
	waitKey(0);
	destroyAllWindows();
	//cvDestroyAllWindows();
	
}

// ��� ����
Mat valueUp(Mat src_img,int value) {
    // rgb->hsv ��ȯ �� v��(���) ����
    Mat hsv;
    cvtColor(src_img, hsv, COLOR_BGR2HSV); // RGB to HSV
    vector<Mat> channels;
    split(hsv, channels); // h,s,v 3ä�η� �и�
    channels[2] += value; // channel[2] : V(value) (���)
    merge(channels, hsv); // ����
    cvtColor(hsv, src_img, COLOR_HSV2BGR); // HSV to RGB

    return src_img;
    
}

// ���� ��ȯ
Mat warpPersSift(Mat src_img) {
    Mat src = src_img;
    Mat dst;

    Point2f src_p[4], dst_p[4];

    src_p[0] = Point2f(0, 0);
    src_p[1] = Point2f(512, 0);
    src_p[2] = Point2f(0, 512);
    src_p[3] = Point2f(512, 512);

    dst_p[0] = Point2f(0, 0);
    dst_p[1] = Point2f(512, 0);
    dst_p[2] = Point2f(0, 512);
    dst_p[3] = Point2f(412, 412);

    Mat pers_mat = getPerspectiveTransform(src_p, dst_p);
    warpPerspective(src, dst, pers_mat, src.size());
  
    return dst;
}

int main() {
    Mat src_img = imread("C:/Users/82107/OneDrive/���� ȭ��/2024_1�б�/��ó/Lab/9nd Week Lab Session Materials/9nd Week Lab Session/church.jpg", 1);
    imshow("src", src_img);
    cvFeatureSIFT(warpPersSift(src_img));
    cvFeatureSIFT(src_img);
    // ��� ����
    int val = -120; // ��� ������
    Mat val_img = valueUp(src_img, val); // ��� ������ Mat
    //cvFeatureSIFT(val_img);
    // ���� ��ȯ
    Mat wrp_img = warpPersSift(val_img); // ��� ���� �� warp
    imshow("val_wrp", wrp_img);

    waitKey(0);
    destroyAllWindows();

    cvFeatureSIFT(wrp_img);
}