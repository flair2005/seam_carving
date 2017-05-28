#ifndef _RESIZING_H_
#define _RESIZING_H_

#include <opencv2/opencv.hpp>

#include "seam_carving.hpp"

void image_extend(cv::Mat &image, int dc, int dr, energy_func_t energy_func);
void image_shrink(cv::Mat &image, int dc, int dr, energy_func_t energy_func);
void image_resize(cv::Mat &image, int new_width, int new_height, energy_func_t energy_func);

#endif // _RESIZING_H_