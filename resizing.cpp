#include "resizing.h"
#include "seam_carving.h"

void image_extend(cv::Mat &image, int dc, int dr)
{
    int r = 0, c = 0;

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::Mat energy_image = sobel_energy(gray_image);

    weighted_int_t *dp_buffer = new weighted_int_t[(image.cols + dc + 1) * (image.rows + dr + 1)];
    while (r + c < dr + dc)
    {
        fprintf(stderr, "\rProcessing... %5.2f%%", (r + c) * 100.0 / (dr + dc));

        if (r == dr) // c != dc
        {
            path_result p = find_vert_seam(energy_image, dp_buffer);
            set_path_vert<1, energy_t>(energy_image, p.path, std::vector<energy_t> {30000});
            image = insert_path_vert<3, unsigned char>(image, p.path);
            energy_image = insert_path_vert<1, energy_t>(energy_image, p.path);
            // 修改能量为“无穷大”，防止再次被选中
            set_path_vert<1, energy_t>(energy_image, p.path, std::vector<energy_t> {30000});
            ++c;
            fprintf(stderr, "|");
        }
        else if (c == dc) // r != dr
        {
            path_result p = find_hori_seam(energy_image, dp_buffer);
            set_path_hori<1, energy_t>(energy_image, p.path, std::vector<energy_t> {30000});
            image = insert_path_hori<3, unsigned char>(image, p.path);
            energy_image = insert_path_hori<1, energy_t>(energy_image, p.path);
            // 修改能量为“无穷大”，防止再次被选中
            set_path_hori<1, energy_t>(energy_image, p.path, std::vector<energy_t> {30000});
            ++r;
            fprintf(stderr, "-");
        }
        else // r != dr && c != dc
        {
            path_result pv = find_vert_seam(energy_image, dp_buffer),
                        ph = find_hori_seam(energy_image, dp_buffer);
            // 贪心
            if (pv.total_energy < ph.total_energy)
            {
                set_path_vert<1, energy_t>(energy_image, pv.path, std::vector<energy_t> {30000});
                image = insert_path_vert<3, unsigned char>(image, pv.path);
                energy_image = insert_path_vert<1, energy_t>(energy_image, pv.path);
                // 修改能量为“无穷大”，防止再次被选中
                set_path_vert<1, energy_t>(energy_image, pv.path, std::vector<energy_t> {30000});
                ++c;
                fprintf(stderr, "|");
            }
            else
            {
                set_path_hori<1, energy_t>(energy_image, ph.path, std::vector<energy_t> {30000});
                image = insert_path_hori<3, unsigned char>(image, ph.path);
                energy_image = insert_path_hori<1, energy_t>(energy_image, ph.path);
                // 修改能量为“无穷大”，防止再次被选中
                set_path_hori<1, energy_t>(energy_image, ph.path, std::vector<energy_t> {30000});
                ++r;
                fprintf(stderr, "-");
            }
        }
    }
    delete[] dp_buffer;
}

void image_shrink(cv::Mat &image, int dc, int dr)
{
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    int r = 0, c = 0;

    weighted_int_t *dp_buffer = new weighted_int_t[(image.cols + 1) * (image.rows + 1)];
    while (r + c < dr + dc)
    {
        cv::Mat energy_image = sobel_energy(gray_image);

        if (r + c == 0)
        {
            cv::imwrite("energy_image.result.png", energy_image);
        }

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
            // 贪心
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
    delete[] dp_buffer;
}

void draw_seam(cv::Mat &image, int nc, int nr, const std::vector<unsigned char> &color)
{
    int r = 0, c = 0;

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::Mat energy_image = sobel_energy(gray_image);

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
            set_path_hori<1, energy_t>(energy_image, p.path, std::vector<energy_t> {30000});
            set_path_hori<3, unsigned char>(image, p.path, color);
            ++r;
            fprintf(stderr, "-");
        }
        else // r != nr && c != nc
        {
            path_result pv = find_vert_seam(energy_image, dp_buffer),
                        ph = find_hori_seam(energy_image, dp_buffer);
            // 贪心
            if (pv.total_energy < ph.total_energy)
            {
                set_path_vert<1, energy_t>(energy_image, pv.path, std::vector<energy_t> {30000});
                set_path_vert<3, unsigned char>(image, pv.path, color);
                ++c;
                fprintf(stderr, "|");
            }
            else
            {
                set_path_hori<1, energy_t>(energy_image, ph.path, std::vector<energy_t> {30000});
                set_path_hori<3, unsigned char>(image, ph.path, color);
                ++r;
                fprintf(stderr, "-");
            }
        }
    }
    delete[] dp_buffer;
}