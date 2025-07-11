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

	// ������׷� ���
	calcHist(&gray, 1, channel_numbers, Mat(), histogram, 1, &number_bins, &channel_ranges);

	// ������׷� plot
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / number_bins);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
	normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat()); // ����ȭ

	for (int i = 1; i < number_bins; i++) {
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(histogram.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(histogram.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0); // ���� ���� �մ� ���� �׸��� ������� plot
	}
	imshow("hist_img", histImage);
	waitKey(0);  // Ű �Է� ���
	destroyAllWindows();  // ��� ������ ����

}

// �̹����� ���� �ð��� �о���� �Լ�
void readImagesAndTimes(vector<Mat>& images, vector<float>& times) {
	int numImages = 4;
	static const float timesArray[] = { 1 / 30.0f, 0.25f, 2.5f, 15.0f };
	times.assign(timesArray, timesArray + numImages);  // ���� �ð� ����
	static const char* filenames[] = {
		"C:/Users/82107/OneDrive/���� ȭ��/��ó������/60by1.jpg",
		"C:/Users/82107/OneDrive/���� ȭ��/��ó������/30by1.jpg",
		"C:/Users/82107/OneDrive/���� ȭ��/��ó������/15by1.jpg" ,
		"C:/Users/82107/OneDrive/���� ȭ��/��ó������/1.jpg" ,
	};
	/*static const char* filenames[] = {
		"C:/Users/82107/OneDrive/���� ȭ��/2024_1�б�/��ó/Lab/11nd Week Lab Session Materials (2) (1)/img_0.033.jpg",
		"C:/Users/82107/OneDrive/���� ȭ��/2024_1�б�/��ó/Lab/11nd Week Lab Session Materials (2) (1)/img_0.25.jpg",
		"C:/Users/82107/OneDrive/���� ȭ��/2024_1�б�/��ó/Lab/11nd Week Lab Session Materials (2) (1)/img_2.5.jpg" ,
		"C:/Users/82107/OneDrive/���� ȭ��/2024_1�б�/��ó/Lab/11nd Week Lab Session Materials (2) (1)/img_15.jpg" ,
		 };*/
	for (int i = 0; i < numImages; i++) {
		Mat im = imread(filenames[i]);  // �̹����� �о��
		if (im.empty()) {
			cout << "Could not open or find the image: " << filenames[i] << endl;
			return;
		}
		images.push_back(im);  // �о�� �̹����� ���Ϳ� �߰�
	}
}

// �̹��� ũ�⸦ �����Ͽ� �����ִ� �Լ�
void showResized(const string& windowName, const Mat& img, int width = 1280, int height = 720) {
	Mat resizedImg;
	resize(img, resizedImg, Size(width, height));  // �̹��� ũ�� ����
	namedWindow(windowName, WINDOW_AUTOSIZE);  // ������ ����
	GetHistogram(resizedImg);
	imshow(windowName, resizedImg);  // �̹��� ������
}

// ���� 1: �̹����� ���� �ð� �б� �� ���÷���
void ex1() {
	cout << "Reading images and exposure times ..." << endl;
	vector<Mat> images;
	vector<float> times;
	readImagesAndTimes(images, times);  // �̹����� ���� �ð� �б�
	cout << "Finished" << endl;

	// �̹����� ���÷����Ͽ� �ùٸ��� �ε�Ǿ����� Ȯ��
	for (size_t i = 0; i < images.size(); i++) {
		string windowName = "Image " + to_string(i + 1);
		showResized(windowName, images[i]);
		GetHistogram(images[i]);
		cout << "Exposure time for image " << i + 1 << ": " << times[i] << " seconds" << endl;
	}
	waitKey(0);  // Ű �Է� ���
	destroyAllWindows();  // ��� ������ ����
}


// ���� 5: HDR �̹��� ���� �� �����
void ex5() {
	cout << "Reading images and exposure times ..." << endl;
	vector<Mat> images;
	vector<float> times;
	readImagesAndTimes(images, times);  // �̹����� ���� �ð� �б�
	cout << "Finished reading images" << endl;

	// ī�޶� ���� �Լ�(CRF) ���
	cout << "Calculating Camera Response Function ..." << endl;
	Mat responseDebevec;
	Ptr<CalibrateDebevec> calibrateDebevec = createCalibrateDebevec();
	calibrateDebevec->process(images, responseDebevec, times);  // CRF ���
	cout << "----- CRF -----" << endl;

	// �̹����� �ϳ��� HDR �̹����� ����
	cout << "Merging images into one HDR image ..." << endl;
	Mat hdrDebevec;
	Ptr<MergeDebevec> mergeDebevec = createMergeDebevec();
	mergeDebevec->process(images, hdrDebevec, times, responseDebevec);  // HDR �̹��� ����
	imwrite("hdrDebevec.hdr", hdrDebevec);  // HDR �̹��� ����
	cout << "Saved hdrDebevec.hdr" << endl;

	// Drago ����� ����� �����
	cout << "Tonemapping using Drago's method ..." << endl;
	Mat ldrDrago;
	Ptr<TonemapDrago> tonemapDrago = createTonemapDrago(1.0, 0.7, 0.85f);
	tonemapDrago->process(hdrDebevec, ldrDrago);  // �����
	ldrDrago = 3 * ldrDrago;
	ldrDrago.convertTo(ldrDrago, CV_8UC3, 255.0);
	imwrite("ldr-Drago.jpg", ldrDrago);  // ����� �̹��� ����
	cout << "Saved ldr-Drago.jpg" << endl;

	// Reinhard ����� ����� �����
	cout << "Tonemapping using Reinhard's method ..." << endl;
	Mat ldrReinhard;
	Ptr<TonemapReinhard> tonemapReinhard = createTonemapReinhard(1.5f, 0, 0, 0);
	tonemapReinhard->process(hdrDebevec, ldrReinhard);  // �����
	ldrReinhard.convertTo(ldrReinhard, CV_8UC3, 255.0);
	imwrite("ldr-Reinhard.jpg", ldrReinhard);  // ����� �̹��� ����
	cout << "Saved ldr-Reinhard.jpg" << endl;

	// Mantiuk ����� ����� �����
	cout << "Tonemapping using Mantiuk's method ..." << endl;
	Mat ldrMantiuk;
	Ptr<TonemapMantiuk> tonemapMantiuk = createTonemapMantiuk(2.2f, 0.85f, 1.2f);
	tonemapMantiuk->process(hdrDebevec, ldrMantiuk);  // �����
	ldrMantiuk = 3 * ldrMantiuk;
	ldrMantiuk.convertTo(ldrMantiuk, CV_8UC3, 255.0);
	imwrite("ldr-Mantiuk.jpg", ldrMantiuk);  // ����� �̹��� ����
	cout << "Saved ldr-Mantiuk.jpg" << endl;

	// ����ε� �̹��� ���÷���
	showResized("Drago Tonemapped Image", ldrDrago);
	showResized("Reinhard Tonemapped Image", ldrReinhard);
	showResized("Mantiuk Tonemapped Image", ldrMantiuk);

	waitKey(0);  // Ű �Է� ���
	destroyAllWindows();  // ��� ������ ����
}




// ���� �Լ�: �� ������ ������� ����
int main() {
	std::cout << "EX 1" << std::endl;
	ex1();

	std::cout << "EX 5" << std::endl;
	ex5();

	return 0;
}
