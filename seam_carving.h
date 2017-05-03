#ifndef _SEAM_CARVING_HPP_
#define _SEAM_CARVING_HPP_

#include <vector>
#include <functional>
#include <opencv2/opencv.hpp>

#include "weighted_value.h"

struct path_result
{
    std::vector<int> path;
    int total_energy;
};

typedef weighted_value<int, int> weighted_int_t;

// find a horizontal path whose energy is minimum
template <typename COMPARE_FUNC = std::less<int> >
path_result find_min_energy_path_hori(const cv::Mat &energy_image,
                                      COMPARE_FUNC cmp = std::less<int>(),
                                      weighted_int_t *buffer = nullptr)
{
    weighted_int_t *dp;
    if (!buffer)
    {
        dp = new weighted_int_t[energy_image.cols * energy_image.rows];
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

    // last level: find minimum.
    weighted_int_t e_min { last_level[1].weight, 1 };
    for (int y = 2; y < energy_image.rows - 1; ++y)
    {
        if (cmp(last_level[y].weight, e_min.weight))
        {
            e_min.weight = last_level[y].weight;
            e_min.value = y;
        }
    }

    // backtrace
    std::vector<int> energy_min_path(energy_image.cols);
    energy_min_path[energy_image.cols - 1] = e_min.value;
    for (int x = energy_image.cols - 2; x >= 0; --x)
    {
        energy_min_path[x] = last_level[energy_min_path[x + 1]].value;
        last_level -= energy_image.rows;
    }

    if (!buffer)
    {
        delete [] dp;
    }

    return path_result { std::move(energy_min_path), e_min.weight };
}

cv::Mat remove_path_hori(const cv::Mat &image, const std::vector<int> &path);
cv::Mat dup_path_hori(const cv::Mat &image, const std::vector<int> &path);

#endif // _SEAM_CARVING_HPP_