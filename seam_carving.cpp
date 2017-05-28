#include "seam_carving.hpp"

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
    cv::Mat img;
    energy_image.convertTo(img, energy_cv_type, 1.0);
    return img;
}

cv::Mat scharr_energy(const cv::Mat &gray_image)
{
    cv::Mat grad_x;
    cv::Sobel(gray_image, grad_x, CV_16S, 1, 0, CV_SCHARR);
    cv::Mat grad_y;
    cv::Sobel(gray_image, grad_y, CV_16S, 0, 1, CV_SCHARR);
    cv::Mat grad_abs_x;
    cv::convertScaleAbs(grad_x, grad_abs_x);
    cv::Mat grad_abs_y;
    cv::convertScaleAbs(grad_y, grad_abs_y);
    cv::Mat energy_image;
    cv::addWeighted(grad_abs_x, 0.5, grad_abs_y, 0.5, 0.0, energy_image);
    cv::Mat img;
    energy_image.convertTo(img, energy_cv_type, 1.0);
    return img;
}

cv::Mat laplacian_energy(const cv::Mat &gray_image)
{
    cv::Mat grad;
    cv::Laplacian(gray_image, grad, CV_16S, 3);
    cv::Mat grad_abs;
    cv::convertScaleAbs(grad, grad_abs);
    cv::Mat img;
    grad_abs.convertTo(img, energy_cv_type, 1.0);
    return img;
}

void draw_seam(cv::Mat &image, int nc, int nr,
               const std::vector<unsigned char> &color, energy_func_t energy_func)
{
    int r = 0, c = 0;

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::Mat energy_image = energy_func(gray_image);

    weighted_int_t *dp_buffer = new weighted_int_t[(image.cols + 1) * (image.rows + 1)];
    while (r + c < nr + nc)
    {
        if (r == nr) // c != nc
        {
            path_result p = find_vert_seam(energy_image, dp_buffer);
            set_path_vert<1, energy_t>(energy_image, p.path, std::vector<energy_t> {30000});
            set_path_vert<3, unsigned char>(image, p.path, color);
            ++c;
            fprintf(stderr, "|");
        }
        else if (c == nc) // r != nr
        {
            path_result p = find_hori_seam(energy_image, dp_buffer);
            set_path_hori<1, energy_t>(energy_image, p.path, std::vector<energy_t> { energy_inf });
            set_path_hori<3, unsigned char>(image, p.path, color);
            ++r;
            fprintf(stderr, "-");
        }
        else // r != nr && c != nc
        {
            path_result pv = find_vert_seam(energy_image, dp_buffer),
                ph = find_hori_seam(energy_image, dp_buffer);
            // Ì°ÐÄ
            if (pv.total_energy < ph.total_energy)
            {
                set_path_vert<1, energy_t>(energy_image, pv.path, std::vector<energy_t> { energy_inf });
                set_path_vert<3, unsigned char>(image, pv.path, color);
                ++c;
                fprintf(stderr, "|");
            }
            else
            {
                set_path_hori<1, energy_t>(energy_image, ph.path, std::vector<energy_t> { energy_inf });
                set_path_hori<3, unsigned char>(image, ph.path, color);
                ++r;
                fprintf(stderr, "-");
            }
        }
    }
    delete[] dp_buffer;
}