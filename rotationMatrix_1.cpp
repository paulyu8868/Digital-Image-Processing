//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include "opencv2/core/core.hpp" 
//#include "opencv2/highgui/highgui.hpp" 
//#include "opencv2/imgproc/imgproc.hpp" 
//using namespace cv;
//using namespace std;
//
//
//// ���� rotation �Լ�
//void cvRotation() {
//	Mat src = imread("C:/Users/82107/OneDrive/���� ȭ��/2024_1�б�/��ó/Lab/10nd Week Lab Session Materials/10nd Week Lab Session Materials/Lenna.png", 1);
//	Mat dst, matrix;
//
//	cv::Point center = cv::Point(src.cols / 2, src.rows / 2);
//	matrix = getRotationMatrix2D(center, 45.0, 1.0); // angle 45
//	warpAffine(src, dst, matrix, src.size());
//
//	imwrite("nonrot.jpg", src);
//	imwrite("rot.jpg", dst);
//
//	imshow("nonrot", src);
//	imshow("cvrot", dst);
//	waitKey(0);
//
//	destroyAllWindows();
//}
//
//
//// center point �߽����� rotation
//void getMyRotationMatrix(Mat src, double angle=45.0, double scale=1.0) {
//    Mat dst;
//    double degree = angle * CV_PI / 180.0; // radian�� degree�� ��ȯ
//    double a = scale * cos(degree); //cos �� ���
//    double b = scale * sin(degree); //sin �� ���
//
//    cv::Point center(src.cols * 0.5, src.rows * 0.5); // �߽���ǥ Point ����
//    // ȸ����ȯ��� ( 10nd lab ���� )
//    double rot[] = { a, b, (1 - a) * center.x - b * center.y,
//                           -b, a, b * center.x + (1 - a) * center.y };
//    Mat matrix(2, 3, CV_64F, rot);
//
//    // dst Mat�� ȸ����ȯ warping
//    warpAffine(src, dst, matrix, src.size());
//  
//    imwrite("nonrot.jpg", src); 
//    imwrite("rot.jpg", dst); 
//
//    // img show
//    imshow("nonrot", src); 
//    imshow("myrot", dst); 
//    waitKey(0);
//
//    destroyAllWindows();
//}
//
//
//int main() {
//
//    Mat src = imread("C:/Users/82107/OneDrive/���� ȭ��/2024_1�б�/��ó/Lab/10nd Week Lab Session Materials/10nd Week Lab Session Materials/Lenna.png", 1); //�̹��� �б�
//
//    cvRotation();
//    getMyRotationMatrix(src,45,1.5); // default degree=45.0 , default scale=1.0
//
//}