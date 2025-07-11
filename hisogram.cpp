#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
using namespace cv;
using namespace std;


void GetHistogram(Mat src) {
	Mat gray;
	cvtColor(src, gray, COLOR_BGR2GRAY);

	Mat histogram;
	const int* channel_numbers = { 0 };
	float channel_range[] = { 0.0, 255.0 };
	const float* channel_ranges = channel_range;
	int number_bins = 255;

	// 히스토그램 계산
	calcHist(&gray, 1, channel_numbers, Mat(), histogram, 1, &number_bins, &channel_ranges);

	// 히스토그램 plot
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / number_bins);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
	normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat()); // 정규화

	for (int i = 1; i < number_bins; i++) {
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(histogram.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(histogram.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0); // 값과 값을 잇는 선을 그리는 방식으로 plot
	}
	imshow("hist_img", histImage);
	waitKey(0);  // 키 입력 대기
	destroyAllWindows();  // 모든 윈도우 제거

}

// 이미지와 노출 시간을 읽어오는 함수
void readImagesAndTimes(vector<Mat>& images, vector<float>& times) {
	int numImages = 4;
	static const float timesArray[] = { 1 / 30.0f, 0.25f, 2.5f, 15.0f };
	times.assign(timesArray, timesArray + numImages);  // 노출 시간 설정
	static const char* filenames[] = {
		"C:/Users/82107/OneDrive/바탕 화면/디영처마지막/60by1.jpg",
		"C:/Users/82107/OneDrive/바탕 화면/디영처마지막/30by1.jpg",
		"C:/Users/82107/OneDrive/바탕 화면/디영처마지막/15by1.jpg" ,
		"C:/Users/82107/OneDrive/바탕 화면/디영처마지막/1.jpg" ,
	};
	/*static const char* filenames[] = {
		"C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/11nd Week Lab Session Materials (2) (1)/img_0.033.jpg",
		"C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/11nd Week Lab Session Materials (2) (1)/img_0.25.jpg",
		"C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/11nd Week Lab Session Materials (2) (1)/img_2.5.jpg" ,
		"C:/Users/82107/OneDrive/바탕 화면/2024_1학기/디영처/Lab/11nd Week Lab Session Materials (2) (1)/img_15.jpg" ,
		 };*/
	for (int i = 0; i < numImages; i++) {
		Mat im = imread(filenames[i]);  // 이미지를 읽어옴
		if (im.empty()) {
			cout << "Could not open or find the image: " << filenames[i] << endl;
			return;
		}
		images.push_back(im);  // 읽어온 이미지를 벡터에 추가
	}
}

// 이미지 크기를 조정하여 보여주는 함수
void showResized(const string& windowName, const Mat& img, int width = 1280, int height = 720) {
	Mat resizedImg;
	resize(img, resizedImg, Size(width, height));  // 이미지 크기 조정
	namedWindow(windowName, WINDOW_AUTOSIZE);  // 윈도우 생성
	GetHistogram(resizedImg);
	imshow(windowName, resizedImg);  // 이미지 보여줌
}

// 예제 1: 이미지와 노출 시간 읽기 및 디스플레이
void ex1() {
	cout << "Reading images and exposure times ..." << endl;
	vector<Mat> images;
	vector<float> times;
	readImagesAndTimes(images, times);  // 이미지와 노출 시간 읽기
	cout << "Finished" << endl;

	// 이미지를 디스플레이하여 올바르게 로드되었는지 확인
	for (size_t i = 0; i < images.size(); i++) {
		string windowName = "Image " + to_string(i + 1);
		showResized(windowName, images[i]);
		GetHistogram(images[i]);
		cout << "Exposure time for image " << i + 1 << ": " << times[i] << " seconds" << endl;
	}
	waitKey(0);  // 키 입력 대기
	destroyAllWindows();  // 모든 윈도우 제거
}


// 예제 5: HDR 이미지 생성 및 톤매핑
void ex5() {
	cout << "Reading images and exposure times ..." << endl;
	vector<Mat> images;
	vector<float> times;
	readImagesAndTimes(images, times);  // 이미지와 노출 시간 읽기
	cout << "Finished reading images" << endl;

	// 카메라 응답 함수(CRF) 계산
	cout << "Calculating Camera Response Function ..." << endl;
	Mat responseDebevec;
	Ptr<CalibrateDebevec> calibrateDebevec = createCalibrateDebevec();
	calibrateDebevec->process(images, responseDebevec, times);  // CRF 계산
	cout << "----- CRF -----" << endl;

	// 이미지를 하나의 HDR 이미지로 병합
	cout << "Merging images into one HDR image ..." << endl;
	Mat hdrDebevec;
	Ptr<MergeDebevec> mergeDebevec = createMergeDebevec();
	mergeDebevec->process(images, hdrDebevec, times, responseDebevec);  // HDR 이미지 병합
	imwrite("hdrDebevec.hdr", hdrDebevec);  // HDR 이미지 저장
	cout << "Saved hdrDebevec.hdr" << endl;

	// Drago 방법을 사용한 톤매핑
	cout << "Tonemapping using Drago's method ..." << endl;
	Mat ldrDrago;
	Ptr<TonemapDrago> tonemapDrago = createTonemapDrago(1.0, 0.7, 0.85f);
	tonemapDrago->process(hdrDebevec, ldrDrago);  // 톤매핑
	ldrDrago = 3 * ldrDrago;
	ldrDrago.convertTo(ldrDrago, CV_8UC3, 255.0);
	imwrite("ldr-Drago.jpg", ldrDrago);  // 톤매핑 이미지 저장
	cout << "Saved ldr-Drago.jpg" << endl;

	// Reinhard 방법을 사용한 톤매핑
	cout << "Tonemapping using Reinhard's method ..." << endl;
	Mat ldrReinhard;
	Ptr<TonemapReinhard> tonemapReinhard = createTonemapReinhard(1.5f, 0, 0, 0);
	tonemapReinhard->process(hdrDebevec, ldrReinhard);  // 톤매핑
	ldrReinhard.convertTo(ldrReinhard, CV_8UC3, 255.0);
	imwrite("ldr-Reinhard.jpg", ldrReinhard);  // 톤매핑 이미지 저장
	cout << "Saved ldr-Reinhard.jpg" << endl;

	// Mantiuk 방법을 사용한 톤매핑
	cout << "Tonemapping using Mantiuk's method ..." << endl;
	Mat ldrMantiuk;
	Ptr<TonemapMantiuk> tonemapMantiuk = createTonemapMantiuk(2.2f, 0.85f, 1.2f);
	tonemapMantiuk->process(hdrDebevec, ldrMantiuk);  // 톤매핑
	ldrMantiuk = 3 * ldrMantiuk;
	ldrMantiuk.convertTo(ldrMantiuk, CV_8UC3, 255.0);
	imwrite("ldr-Mantiuk.jpg", ldrMantiuk);  // 톤매핑 이미지 저장
	cout << "Saved ldr-Mantiuk.jpg" << endl;

	// 톤매핑된 이미지 디스플레이
	showResized("Drago Tonemapped Image", ldrDrago);
	showResized("Reinhard Tonemapped Image", ldrReinhard);
	showResized("Mantiuk Tonemapped Image", ldrMantiuk);

	waitKey(0);  // 키 입력 대기
	destroyAllWindows();  // 모든 윈도우 제거
}




// 메인 함수: 각 예제를 순서대로 실행
int main() {
	std::cout << "EX 1" << std::endl;
	ex1();

	std::cout << "EX 5" << std::endl;
	ex5();

	return 0;
}
