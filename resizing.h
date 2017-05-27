#ifndef _RESIZING_H_
#define _RESIZING_H_

#include <opencv2/opencv.hpp>

void image_extend(cv::Mat &image, int dc, int dr);
void image_shrink(cv::Mat &image, int dc, int dr);
void draw_seam(cv::Mat &image, int nc, int nr, const std::vector<unsigned char> &color);

#endif // _RESIZING_H_