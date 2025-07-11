//#include <iostream>
//#include <vector>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc//imgproc.hpp"
//using namespace cv;
//using namespace std;
//
//// OpenCV로 Meanshifting 수행
//void exCvMeanShift() {
//	// 이미지 읽기
//	Mat img = imread("C://Users//82107//OneDrive//바탕 화면//8nd Week Lab Session Materials//8nd Week Lab Session Materials//fruits.png");
//	// 실패시 종료
//	if (img.empty())
//		exit(-1);
//	cout << " -- exCvMeanShft() --- " << endl;
//
//	// 이미지 크기 재설정 (256x256 으로 조정)
//	resize(img, img, Size(256, 256), 0, 0, INTER_LINEAR); // 구 버전 : CV_INTER_AREA (이미지 resize 보간)
//	// 원본이미지 출력
//	imshow("Src", img);
//	imwrite("exCvMeanShift_src.jpg", img);
//
//	// Mean-shift OpenCV
//	pyrMeanShiftFiltering(img, img, 8, 16);
//
//	// 필터링 이미지 출력
//	imshow("Dst", img); 
//	waitKey();
//	destroyAllWindows();
//	imwrite("exCvMeanShift_dst.jpg", img);
//}
//
//// --------------------------------------		low_level 구현		-----------------------------------
//
//// 구현의 편의를 위한 5 dimension class
//class Point5D {
//public:
//	float x, y, l, u, v; // x,y : 포인트좌표 l,u,v : LUV 값
//
//	// 생성자 및 소멸자
//	Point5D();
//	~Point5D();
//
//	void accumPt(Point5D); // 포인트 축적
//	void copyPt(Point5D); // 포인트 복사
//	float getColorDist(Point5D); // 색상 거리 계산
//	float getSpatialDist(Point5D); // 좌표 거리 계산
//	void scalePt(float); // 포인트 스케일링 함수 (평균용)
//	void setPt(float, float, float, float, float); // 포인트값 설정함수
//	void printPt();
//};
//
//// 포인트 축적함수 정의
//void Point5D::accumPt(Point5D Pt) {
//	// 입력 포인트의 모든 멤버변수값을 더한다(축적).
//	x += Pt.x;
//	y += Pt.y;
//	l += Pt.l;
//	u += Pt.u;
//	v += Pt.v;
//}
//
//// 포인트 복사함수 정의
//void Point5D::copyPt(Point5D Pt) {
//	// 입력 포인트의 모든 멤버변수값을 할당한다(복사).
//	x = Pt.x;
//	y = Pt.y;
//	l = Pt.l;
//	u = Pt.u;
//	v = Pt.v;
//}
//
//// 색상 거리계산 함수 정의
//float Point5D::getColorDist(Point5D Pt) {
//	// 해당포인트와 입력포인트의 색상값 l,u,v 에 대한 거리값을 계산 후 반환.
//	return sqrt(pow(l - Pt.l, 2) + pow(u - Pt.u, 2) + pow(v - Pt.v, 2));
//}
//
//// 좌표 거리계산 함수 정의
//float Point5D::getSpatialDist(Point5D Pt) {
//	//  해당포인트와 입력포인트의 좌표값 x,y 에 대한 거리값을 계산 후 반환.
//	return sqrt(pow(x - Pt.x, 2) + pow(y - Pt.y, 2));
//}
//
//// 포인트 스케일링 함수
//void Point5D::scalePt(float scale) { 
//	// 해당 포인트의 각 변수값을 입력 인자만큼 곱함 (스케일링).
//	x *= scale;
//	y *= scale;
//	l *= scale;
//	u *= scale;
//	v *= scale;
//}
//
//// 포인트 변수값 재설정
//void Point5D::setPt(float px, float py, float pl, float pa, float pb) {
//	x = px;
//	y = py;
//	l = pl;
//	u = pa;
//	v = pb;
//}
//
//// x,y,l,u,v 값 출력
//void Point5D::printPt() {
//	cout << x << " " << y << " " << l << " " << u << " " << v << endl;
//}
//
//
//// ** 생성자 및 소멸자 정의 **
//Point5D::Point5D() {
//	x = -1;
//	y = -1;
//}
//Point5D::~Point5D() {
//}
//
//
//// MeanShift 클래스
//class MeanShift {
//public:
//	float bw_spatial = 8; // Spatial bandwidth
//	float bw_color = 16; // COlor bandwidth
//	float min_shift_color = 0.1;	// 최소 컬러변화
//	float min_shift_spatial = 0.1;	// 최소 위치변화
//	int max_steps = 10;	// 최대 반복횟수
//	vector<Mat> img_split;	// 채널별로 분할되는 Mat
//	MeanShift(float, float, float, float, int);	// Bandwidth 설정을 위한 생성자
//	void doFiltering(Mat&);	// Mean shift filtering 함수
//};
//
//
//// MeanShift 생성자 정의
//MeanShift::MeanShift(float bs, float bc, float msc, float mss, int ms) {
//	bw_spatial = bs;
//	bw_color = bc;
//	max_steps = ms;
//	min_shift_color = msc;
//	min_shift_spatial = mss;
//}
//
//
//// ** doFiltering() 정의 **
//void MeanShift::doFiltering(Mat& Img) {
//	int height = Img.rows;
//	int width = Img.cols;
//	split(Img, img_split); // 이미지에 대한 색상값을 채널별로 분할
//
//	Point5D pt, pt_prev, pt_cur, pt_sum; 
//	int pad_left, pad_right, pad_top, pad_bottom;
//	size_t n_pt, step;
//
//	for (int row = 0; row < height; row++) {
//		for (int col = 0; col < width; col++) {
//			// **
//			// bw_spatial 값 만큼의 범위 내 탐색
//			pad_left = (col - bw_spatial) > 0 ? (col - bw_spatial) : 0;
//			pad_right = (col + bw_spatial) < width ? (col + bw_spatial) : width;
//			pad_top = (row - bw_spatial) > 0 ? (row - bw_spatial) : 0;
//			pad_bottom = (row + bw_spatial) < height ? (row + bw_spatial) : height;
//
//			// <현재픽셀 세팅>
//			pt_cur.setPt(row, col,
//				(float)img_split[0].at<uchar>(row, col),
//				(float)img_split[1].at<uchar>(row, col),
//				(float)img_split[2].at<uchar>(row, col));
//
//			// <주변픽셀 탐색>
//			step = 0;
//			do {
//				// 현재 포인트값 복사
//				pt_prev.copyPt(pt_cur);
//				pt_sum.setPt(0, 0, 0, 0, 0); // 포인트 합 초기화
//				n_pt = 0; // 전체 갯수 count (평균값 내기 위해)
//				// bw_spatial 범위 내 탐색 (=region of interest)
//				for (int hx = pad_top; hx < pad_bottom; hx++) {
//					for (int hy = pad_left; hy < pad_right; hy++) {
//						// hx,hy 좌표에서 luv값 세팅
//						pt.setPt(hx, hy,
//							(float)img_split[0].at<uchar>(hx, hy),
//							(float)img_split[1].at<uchar>(hx, hy),
//							(float)img_split[2].at<uchar>(hx, hy));
//
//						// **
//						// bw_color 범위 내의 색상이면 count
//						if (pt.getColorDist(pt_cur) < bw_color) {
//							pt_sum.accumPt(pt);
//							n_pt++;
//						}
//					}
//				}
//
//				// <축적결과를 기반으로 현재픽셀 갱신>
//				pt_sum.scalePt(1.0 / n_pt); // 축적결과 평균 (next center)
//				pt_cur.copyPt(pt_sum); // 다음 센터값 갱신
//				step++; // 이동
//
//			} while ((pt_cur.getColorDist(pt_prev) > min_shift_color) &&
//				(pt_cur.getSpatialDist(pt_prev) > min_shift_spatial) &&
//				(step < max_steps));
//			// 변화량 최소조건을 만족할 때 까지 반복
//			// 최대 반복횟수 조건도 포함
//
//			// < 결과 픽셀 갱신 >
//			Img.at<Vec3b>(row, col) = Vec3b(pt_cur.l, pt_cur.u, pt_cur.v);
//		}
//	}
//}
//
//void exMyMeanShift() {
//	// 이미지 읽기
//	Mat img = imread("C://Users//82107//OneDrive//바탕 화면//8nd Week Lab Session Materials//8nd Week Lab Session Materials//fruits.png");
//	// 실패시 종료
//	if (img.empty())
//		exit(-1);
//	cout << "  -- exMyMeanShift() ---  " << endl;
//
//	// 이미지 크기 재설정 (256x256 으로 조정)
//	resize(img, img, Size(256, 256), 0, 0, INTER_LINEAR);
//	// 원본이미지 출력
//	imshow("Src", img); 
//	imwrite("exMyMeanShift_src.jpg", img);
//
//	// MymeanShift
//	cvtColor(img, img, COLOR_RGB2Luv); // RGB 컬러를 LUV로 변환
//	//MeanShift MSProc(8, 16, 0.1, 0.1, 10);
//	MeanShift MSProc(15, 25, 0.1, 0.1, 20);// MeanShift 클래스 객체생성
//	MSProc.doFiltering(img); // MeanShift 클래스의 doFiltering() 함수 적용
//	cvtColor(img, img, COLOR_Luv2RGB); // LUV를 RGB로 변환
//	
//	// 필터링 이미지 출력
//	imshow("Dst", img);
//	waitKey();
//	destroyAllWindows();
//	imwrite("exMyMeanShift_dst.jpg", img);
//}
//
//int main() {
//
//	//exCvMeanShift();
//
//	exMyMeanShift();
//
//}