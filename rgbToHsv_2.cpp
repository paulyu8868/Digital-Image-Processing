//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include "opencv2/core/core.hpp" 
//#include "opencv2/highgui/highgui.hpp" 
//#include "opencv2/imgproc/imgproc.hpp" 
//using namespace cv;
//using namespace std;
//
//Mat CvKMeans(Mat src_img, int k) {
//    Mat samples(src_img.rows * src_img.cols, src_img.channels(), CV_32F);
//    for (int y = 0; y < src_img.rows; y++) {
//        for (int x = 0; x < src_img.cols; x++) {
//            if (src_img.channels() == 3) {
//                for (int z = 0; z < src_img.channels(); z++) {
//                    samples.at<float>(y + x * src_img.rows, z) =
//                        (float)src_img.at<Vec3b>(y, x)[z];
//                }
//            }
//            else {
//                samples.at<float>(y + x * src_img.rows) =
//                    (float)src_img.at<uchar>(y, x);
//            }
//        }
//    }
//
//    Mat labels;
//    Mat centers;
//    int attempts = 5;
//
//    kmeans(samples, k, labels, TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS,
//        10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
//
//    Mat dst_img(src_img.size(), src_img.type());
//    for (int y = 0; y < src_img.rows; y++) {
//        for (int x = 0; x < src_img.cols; x++) {
//            int cluster_idx = labels.at<int>(y + x * src_img.rows, 0);
//            if (src_img.channels() == 3) {
//                for (int z = 0; z < src_img.channels(); z++) {
//                    dst_img.at<Vec3b>(y, x)[z] =
//                        (uchar)centers.at<float>(cluster_idx, z);
//                }
//            }
//            else {
//                dst_img.at<uchar>(y, x) = (uchar)centers.at<float>(cluster_idx, 0);
//            }
//        }
//    }
//    return dst_img;
//}
//
//int main() {
//    Mat src_img = imread("C://Users//82107//OneDrive//πŸ≈¡ »≠∏È//7nd Week Lab Session Materials//7nd Week Lab Session Materials//fruits.png", 1);
//    std::cout << "~kmeans waiting~" << endl;
//
//    //// k=2
//    //Mat beach2 = CvKMeans(src_img, 2); 
//    //imshow("beach2", beach2);
//
//    //// k=5
//    //Mat beach5 = CvKMeans(src_img, 5);
//    //imshow("beach5", beach5);
//    //
//    // k=10
//    Mat beach10 = CvKMeans(src_img, 10);
//    imshow("beach10", beach10);
//
//    //// k=30
//    //Mat beach30 = CvKMeans(src_img, 30);
//    //imshow("beach30", beach30);
//
//    std::cout << "kmeans Done!" << endl;
//    waitKey(0); 
//
//    return 0;
//}