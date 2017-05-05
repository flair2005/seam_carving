#include "seam_carving.h"

cv::Mat remove_path_hori(const cv::Mat &image, const std::vector<int> &path)
{
    cv::Mat result(image.rows - 1, image.cols, CV_8UC3);
    for (int y = 0; y < result.rows; ++y)
    {
        for (int x = 0; x < result.cols; ++x)
        {
            int img_y = y;
            if (img_y >= path[x])
            {
                ++img_y;
            }
            for (int c = 0; c < 3; ++c)
            {
                result.at<unsigned char>(y, 3 * x + c) =
                    image.at<unsigned char>(img_y, 3 * x + c);
            }
        }
    }
    return result;
}

cv::Mat insert_path_hori(const cv::Mat &image, const std::vector<int> &path)
{
    cv::Mat result(image.rows + 1, image.cols, CV_8UC3);
    for (int y = 0; y < result.rows; ++y)
    {
        for (int x = 0; x < result.cols; ++x)
        {
            int img_y = y;
            if (img_y == path[x] && img_y < image.rows)
            {
                for (int c = 0; c < 3; ++c)
                {
                    result.at<unsigned char>(y, 3 * x + c) =
                        static_cast<unsigned char>(
                            (static_cast<int>(image.at<unsigned char>(img_y - 1, 3 * x + c)) +
                             static_cast<int>(image.at<unsigned char>(img_y, 3 * x + c))) / 2
                            );
                }
            }
            else
            {
                if (img_y > path[x])
                {
                    --img_y;
                }
                for (int c = 0; c < 3; ++c)
                {
                    result.at<unsigned char>(y, 3 * x + c) =
                        image.at<unsigned char>(img_y, 3 * x + c);
                }
            }
        }
    }
    return result;
}

cv::Mat remove_path_vert(const cv::Mat &image, const std::vector<int> &path)
{
    cv::Mat result(image.rows, image.cols - 1, CV_8UC3);
    for (int y = 0; y < result.rows; ++y)
    {
        for (int x = 0; x < result.cols; ++x)
        {
            int img_x = x;
            if (img_x >= path[y])
            {
                ++img_x;
            }
            for (int c = 0; c < 3; ++c)
            {
                result.at<unsigned char>(y, 3 * x + c) =
                    image.at<unsigned char>(y, 3 * img_x + c);
            }
        }
    }
    return result;
}

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
    return energy_image;
}

cv::Mat laplacian_energy(const cv::Mat &gray_image)
{
    cv::Mat grad;
    cv::Laplacian(gray_image, grad, CV_16S, 3);
    cv::Mat grad_abs;
    cv::convertScaleAbs(grad, grad_abs);
    return grad_abs;
}