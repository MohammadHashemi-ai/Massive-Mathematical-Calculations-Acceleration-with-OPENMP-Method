
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <chrono>
#include "stdio.h"
#include "omp.h"

#define		NUM_OF_THREADS		2

using namespace cv;
using namespace std;
using namespace std::chrono;
int main()
{
Mat src1, src2, src_gray1, src_gray2;


// Load an image
src1 = imread("lena.png");
src2 = imread("lena1.png");
//src1 = imread("girlface.png");
//src2 = imread("girlface1.png");
//src1 = imread("im.png");
//src2 = imread("im1.png");

	if (!src1.data || !src2.data)
	{
		cout << "wrong file input" << endl;
		cout << "press any key to exit" << endl;
		getchar();
		return -1;
	}


//check input images size
unsigned int src1_rows, src1_cols, src2_rows, src2_cols;
src1_rows = src1.rows;
src1_cols = src1.cols;
src2_rows = src2.rows;
src2_cols = src2.cols;

	if ((src1_rows != src2_rows) || (src1_cols != src2_cols)) {
		cout << "input images size are not equal" << endl;
		cout << "press any key to exit" << endl;
		getchar();
		return -1;
	}


imshow("Original first image", src1);
imshow("Original second image", src2);

// Convert it to gray
cvtColor(src1, src_gray1, CV_RGB2GRAY);
cvtColor(src2, src_gray2, CV_RGB2GRAY);

src_gray1.convertTo(src_gray1, CV_64F, 1, 0);
src_gray2.convertTo(src_gray2, CV_64F, 1, 0);


// Create window
namedWindow("Image Difference serial", CV_WINDOW_AUTOSIZE);

//declare new Matrix to calculate absolute difference
Mat diff;
diff = Mat::zeros(src_gray1.rows, src_gray1.cols, CV_64F);	//all elements become zero


high_resolution_clock::time_point start_serial = high_resolution_clock::now();

//find absolute difference of two input images
for (unsigned int i = 0; i < src_gray1.rows - 1; i++) {
	for (unsigned int j = 0; j < src_gray1.cols - 1; j++) {
		diff.at<double>(i, j) = abs(src_gray1.at<double>(i, j) - src_gray2.at<double>(i, j));
	}
}



cout << "output image width: " << diff.cols << endl;
cout << "output image height: " << diff.rows << endl;


high_resolution_clock::time_point end_serial = high_resolution_clock::now();
duration<double> time_span_serial = duration_cast<duration<double>>(end_serial - start_serial);
cout << "Execution time of Serial code: " << time_span_serial.count() << " seconds" << endl;

//conversion due to show image
diff.convertTo(diff, CV_8U, 5, 0);
imshow("Image Difference serial", diff);

Mat diff1;
diff1 = Mat::zeros(src_gray1.rows, src_gray1.cols, CV_64F);
int i, j;
high_resolution_clock::time_point start_omp = high_resolution_clock::now();

#pragma omp parallel num_threads(NUM_OF_THREADS) shared(src_gray1,diff,src_gray2) private(i,j)
{
	#pragma omp for
	for (i = 0; i < src_gray1.rows - 1; i++) {
		for (j = 0; j < src_gray1.cols - 1; j++) {
			diff1.at<double>(i, j) = abs(src_gray1.at<double>(i, j) - src_gray2.at<double>(i, j));
		}
	}
}


high_resolution_clock::time_point end_omp = high_resolution_clock::now();
duration<double> time_span_omp = duration_cast<duration<double>>(end_omp - start_omp);
cout << "Execution time of OpenMP code: " << time_span_omp.count() << " seconds" << endl;


diff1.convertTo(diff1, CV_8U, 5, 0);
imshow("Image Difference parallel", diff1);
printf("Speedup from Serial to OpenMP = %4.2f\n", (float)(time_span_serial.count()) / (float)(time_span_omp.count()));


waitKey(0);

return 0;
}
