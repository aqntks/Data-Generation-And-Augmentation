#include "opencv2/opencv.hpp"
#include <afxwin.h>

HBITMAP stringToBitmap(char* text, cv::Scalar color, int w, int h);
cv::Mat bitmapToMat(HBITMAP hBitmap);

void putUniText(cv::Mat src, cv::Mat& dst, char* text, cv::Point point, cv::Scalar color, int w, int h);
/*
* src: source image where we want to show the text
* dst: destination image where the text will be shown
* text: text which will be shown on the dst image
* point: left-top point to show text on the image
* color: text color
*/