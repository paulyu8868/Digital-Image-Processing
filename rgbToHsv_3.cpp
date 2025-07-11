#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
using namespace cv;
using namespace std;

Mat CvKMeans(Mat src_img, int k) {
    Mat samples(src_img.rows * src_img.cols, src_img.channels(), CV_32F);
    for (int y = 0; y < src_img.rows; y++) {
        for (int x = 0; x < src_img.cols; x++) {
            if (src_img.channels() == 3) {
                for (int z = 0; z < src_img.channels(); z++) {
                    samples.at<float>(y + x * src_img.rows, z) =
                        (float)src_img.at<Vec3b>(y, x)[z];
                }
            }
            else {
                samples.at<float>(y + x * src_img.rows) =
                    (float)src_img.at<uchar>(y, x);
            }
        }
    }

    Mat labels;
    Mat centers;
    int attempts = 5;

    kmeans(samples, k, labels, TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS,
        10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

    Mat dst_img(src_img.size(), src_img.type());
    for (int y = 0; y < src_img.rows; y++) {
        for (int x = 0; x < src_img.cols; x++) {
            int cluster_idx = labels.at<int>(y + x * src_img.rows, 0);
            if (src_img.channels() == 3) {
                for (int z = 0; z < src_img.channels(); z++) {
                    dst_img.at<Vec3b>(y, x)[z] =
                        (uchar)centers.at<float>(cluster_idx, z);
                }
            }
            else {
                dst_img.at<uchar>(y, x) = (uchar)centers.at<float>(cluster_idx, 0);
            }
        }
    }
    return dst_img;
}


// RGB 컬러를 HSV 모델로 변환
Mat MyBgr2Hsv(Mat src_img) {
    double b, g, r, h, s, v;
    Mat dst_img(src_img.size(), src_img.type());
    for (int y = 0; y < src_img.rows; y++) {
        for (int x = 0; x < src_img.cols; x++) {
            // rgb 컬러 추출
            b = (double)src_img.at<Vec3b>(y, x)[0];
            g = (double)src_img.at<Vec3b>(y, x)[1];
            r = (double)src_img.at<Vec3b>(y, x)[2];
            // RGB to HSV 
            double max_val = max(max(b, g), r);
            double min_val = min(min(b, g), r);
            double delta = max_val - min_val;
            // Hue
            if (max_val == r) {
                h = 60 * (0 + (g - b) / (max_val - min_val));
            }
            if (max_val == g) {
                h = 60 * (2 + (b - r) / (max_val - min_val));
            }
            if (max_val == b) {
                h = 60 * (4 + (r - g) / (max_val - min_val));
            }
            // 범위 [0~360]
            if (h < 0) {
                h = h + 360;
            }
            // Saturation
            if (delta == 0) {
                s = 0;
            }
            else
                s = (max_val - min_val) / max_val;

            //V
            v = max_val;
            // Mat hsv_img;
            dst_img.at<Vec3b>(y, x)[0] = (uchar)(h / 2);
            dst_img.at<Vec3b>(y, x)[1] = (uchar)(s * 255);
            dst_img.at<Vec3b>(y, x)[2] = (uchar)v;
        }
    }

    return dst_img;
}


// 마스크 이미지 생성
Mat MyinRange(Mat src_img, Scalar lower, Scalar upper) {

    Mat dst_img(src_img.rows, src_img.cols, CV_8UC1);
    for (int y = 0; y < src_img.rows; y++) {
        for (int x = 0; x < src_img.cols; x++) {
            double b = (double)src_img.at<Vec3b>(y, x)[0];
            double g = (double)src_img.at<Vec3b>(y, x)[1];
            double r = (double)src_img.at<Vec3b>(y, x)[2];


            bool in_range = (lower[0] <= b && b <= upper[0]) &&
                (lower[1] <= g && g <= upper[1]) &&
                (lower[2] <= r && r <= upper[2]);
            if (in_range) {
                dst_img.at<uchar>(y, x) = 255;
            }
            else {
                dst_img.at<uchar>(y, x) = 0;
            }

        }
    }
    return dst_img;
}



int main() {
    // Red
    Scalar lower_red = Scalar(0, 50, 100);
    Scalar upper_red = Scalar(10, 255, 255);
    // Blue
    Scalar lower_blue = Scalar(100, 50, 100);
    Scalar upper_blue = Scalar(120, 255, 255);
    // Green
    Scalar lower_green = Scalar(30, 30, 50);
    Scalar upper_green = Scalar(50, 255, 255);
    Mat src_img = imread("C://Users//82107//OneDrive//바탕 화면//7nd Week Lab Session Materials//7nd Week Lab Session Materials//fruits.png", 1);
    
    std::cout << "~kmeans waiting~" << endl;
    Mat fruits_5 = CvKMeans(src_img, 5);
    std::cout << "kmeans Done!" << endl;

    //hsv로 변환
    Mat hsv_img = MyBgr2Hsv(fruits_5);
    // 마스크 생성
    Mat mask_red = MyinRange(hsv_img, lower_red, upper_red);
    Mat mask_blue = MyinRange(hsv_img, lower_blue, upper_blue);
    Mat mask_green = MyinRange(hsv_img, lower_green, upper_green);
    // 색상 검출
    int red_pixels = countNonZero(mask_red);
    int blue_pixels = countNonZero(mask_blue);
    int green_pixels = countNonZero(mask_green);
    Mat final;
    if (red_pixels > blue_pixels && red_pixels > green_pixels)
    {
        cout << "The image is red" << endl;
        bitwise_and(src_img, src_img, final, mask_red);
    }
    else if (blue_pixels > red_pixels && blue_pixels > green_pixels)
    {
        cout << "The image is blue" << endl;
        bitwise_and(src_img, src_img, final, mask_blue);
    }
    else if (green_pixels > red_pixels && green_pixels > blue_pixels)
    {
        cout << "The image is green" << endl;
        bitwise_and(src_img, src_img, final, mask_green);
    }
    else
    {
        cout << "The image contains multiple colors" << endl;
    }

    imshow("final", final);
    waitKey(0);

    return 0;
}