#ifndef _UTIL_H_
#define _UTIL_H_

#include <opencv2/opencv.hpp>

cv::Mat load_mask(const std::string &filename);
void apply_mask(cv::Mat &energy_image, const cv::Mat &mask);
void object_remove(cv::Mat &image, const cv::Mat &mask_);

#endif // _UTIL_H_