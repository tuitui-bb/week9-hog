#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;
float norml2(float * a,float * b,int n) {
	float h = 0;
	for (int i = 0; i < n; i++)
	{
		h = (a[i] - b[i]) * (a[i] - b[i]);
	}
	return sqrt(h);
}

int calcHog(Mat src, float* hist, int nAngle, int cellSize) {
	Mat gx, gy;
	Mat mag, angle;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	int nX = src.cols / cellSize;
	int nY = src.rows / cellSize;
	Rect roi;
	roi.height = cellSize;
	roi.width = cellSize;
		
		for (int i = 0; i < nY; i++) {
			for (int j = 0; j < nX; j++) {
				cv::Mat roiMat;
				cv::Mat roiMag;
				cv::Mat roiAgl;
				roi.x = j * cellSize;
				roi.y = i * cellSize;
				roiMat = src(roi);
				roiMag = mag(roi);
				roiAgl = angle(roi);
				int head = (i * nX + j) * nAngle;
				
				for (int n = 0; n < roiMat.rows; n++)
					for (int m = 0; m < roiMat.cols; m++) {
						hist[head + (int)roiAgl.at<float>(n, m) / (360 / nAngle)] += roiMag.at<float>(n, m);
					}
			}
		}
		return 0;
}


int main() {
	int cellSize = 16,  nAngle = 8;
	Mat refMat = imread("D://pp1.jpg");
	Mat plMat = imread("D://pp2.jpg");
	Mat bgMat = imread("D://pp3.jpg");
	
	int nX = refMat.cols / cellSize;
	int nY = refMat.rows / cellSize;

	int	bins = nAngle* nY * nX;

	
		float* ref_hist = new float[bins];
		memset(ref_hist, 0, sizeof(float) * bins);
		float* pl_hist = new float[bins];
		memset(pl_hist, 0, sizeof(float) * bins);
		float* bg_hist = new float[bins];
		memset(bg_hist, 0, sizeof(float) * bins);
	int reCode = 0;
	reCode = calcHog(refMat, ref_hist, nAngle, cellSize);
	reCode = calcHog(plMat, pl_hist, nAngle, cellSize);
	reCode = calcHog(bgMat, bg_hist, nAngle, cellSize);

	if (reCode != 0) {
		return -1;
	}	

	float dist1 = norml2(ref_hist, pl_hist, bins);
	float dist2 = norml2(ref_hist, bg_hist, bins);
	cout<< dist1 << endl;
	cout << dist2 << endl;
	  
	if (dist1 > dist2) {
		cout << "图二更接近原图" << endl;
	}
	else 
	{	
		cout<<"图一更接近原图"  << endl;
	}
	delete[] ref_hist;
	delete[] pl_hist;
	delete[] bg_hist;
return 0;
}


