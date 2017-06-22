#include "resizing.h"
#include "seam_carving.hpp"

void image_extend(cv::Mat &image, int dc, int dr, energy_func_t energy_func)
{
    int r = 0, c = 0;

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::Mat energy_image = energy_func(gray_image);

    weighted_int_t *dp_buffer = new weighted_int_t[(image.cols + dc + 1) * (image.rows + dr + 1)];
    while (r + c < dr + dc)
    {
        if (r == dr) // c != dc
        {
            path_result p = find_vert_seam(energy_image, dp_buffer);
            set_path_vert<1, energy_t>(energy_image, p.path, std::vector<energy_t> { energy_inf });
            image = insert_path_vert<3, unsigned char>(image, p.path);
            energy_image = insert_path_vert<1, energy_t>(energy_image, p.path);
            // �޸�����Ϊ������󡱣���ֹ�ٴα�ѡ��
            set_path_vert<1, energy_t>(energy_image, p.path, std::vector<energy_t> { energy_inf });
            ++c;
            fprintf(stderr, "|");
        }
        else if (c == dc) // r != dr
        {
            path_result p = find_hori_seam(energy_image, dp_buffer);
            set_path_hori<1, energy_t>(energy_image, p.path, std::vector<energy_t> { energy_inf });
            image = insert_path_hori<3, unsigned char>(image, p.path);
            energy_image = insert_path_hori<1, energy_t>(energy_image, p.path);
            // �޸�����Ϊ������󡱣���ֹ�ٴα�ѡ��
            set_path_hori<1, energy_t>(energy_image, p.path, std::vector<energy_t> { energy_inf });
            ++r;
            fprintf(stderr, "-");
        }
        else // r != dr && c != dc
        {
            path_result pv = find_vert_seam(energy_image, dp_buffer),
                        ph = find_hori_seam(energy_image, dp_buffer);
            // ̰��
            if (pv.total_energy < ph.total_energy)
            {
                set_path_vert<1, energy_t>(energy_image, pv.path, std::vector<energy_t> { energy_inf });
                image = insert_path_vert<3, unsigned char>(image, pv.path);
                energy_image = insert_path_vert<1, energy_t>(energy_image, pv.path);
                // �޸�����Ϊ������󡱣���ֹ�ٴα�ѡ��
                set_path_vert<1, energy_t>(energy_image, pv.path, std::vector<energy_t> { energy_inf });
                ++c;
                fprintf(stderr, "|");
            }
            else
            {
                set_path_hori<1, energy_t>(energy_image, ph.path, std::vector<energy_t> { energy_inf });
                image = insert_path_hori<3, unsigned char>(image, ph.path);
                energy_image = insert_path_hori<1, energy_t>(energy_image, ph.path);
                // �޸�����Ϊ������󡱣���ֹ�ٴα�ѡ��
                set_path_hori<1, energy_t>(energy_image, ph.path, std::vector<energy_t> { energy_inf });
                ++r;
                fprintf(stderr, "-");
            }
        }
    }
    delete [] dp_buffer;
}

void image_shrink(cv::Mat &image, int dc, int dr, energy_func_t energy_func)
{
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    int r = 0, c = 0;

    weighted_int_t *dp_buffer = new weighted_int_t[(image.cols + 1) * (image.rows + 1)];
    while (r + c < dr + dc)
    {
        cv::Mat energy_image = energy_func(gray_image);

        if (r == dr) // c != dc
        {
            path_result p = find_vert_seam(energy_image, dp_buffer);
            image = remove_path_vert<3, unsigned char>(image, p.path);
            gray_image = remove_path_vert<1, unsigned char>(gray_image, p.path);
            ++c;
            fprintf(stderr, "|");
        }
        else if (c == dc) // r != dr
        {
            path_result p = find_hori_seam(energy_image, dp_buffer);
            image = remove_path_hori<3, unsigned char>(image, p.path);
            gray_image = remove_path_hori<1, unsigned char>(gray_image, p.path);
            ++r;
            fprintf(stderr, "-");
        }
        else // r != dr && c != dc
        {
            path_result pv = find_vert_seam(energy_image, dp_buffer),
                        ph = find_hori_seam(energy_image, dp_buffer);
            // ̰��
            if (pv.total_energy < ph.total_energy)
            {
                image = remove_path_vert<3, unsigned char>(image, pv.path);
                gray_image = remove_path_vert<1, unsigned char>(gray_image, pv.path);
                ++c;
                fprintf(stderr, "|");
            }
            else
            {
                image = remove_path_hori<3, unsigned char>(image, ph.path);
                gray_image = remove_path_hori<1, unsigned char>(gray_image, ph.path);
                ++r;
                fprintf(stderr, "-");
            }
        }
    }
    delete [] dp_buffer;
}

void image_resize(cv::Mat &image, int new_width, int new_height, energy_func_t energy_func)
{
    int dc = new_width - image.cols,
        dr = new_height - image.rows;
    if (dc <= 0 && dr <= 0)
    {
        image_shrink(image, -dc, -dr, energy_func);
    }
    else if (dc > 0 && dr > 0)
    {
        image_extend(image, dc, dr, energy_func);
    }
    else if (dc <= 0 && dr > 0)
    {
        // TODO: order?
        image_shrink(image, -dc, 0, energy_func);
        image_extend(image, 0, dr, energy_func);
    }
    else if (dc > 0 && dr <= 0)
    {
        // TODO: order?
        image_shrink(image, 0, -dr, energy_func);
        image_extend(image, dc, 0, energy_func);
    }
    else
    {
        // assert false
    }
}