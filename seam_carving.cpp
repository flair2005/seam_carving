#include "seam_carving.h"

cv::Mat sobel_energy(const cv::Mat &gray_image)
{
    cv::Mat grad_x;
    cv::Sobel(gray_image, grad_x, CV_16S, 1, 0, 3);
    cv::Mat grad_y;
    cv::Sobel(gray_image, grad_y, CV_16S, 0, 1, 3);
    cv::Mat grad_abs_x;
    cv::convertScaleAbs(grad_x, grad_abs_x);
    cv::Mat grad_abs_y;
    cv::convertScaleAbs(grad_y, grad_abs_y);
    cv::Mat energy_image;
    cv::addWeighted(grad_abs_x, 0.5, grad_abs_y, 0.5, 0.0, energy_image);
    cv::Mat img_16s;
    energy_image.convertTo(img_16s, CV_16S, 1.0);
    return img_16s;
}

cv::Mat laplacian_energy(const cv::Mat &gray_image)
{
    cv::Mat grad;
    cv::Laplacian(gray_image, grad, CV_16S, 3);
    cv::Mat grad_abs;
    cv::convertScaleAbs(grad, grad_abs);
    cv::Mat img_16s;
    grad_abs.convertTo(img_16s, CV_16S, 1.0);
    return img_16s;
}