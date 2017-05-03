#include "seam_carving.h"

cv::Mat remove_path_hori(const cv::Mat &image, const std::vector<int> &path)
{
    cv::Mat result(image.rows - 1, image.cols, CV_8UC3);
    for (int y = 0; y < result.rows; ++y)
    {
        for (int x = 0; x < result.cols; ++x)
        {
            int img_x = x, img_y = y;
            if (img_y >= path[x])
            {
                ++img_y;
            }
            for (int c = 0; c < 3; ++c)
            {
                result.at<unsigned char>(y, 3 * x + c) =
                    image.at<unsigned char>(img_y, 3 * img_x + c);
            }
        }
    }
    return result;
}

cv::Mat dup_path_hori(const cv::Mat &image, const std::vector<int> &path)
{
    cv::Mat result(image.rows + 1, image.cols, CV_8UC3);
    for (int y = 0; y < result.rows; ++y)
    {
        for (int x = 0; x < result.cols; ++x)
        {
            int img_x = x, img_y = y;
            if (img_y > path[x])
            {
                --img_y;
            }
            for (int c = 0; c < 3; ++c)
            {
                result.at<unsigned char>(y, 3 * x + c) =
                    image.at<unsigned char>(img_y, 3 * img_x + c);
            }
        }
    }
    return result;
}