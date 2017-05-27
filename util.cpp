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
                // TODO: bug, -30000²»¹»Ð¡
                energy_image.at<energy_t>(y, x) = -30000;
            }
        }
    }
}

void object_remove(cv::Mat &image, const cv::Mat &mask_)
{
    cv::Mat mask = mask_;

    int r = 0, c = 0;

    weighted_int_t *dp_buffer = new weighted_int_t[(image.cols + 1) * (image.rows + 1)];
    while (true)
    {
        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
        cv::Mat energy_image = sobel_energy(gray_image);
        apply_mask(energy_image, mask);

        path_result pv = find_vert_seam(energy_image, dp_buffer),
                    ph = find_hori_seam(energy_image, dp_buffer);
        if (pv.total_energy > 0 && ph.total_energy > 0)
        {
            break;
        }
        // Ì°ÐÄ
        if (pv.total_energy < ph.total_energy)
        {
            image = remove_path_vert<3, unsigned char>(image, pv.path);
            mask = remove_path_vert<1, unsigned char>(mask, pv.path);
            ++c;
            fprintf(stderr, "|");
        }
        else
        {
            image = remove_path_hori<3, unsigned char>(image, ph.path);
            mask = remove_path_hori<1, unsigned char>(mask, ph.path);
            ++r;
            fprintf(stderr, "-");
        }
    }
    delete [] dp_buffer;
}
