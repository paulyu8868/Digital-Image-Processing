//#include <iostream>
//#include <vector>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc//imgproc.hpp"
//using namespace cv;
//using namespace std;
//
//// OpenCV�� Meanshifting ����
//void exCvMeanShift() {
//	// �̹��� �б�
//	Mat img = imread("C://Users//82107//OneDrive//���� ȭ��//8nd Week Lab Session Materials//8nd Week Lab Session Materials//fruits.png");
//	// ���н� ����
//	if (img.empty())
//		exit(-1);
//	cout << " -- exCvMeanShft() --- " << endl;
//
//	// �̹��� ũ�� �缳�� (256x256 ���� ����)
//	resize(img, img, Size(256, 256), 0, 0, INTER_LINEAR); // �� ���� : CV_INTER_AREA (�̹��� resize ����)
//	// �����̹��� ���
//	imshow("Src", img);
//	imwrite("exCvMeanShift_src.jpg", img);
//
//	// Mean-shift OpenCV
//	pyrMeanShiftFiltering(img, img, 8, 16);
//
//	// ���͸� �̹��� ���
//	imshow("Dst", img); 
//	waitKey();
//	destroyAllWindows();
//	imwrite("exCvMeanShift_dst.jpg", img);
//}
//
//// --------------------------------------		low_level ����		-----------------------------------
//
//// ������ ���Ǹ� ���� 5 dimension class
//class Point5D {
//public:
//	float x, y, l, u, v; // x,y : ����Ʈ��ǥ l,u,v : LUV ��
//
//	// ������ �� �Ҹ���
//	Point5D();
//	~Point5D();
//
//	void accumPt(Point5D); // ����Ʈ ����
//	void copyPt(Point5D); // ����Ʈ ����
//	float getColorDist(Point5D); // ���� �Ÿ� ���
//	float getSpatialDist(Point5D); // ��ǥ �Ÿ� ���
//	void scalePt(float); // ����Ʈ �����ϸ� �Լ� (��տ�)
//	void setPt(float, float, float, float, float); // ����Ʈ�� �����Լ�
//	void printPt();
//};
//
//// ����Ʈ �����Լ� ����
//void Point5D::accumPt(Point5D Pt) {
//	// �Է� ����Ʈ�� ��� ����������� ���Ѵ�(����).
//	x += Pt.x;
//	y += Pt.y;
//	l += Pt.l;
//	u += Pt.u;
//	v += Pt.v;
//}
//
//// ����Ʈ �����Լ� ����
//void Point5D::copyPt(Point5D Pt) {
//	// �Է� ����Ʈ�� ��� ����������� �Ҵ��Ѵ�(����).
//	x = Pt.x;
//	y = Pt.y;
//	l = Pt.l;
//	u = Pt.u;
//	v = Pt.v;
//}
//
//// ���� �Ÿ���� �Լ� ����
//float Point5D::getColorDist(Point5D Pt) {
//	// �ش�����Ʈ�� �Է�����Ʈ�� ���� l,u,v �� ���� �Ÿ����� ��� �� ��ȯ.
//	return sqrt(pow(l - Pt.l, 2) + pow(u - Pt.u, 2) + pow(v - Pt.v, 2));
//}
//
//// ��ǥ �Ÿ���� �Լ� ����
//float Point5D::getSpatialDist(Point5D Pt) {
//	//  �ش�����Ʈ�� �Է�����Ʈ�� ��ǥ�� x,y �� ���� �Ÿ����� ��� �� ��ȯ.
//	return sqrt(pow(x - Pt.x, 2) + pow(y - Pt.y, 2));
//}
//
//// ����Ʈ �����ϸ� �Լ�
//void Point5D::scalePt(float scale) { 
//	// �ش� ����Ʈ�� �� �������� �Է� ���ڸ�ŭ ���� (�����ϸ�).
//	x *= scale;
//	y *= scale;
//	l *= scale;
//	u *= scale;
//	v *= scale;
//}
//
//// ����Ʈ ������ �缳��
//void Point5D::setPt(float px, float py, float pl, float pa, float pb) {
//	x = px;
//	y = py;
//	l = pl;
//	u = pa;
//	v = pb;
//}
//
//// x,y,l,u,v �� ���
//void Point5D::printPt() {
//	cout << x << " " << y << " " << l << " " << u << " " << v << endl;
//}
//
//
//// ** ������ �� �Ҹ��� ���� **
//Point5D::Point5D() {
//	x = -1;
//	y = -1;
//}
//Point5D::~Point5D() {
//}
//
//
//// MeanShift Ŭ����
//class MeanShift {
//public:
//	float bw_spatial = 8; // Spatial bandwidth
//	float bw_color = 16; // COlor bandwidth
//	float min_shift_color = 0.1;	// �ּ� �÷���ȭ
//	float min_shift_spatial = 0.1;	// �ּ� ��ġ��ȭ
//	int max_steps = 10;	// �ִ� �ݺ�Ƚ��
//	vector<Mat> img_split;	// ä�κ��� ���ҵǴ� Mat
//	MeanShift(float, float, float, float, int);	// Bandwidth ������ ���� ������
//	void doFiltering(Mat&);	// Mean shift filtering �Լ�
//};
//
//
//// MeanShift ������ ����
//MeanShift::MeanShift(float bs, float bc, float msc, float mss, int ms) {
//	bw_spatial = bs;
//	bw_color = bc;
//	max_steps = ms;
//	min_shift_color = msc;
//	min_shift_spatial = mss;
//}
//
//
//// ** doFiltering() ���� **
//void MeanShift::doFiltering(Mat& Img) {
//	int height = Img.rows;
//	int width = Img.cols;
//	split(Img, img_split); // �̹����� ���� ������ ä�κ��� ����
//
//	Point5D pt, pt_prev, pt_cur, pt_sum; 
//	int pad_left, pad_right, pad_top, pad_bottom;
//	size_t n_pt, step;
//
//	for (int row = 0; row < height; row++) {
//		for (int col = 0; col < width; col++) {
//			// **
//			// bw_spatial �� ��ŭ�� ���� �� Ž��
//			pad_left = (col - bw_spatial) > 0 ? (col - bw_spatial) : 0;
//			pad_right = (col + bw_spatial) < width ? (col + bw_spatial) : width;
//			pad_top = (row - bw_spatial) > 0 ? (row - bw_spatial) : 0;
//			pad_bottom = (row + bw_spatial) < height ? (row + bw_spatial) : height;
//
//			// <�����ȼ� ����>
//			pt_cur.setPt(row, col,
//				(float)img_split[0].at<uchar>(row, col),
//				(float)img_split[1].at<uchar>(row, col),
//				(float)img_split[2].at<uchar>(row, col));
//
//			// <�ֺ��ȼ� Ž��>
//			step = 0;
//			do {
//				// ���� ����Ʈ�� ����
//				pt_prev.copyPt(pt_cur);
//				pt_sum.setPt(0, 0, 0, 0, 0); // ����Ʈ �� �ʱ�ȭ
//				n_pt = 0; // ��ü ���� count (��հ� ���� ����)
//				// bw_spatial ���� �� Ž�� (=region of interest)
//				for (int hx = pad_top; hx < pad_bottom; hx++) {
//					for (int hy = pad_left; hy < pad_right; hy++) {
//						// hx,hy ��ǥ���� luv�� ����
//						pt.setPt(hx, hy,
//							(float)img_split[0].at<uchar>(hx, hy),
//							(float)img_split[1].at<uchar>(hx, hy),
//							(float)img_split[2].at<uchar>(hx, hy));
//
//						// **
//						// bw_color ���� ���� �����̸� count
//						if (pt.getColorDist(pt_cur) < bw_color) {
//							pt_sum.accumPt(pt);
//							n_pt++;
//						}
//					}
//				}
//
//				// <��������� ������� �����ȼ� ����>
//				pt_sum.scalePt(1.0 / n_pt); // ������� ��� (next center)
//				pt_cur.copyPt(pt_sum); // ���� ���Ͱ� ����
//				step++; // �̵�
//
//			} while ((pt_cur.getColorDist(pt_prev) > min_shift_color) &&
//				(pt_cur.getSpatialDist(pt_prev) > min_shift_spatial) &&
//				(step < max_steps));
//			// ��ȭ�� �ּ������� ������ �� ���� �ݺ�
//			// �ִ� �ݺ�Ƚ�� ���ǵ� ����
//
//			// < ��� �ȼ� ���� >
//			Img.at<Vec3b>(row, col) = Vec3b(pt_cur.l, pt_cur.u, pt_cur.v);
//		}
//	}
//}
//
//void exMyMeanShift() {
//	// �̹��� �б�
//	Mat img = imread("C://Users//82107//OneDrive//���� ȭ��//8nd Week Lab Session Materials//8nd Week Lab Session Materials//fruits.png");
//	// ���н� ����
//	if (img.empty())
//		exit(-1);
//	cout << "  -- exMyMeanShift() ---  " << endl;
//
//	// �̹��� ũ�� �缳�� (256x256 ���� ����)
//	resize(img, img, Size(256, 256), 0, 0, INTER_LINEAR);
//	// �����̹��� ���
//	imshow("Src", img); 
//	imwrite("exMyMeanShift_src.jpg", img);
//
//	// MymeanShift
//	cvtColor(img, img, COLOR_RGB2Luv); // RGB �÷��� LUV�� ��ȯ
//	//MeanShift MSProc(8, 16, 0.1, 0.1, 10);
//	MeanShift MSProc(15, 25, 0.1, 0.1, 20);// MeanShift Ŭ���� ��ü����
//	MSProc.doFiltering(img); // MeanShift Ŭ������ doFiltering() �Լ� ����
//	cvtColor(img, img, COLOR_Luv2RGB); // LUV�� RGB�� ��ȯ
//	
//	// ���͸� �̹��� ���
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