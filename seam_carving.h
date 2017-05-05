#ifndef _SEAM_CARVING_HPP_
#define _SEAM_CARVING_HPP_

#include <vector>
#include <functional>
#include <algorithm>
#include <opencv2/opencv.hpp>

#include "weighted_value.h"

struct path_result
{
    std::vector<int> path;
    int total_energy;
};

typedef weighted_value<int, int> weighted_int_t;

// find a horizontal seam whose energy is minimum
template <typename COMPARE_FUNC = std::less<int> >
path_result find_hori_seam(const cv::Mat &energy_image,
                           weighted_int_t *buffer = nullptr,
                           int nth = 0,
                           COMPARE_FUNC cmp = std::less<int>())
{
    weighted_int_t *dp;
    if (!buffer)
    {
        dp = new weighted_int_t[(energy_image.cols + 1) * energy_image.rows];
    }
    else
    {
        dp = buffer;
    }

    // first level: init
    for (int y = 1; y < energy_image.rows - 1; ++y)
    {
        int e = energy_image.at<unsigned char>(y, 0);
        dp[y] = weighted_int_t { e, -1 };
    }

    // dp
    weighted_int_t *last_level = dp, *current_level = dp + energy_image.rows;
    for (int x = 1; x < energy_image.cols; ++x)
    {
        for (int y = 1; y < energy_image.rows - 1; ++y)
        {
            weighted_int_t &e_min = current_level[y];

            // (x - 1, y)
            e_min.weight = last_level[y].weight;
            e_min.value = y;

            // (x - 1, y - 1)
            if (y - 1 >= 1 && cmp(last_level[y - 1].weight, e_min.weight))
            {
                e_min.weight = last_level[y - 1].weight;
                e_min.value = y - 1;
            }

            // (x - 1, y + 1)
            if (y + 1 < energy_image.rows - 1 && cmp(last_level[y + 1].weight, e_min.weight))
            {
                e_min.weight = last_level[y + 1].weight;
                e_min.value = y + 1;
            }

            short e = energy_image.at<unsigned char>(y, x);
            e_min.weight += e;
        }

        last_level += energy_image.rows;
        current_level += energy_image.rows;
    }

    for (int y = 1; y < energy_image.rows - 1; ++y)
    {
        weighted_int_t &e_min = current_level[y];

        // (x - 1, y)
        e_min.weight = last_level[y].weight;
        e_min.value = y;
    }

    // last level: find minimum.
    weighted_int_t *e_min;
    if (nth == 0)
    {
        e_min = std::min_element(current_level + 1, current_level + (energy_image.rows - 1));
    }
    else
    {
        std::nth_element(current_level + 1,
                         current_level + (1 + nth),
                         current_level + (energy_image.rows - 1));
        e_min = current_level + (1 + nth);
    }

    // backtrace
    std::vector<int> energy_min_path(energy_image.cols);
    energy_min_path[energy_image.cols - 1] = e_min->value;
    for (int x = energy_image.cols - 2; x >= 0; --x)
    {
        energy_min_path[x] = last_level[energy_min_path[x + 1]].value;
        last_level -= energy_image.rows;
    }

    if (!buffer)
    {
        delete [] dp;
    }

    return path_result { std::move(energy_min_path), e_min->weight };
}

// find a vertical seam whose energy is minimum
template <typename COMPARE_FUNC = std::less<int> >
path_result find_vert_seam(const cv::Mat &energy_image,
                           weighted_int_t *buffer = nullptr,
                           int nth = 0,
                           COMPARE_FUNC cmp = std::less<int>())
{
    weighted_int_t *dp;
    if (!buffer)
    {
        dp = new weighted_int_t[(energy_image.rows + 1) * energy_image.cols];
    }
    else
    {
        dp = buffer;
    }

    // first level: init
    for (int x = 1; x < energy_image.cols - 1; ++x)
    {
        int e = energy_image.at<unsigned char>(0, x);
        dp[x] = weighted_int_t{ e, -1 };
    }

    // dp
    weighted_int_t *last_level = dp, *current_level = dp + energy_image.cols;
    for (int y = 1; y < energy_image.rows; ++y)
    {
        for (int x = 1; x < energy_image.cols - 1; ++x)
        {
            weighted_int_t &e_min = current_level[x];

            // (x, y - 1)
            e_min.weight = last_level[x].weight;
            e_min.value = x;

            // (x - 1, y - 1)
            if (x - 1 >= 1 && cmp(last_level[x - 1].weight, e_min.weight))
            {
                e_min.weight = last_level[x - 1].weight;
                e_min.value = x - 1;
            }

            // (x + 1, y - 1)
            if (x + 1 < energy_image.cols - 1 && cmp(last_level[x + 1].weight, e_min.weight))
            {
                e_min.weight = last_level[x + 1].weight;
                e_min.value = x + 1;
            }

            short e = energy_image.at<unsigned char>(y, x);
            e_min.weight += e;
        }

        last_level += energy_image.cols;
        current_level += energy_image.cols;
    }

    for (int x = 1; x < energy_image.cols - 1; ++x)
    {
        weighted_int_t &e_min = current_level[x];

        // (x, y - 1)
        e_min.weight = last_level[x].weight;
        e_min.value = x;
    }

    // last level: find minimum.
    weighted_int_t *e_min;
    if (nth == 0)
    {
        e_min = std::min_element(current_level + 1, current_level + (energy_image.cols - 1));
    }
    else
    {
        std::nth_element(current_level + 1,
                         current_level + (1 + nth),
                         current_level + (energy_image.cols - 1));
        e_min = current_level + (1 + nth);
    }

    // backtrace
    std::vector<int> energy_min_path(energy_image.rows);
    energy_min_path[energy_image.rows - 1] = e_min->value;
    for (int y = energy_image.rows - 2; y >= 0; --y)
    {
        energy_min_path[y] = last_level[energy_min_path[y + 1]].value;
        last_level -= energy_image.cols;
    }

    if (!buffer)
    {
        delete[] dp;
    }

    return path_result{ std::move(energy_min_path), e_min->weight };
}

cv::Mat remove_path_hori(const cv::Mat &image, const std::vector<int> &path);
cv::Mat insert_path_hori(const cv::Mat &image, const std::vector<int> &path);
cv::Mat remove_path_vert(const cv::Mat &image, const std::vector<int> &path);
// cv::Mat insert_path_hori(const cv::Mat &image, const std::vector<int> &path);
cv::Mat sobel_energy(const cv::Mat &gray_image);
cv::Mat laplacian_energy(const cv::Mat &gray_image);

#endif // _SEAM_CARVING_HPP_