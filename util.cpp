#include "util.h"
#include "seam_carving.hpp"

cv::Mat load_mask(const std::string &filename)
{
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    return gray_image;
}

void apply_mask(cv::Mat &energy_image, const cv::Mat &mask)
{
    for (int y = 0; y < energy_image.rows; ++y)
    {
        for (int x = 0; x < energy_image.cols; ++x)
        {
            if (!mask.at<unsigned char>(y, x))
            {
                // FIXME: bug, -30000不够小
                energy_image.at<energy_t>(y, x) = -energy_inf;
            }
        }
    }
}

void object_remove(cv::Mat &image, cv::Mat mask, energy_func_t energy_func)
{
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    int r = 0, c = 0;

    weighted_int_t *dp_buffer = new weighted_int_t[(image.cols + 1) * (image.rows + 1)];
    while (true)
    {
        cv::Mat energy_image = energy_func(gray_image);
        apply_mask(energy_image, mask);

        path_result pv = find_vert_seam(energy_image, dp_buffer),
                    ph = find_hori_seam(energy_image, dp_buffer);

        // 所有需要删去的seam都已删去
        if (pv.total_energy > 0 && ph.total_energy > 0)
        {
            break;
        }

        // 贪心
        if (pv.total_energy < ph.total_energy)
        {
            image = remove_path_vert<3, unsigned char>(image, pv.path);
            mask = remove_path_vert<1, unsigned char>(mask, pv.path);
            gray_image = remove_path_vert<1, unsigned char>(gray_image, pv.path);
            ++c;
            fprintf(stderr, "|");
        }
        else
        {
            image = remove_path_hori<3, unsigned char>(image, ph.path);
            mask = remove_path_hori<1, unsigned char>(mask, ph.path);
            gray_image = remove_path_hori<1, unsigned char>(gray_image, ph.path);
            ++r;
            fprintf(stderr, "-");
        }
    }
    delete [] dp_buffer;
}
