#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define USE_OPENCV true;

using namespace cv;
using namespace std;

// ---------------------Median-------------------------------

//오름차순 정렬
void bubbleSort(int arr[], int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			// 다음 원소가 더 작을시
			if (arr[j] > arr[j + 1]) {
				// 두 원소를 교환
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}


//3x3 Median 필터
int myMedian3x3(uchar* arr, int x, int y, int width, int height) {
	int med[9] =
	{
		0,0,0,
		0,0,0,
		0,0,0
	};

	for (int j = -1; j <= 1; j++) {
		for (int i = -1; i <= 1; i++) {
			// 이미지를 넘는 범위는 탐색하지 않게 범위 설정.
			if ((y + j) >= 0 && (y + j) < height && (x + i) >= 0 && (x + i) < width) {
				med[3 * (i + 1) + (j + 1)] += arr[(y + j) * width + (x + i)];
			}
		}
	}

	//med 정렬
	
	bubbleSort(med, 9);

	return med[4];
}

//5x5 Median 필터
int myMedian5x5(uchar* arr, int x, int y, int width, int height) {
	int med[25] =
	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,0,0
	};

	for (int j = -2; j <= 2; j++) {
		for (int i = -2; i <= 2; i++) {
			// 이미지를 넘는 범위는 탐색하지 않게 범위 설정.
			if ((y + j) >= 0 && (y + j) < height && (x + i) >= 0 && (x + i) < width) {
				med[3 * (i + 2) + (j + 2)] += arr[(y + j) * width + (x + i)];
			}
		}
	}

	//med 정렬

	bubbleSort(med, 25);

	return med[12];
}



void hw1_median() {
	// 이미지 읽어서 저장
	Mat src_img = imread("C:/Users/82107/OneDrive/바탕 화면/디영처_3_4주/4nd Week Lab Session Materials/salt_pepper2.png", 0);
	// 너비
	int width = src_img.cols;
	// 높이
	int height = src_img.rows;
	// 새로운 이미지를 저장할 MAT 생성
	Mat dst_img(src_img.size(), CV_8UC1);
	// 원본이미지 데이터 (0~255)
	uchar* srcData = src_img.data;
	// 새로운 이미지 데이터
	uchar* dstData = dst_img.data;

	// 원본이미지 데이터에 median 필터 적용
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			dstData[y * width + x] = myMedian3x3(srcData, x, y, width, height);
		}
	}

	//원본이미지 나타내기
	imshow("Src", src_img);
	//필터링이미지 나타내기
	imshow("Dst", dst_img);
	//키 입력대기 (결과 이미지 유지)
	waitKey(0);
	//모든 창 닫기
	destroyAllWindows();
}

void hw1_median_double() {
	// 이미지 읽어서 저장
	Mat src_img = imread("C:/Users/82107/OneDrive/바탕 화면/디영처_3_4주/4nd Week Lab Session Materials/salt_pepper2.png", 0);
	// 너비
	int width = src_img.cols;
	// 높이
	int height = src_img.rows;
	// 새로운 이미지를 저장할 MAT 생성
	Mat dst_img(src_img.size(), CV_8UC1);
	// 원본이미지 데이터 (0~255)
	uchar* srcData = src_img.data;
	// 새로운 이미지 데이터
	uchar* dstData = dst_img.data;

	// 원본이미지 데이터에 median 필터 적용
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			dstData[y * width + x] = myMedian3x3(srcData, x, y, width, height);
		}
	}

	// median filter가 적용된 이미지에 한번더 필터를 적용
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			dstData[y * width + x] = myMedian3x3(dstData, x, y, width, height);
		}
	}

	//원본이미지 나타내기
	imshow("Src", src_img);
	//필터링이미지 나타내기
	imshow("Dst", dst_img);
	//키 입력대기 (결과 이미지 유지)
	waitKey(0);
	//모든 창 닫기
	destroyAllWindows();
}

//-------------------------------Bilateral--------------------

// 1변수 가우시안 
double gaussian(float x, double sigma) {
	return exp(-(pow(x, 2)) / (2 * pow(sigma, 2))) / (2 * CV_PI * pow(sigma, 2));
}

// (x,y) 에서 (i,j) 까지의 거리
float distance(int x, int y, int i, int j) {
	return float(sqrt(pow(x - i, 2) + pow(y - j, 2)));
}


// Bilateral Filter 1픽셀
void bilateral(const Mat& src_img, Mat& dst_img,
	int c, int r, int diameter, double sig_r, double sig_s) {
	// 필터 반경
	int radius = diameter / 2;
	double gr, gs, wei;
	double tmp = 0.0;
	double sum = 0.0;

	// 범위만큼 가우시안 필터 적용
	for (int kc = -radius; kc <= radius; kc++) {
		for (int kr = -radius; kr <= radius; kr++) {
			gr = gaussian((float)src_img.at<uchar>(c + kc, r + kr)
				- (float)src_img.at<uchar>(c, r), sig_r);
			gs = gaussian(distance(c, r, c + kc, r + kr), sig_s);
			// 가중치
			wei = gr * gs;
			tmp += src_img.at<uchar>(c + kc, r + kr) * wei;
			sum += wei;
		}
	}

	dst_img.at<double>(c, r) = tmp / sum; //정규화
}

// 이미지에 Bilateral Filter 적용 
void myBilateral(const Mat& src_img, Mat& dst_img,
	int diameter, double sig_r, double sig_s) {
	dst_img = Mat::zeros(src_img.size(), CV_8UC1);

	Mat guide_img = Mat::zeros(src_img.size(), CV_64F);
	int wh = src_img.cols; int hg = src_img.rows;
	int radius = diameter / 2;

	// 이미지를 넘는 범위는 탐색하지 않게 범위 설정.
	for (int c = radius + 1; c < hg - radius; c++) {
		for (int r = radius + 1; r < wh - radius; r++) {
			bilateral(src_img, guide_img, c, r, diameter, sig_r, sig_s);
		}
	}
	// Mat Type 변환하기.
	guide_img.convertTo(dst_img, CV_8UC1); 
}

void hw2_Bilateral() {

	// 이미지 읽어오기.
	Mat src_img = imread("C:/Users/82107/OneDrive/바탕 화면/디영처_3_4주/4nd Week Lab Session Materials/rock.png", 0);
	// 필터된 이미지를 저장할 Mat 생성.
	Mat dst_img;
	if (!src_img.data) printf("No image data \n");

	// < Bilateral 필터링 수행 >
	myBilateral(src_img, dst_img, 5, 25.0, 50.0);

	// result_img Mat 생성하여 원본과 필터링 된 이미지 합쳐서 저장.
	Mat result_img;
	hconcat(src_img, dst_img, result_img);
	imshow("Bilateral", result_img);
	waitKey(0);
}

//cannyEdge detection
void hw3_CannyEdge() {
	Mat src_img = imread("C:/Users/82107/OneDrive/바탕 화면/디영처_3_4주/4nd Week Lab Session Materials/rock.png", 0);
	Mat dst_img;
#if USE_OPENCV
	Canny(src_img,dst_img,200,300);
#endif
	Mat result_img;
	hconcat(src_img, dst_img, result_img);
	imshow("CannyEdge", result_img);
	waitKey(0);
}

int main() {
	hw3_CannyEdge();

	return 0;
}
