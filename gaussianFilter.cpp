#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int KernelConv_3x3(uchar* arr, int kernel[][3], int x, int y, int w, int h) {
	int sum = 0;
	int sumKernel = 0;

	for (int j = -1; j <= 1; j++) {
		for (int i = -1; i <= 1; i++) {
			// 크기를 벗어나지 않게 범위 설정
			if ((y + j) >= 0 && (y + j) < h && (x + i) >= 0 && (x + i) < w) {
				sum += arr[(y + j) * w + (x + i)] * kernel[i + 1][j + 1];
				sumKernel += kernel[i + 1][j + 1];
			}
		}
	}

	if (sumKernel != 0)
		return sum / sumKernel;
	else
		return sum;
}

int KernelConv_9x9(uchar* arr,int kernel[][9],int x,int y,int w,int h) {
	int sum = 0;
	int sumKernel = 0;

	for (int j = -4; j <= 4; j++) {
		for (int i = -4; i <= 4; i++) {
			// 크기를 벗어나지 않게 범위 설정
			if ((y + j) >= 0 && (y + j) < h && (x + i) >= 0 && (x + i) < w) {
				sum += arr[(y + j) * w + (x + i)] * kernel[i + 4][j + 4];
				sumKernel += kernel[i + 4][j + 4];
			}
		}
	}

	if (sumKernel != 0)
		return sum / sumKernel;
	else
		return sum;
}

Mat GaussianFilter_9x9(Mat srcImg) {
	// w:너비 , h:높이
	int w = srcImg.cols;
	int h = srcImg.rows;
	// 9x9 가우시안 필터링 마스크
	int kernel[9][9] =
	{ 1,4,7,10,7,4,1,
		4,16,26,34,26,16,4,
		7,26,41,52,41,26,7,
		10,34,51,64,52,34,10,
		16,40,59,70,59,40,13,
		10,34,51,64,52,34,10,
		7,26,41,52,41,26,7,
		4,16,26,34,26,16,4,
		1,4,7,10,7,4,1
	};

	Mat dstImg(srcImg.size(), CV_8UC1);
	uchar* srcData = srcImg.data;
	uchar* dstData = dstImg.data;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			dstData[(y * w) + x] = KernelConv_9x9(srcData, kernel, x, y, w, h);
		}
	}

	return dstImg;
}

//---------------------------#2-----------------------------------
Mat GetHistogram(Mat& src) {
	Mat histogram;

	const int* channel_numbers = { 0 };
	float channel_range[] = { 0.0, 255.0 }; //채널 범위를 0~255
	const float* channel_ranges = channel_range;
	int number_bins = 255;

	//히스토그램 계산
	calcHist(&src, 1, channel_numbers, Mat(), histogram, 1, &number_bins, &channel_ranges);

	//히스토그램 plot
	int hist_w = src.cols;
	int hist_h = src.rows;
	int bin_w = cvRound((double)hist_w / number_bins);

	Mat hist_img(hist_h, hist_w, CV_8UC1, Scalar::all(0)); //0~255 : 8비트 , 채널이 1개로 흑백
	normalize(histogram, histogram, 0, hist_img.rows, NORM_MINMAX, -1, Mat()); //정규화

	for (int i = 1; i < number_bins; i++)
	{
		line(hist_img, Point(bin_w * (i - 1), hist_h - cvRound(histogram.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(histogram.at<float>(i))),
			Scalar(255, 0, 0), 1, 8, 0); //값과 값을 잇는 선을 그리는 방식으로 plot
	}
	return  hist_img;
}

//---------------------------#3-----------------------------------
void saltAndpepper(Mat img, int num) {
	// 랜덤한 분포를 위해 난수 생성
	srand((unsigned int)time(NULL));
	for (int n = 0; n < num; n++) {
		int x = rand() % img.cols;
		int y = rand() % img.rows;
		int i = rand() % 2; // 흑:0 , 백:1
		if (img.channels() == 1) { 
			if (i == 1) { // 흰색점 할당
				img.at<uchar>(y, x) = 255;
			}
			else // 그 외 검은점 할당
			{
				img.at<uchar>(y, x) = 0;
			}
		}
	}

}
//----------------4-----------------------------------
Mat mysobelFilter(Mat srcImg) {
	Mat dstImg(srcImg.size(), CV_8UC1);
	uchar* srcData = srcImg.data;
	uchar* dstData = dstImg.data;
	int width = srcImg.cols;
	int height = srcImg.rows;

	// 45도 마스크
	int kernelX[3][3] = { 0, 1 ,2,
						 -1, 0, 1,
						 -2, -1, 0 };
	// 135도 마스크
	int kernelY[3][3] = { -2,-1,0, 
						 -1, 0, 1,
						  0, 1, 2 };

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// 두 에지 결과 값을 절대값의 합 형태로
			dstData[y * width + x] = (abs(KernelConv_3x3(srcData, kernelX, x, y, width, height)) +
				abs(KernelConv_3x3(srcData, kernelY, x, y, width, height))) / 2;
		}
	}
	return dstImg;
}

