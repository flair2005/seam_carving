#ifndef _UTIL_H_
#define _UTIL_H_

#include <opencv2/opencv.hpp>

#include "seam_carving.hpp"

cv::Mat load_mask(const std::string &filename);
void apply_mask(cv::Mat &energy_image, const cv::Mat &mask);
void object_remove(cv::Mat &image, cv::Mat mask, energy_func_t energy_func);

#endif // _UTIL_H_