//--------------------------------5------------------------------
Mat mySampling(Mat srcImg) {
	int width = srcImg.cols / 2;
	int height = srcImg.rows / 2;
	Mat dstImg(height, width, CV_8UC3); // CV_8UC3 : 컬러 이미지 
	uchar* srcData = srcImg.data;
	uchar* dstData = dstImg.data;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// for문으로 각각의 채널에 대한 계산 처리
			for (int c = 0; c < 3; c++) { 
				dstData[(y * width + x) * 3 + c] = srcData[((y * 2) * (width * 2) + (x * 2)) * 3 + c];
			}
		}
	}
	return dstImg;
}


int kernelconv_3x3_color(uchar* arr, int kernel[][3], int x, int y, int width, int height, int channel) {
//컬러영상으로 변환해야하기 때문에 매개변수에 channel을 추가하였다 
    int sum = 0;
    int sumKernel = 0;
    for (int j = -1; j <= 1; j++) {//3x3로 하기위해선 범위로 -1~1로 해주어야한다.
        for (int i = -1; i <= 1; i++) {
            //3x3로 하기위해선 범위로 -1~1로 해주어야한다.
            if ((y + j) >= 0 && (y + j) < height && (x + i) >= 0 && (x + i) < width) {
                sum += arr[(y + j) * width * 3 + (x + i) * 3 + channel] * kernel[i + 1][j + 1];
                sumKernel += kernel[i + 1][j + 1];
                //컬러로 변환하기 위해 3씩 곱해 늘려주었다

            }
        }
    }
    if (sumKernel != 0) { return sum / sumKernel; } // 정규화
    else { return sum; }
}

Mat GaussianFilter_3x3(Mat srcImg) {
	// w:너비 , h:높이
	int w = srcImg.cols;
	int h = srcImg.rows;
	// 3x3 가우시안 필터링 마스크
	int kernel[3][3] =
	{ 1,2,1,
	2,4,2,
 	1,2,1
	};

	
	Mat dstImg(srcImg.size(), CV_8UC3); // 컬러 이미지 : CV_8UC3
	uchar* srcData = srcImg.data;
	uchar* dstData = dstImg.data;

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			for (int c = 0; c < 3; c++) {
				dstData[(y * w + x) * 3 + c] = kernelconv_3x3_color(srcData+c, kernel, x, y, w, h,c);
			}
		}
	}
	return dstImg;
}


vector<Mat> myGaussainPyramid(Mat srcImg) {
	vector<Mat> Vec;//여러 영상을 모아서 저장하기 위해 STL의 vector컨테이너 사용

	Vec.push_back(srcImg);
	for (int i = 0; i < 4; i++) {
		srcImg = mySampling(srcImg); // 앞서 구현한 down sampling
		srcImg = GaussianFilter_3x3(srcImg); //vector컨테이너에 하나씩 처리결과를 삽입
		Vec.push_back(srcImg);
	}
	return Vec;
}


int main() {
	Mat srcImg = imread("C:/Users/82107/OneDrive/바탕 화면/디영처_3_4주/4nd Week Lab Session Materials/gear.jpg", 0);
	
	////#1 (9x9 가우시안)
	//Mat dstImg = GaussianFilter_9x9(srcImg);
	//imshow("Src", srcImg);
	//imshow("Dst", dstImg);
	//waitKey(0);
	//destroyAllWindows();

	////#2 (Histogram)
	//Mat dstImg = GaussianFilter_9x9(srcImg); // 가우시안 필터링 된 이미지
	//Mat src_hist = GetHistogram(srcImg); // 원본 히스토그램 MAT
	//Mat dst_hist = GetHistogram(dstImg); // 가우시안 히스토그램 MAT
	//imshow("Src", srcImg); // 원본
	//imshow("Src_histogram", src_hist); // 원본 히스토그램
	//imshow("Dst", dstImg); // 가우시안 필터링 된 이미지
	//imshow("Dst_histogram", dst_hist); // 가우시안 히스토그램
	//waitKey(0);
	//destroyAllWindows();
	
	//#3 (Salt&Pepper)
	//saltAndpepper(srcImg, 5000);
	//Mat GSF_salt = GaussianFilter_9x9(srcImg);
	//imshow("Src_Salted", srcImg);
	//imshow("Filtered", GSF_salt);
	//waitKey(0);
	//destroyAllWindows();

	////#4 (Sobel Filter)
	//Mat sobel = mysobelFilter(srcImg);
	//imshow("src", srcImg);
	//imshow("sobel filtered", sobel);
	//waitKey(0);
	//destroyAllWindows();

	// #5 (colored GP)
	// color로 영상을 받기 위해 flag=1
	Mat srcImg_colored = imread("C:/Users/82107/OneDrive/바탕 화면/디영처_3_4주/4nd Week Lab Session Materials/gear.jpg", 1);

	vector<Mat> GauPyr = myGaussainPyramid(srcImg_colored);//가우시안 피라미드 적용한 변수

	imshow("GauPyr0", GauPyr[0]); //가우시안 피라미드를 서로 다른 창으로 하여 출력
	imshow("GauPyr1", GauPyr[1]); //큰 창부터 출력된다
	imshow("GauPyr2", GauPyr[2]);
	imshow("GauPyr3", GauPyr[3]);
	imshow("GauPyr4", GauPyr[4]);

	waitKey(0);
	destroyAllWindows();

	return 0;

}
