#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <sstream>

#include "seam_carving.h"

std::string make_filename(int i)
{
    std::stringstream ss;
    ss << "result_" << i << ".png";
    return ss.str();
}

int main()
{
    std::string filename = "cat_hd.jpg";
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (!image.data)
    {
        std::cout << "Open input file failed." << std::endl;
        exit(1);
    }

    int dr = 1000, dc = 1000;
    int r = 0, c = 0;

    weighted_int_t *dp_buffer = new weighted_int_t[image.cols * image.rows];
    while (r + c < dr + dc)
    {
        //fprintf(stderr, "\rProcessing... %5.2f%%", (r + c) * 100.0 / (dr + dc));
        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
        //cv::imwrite("gray_image.png", gray_image);
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

        if (r == dr) // c != dc
        {
            path_result p = find_vert_seam(energy_image, dp_buffer);
            image = remove_path_vert(image, p.path);
            ++c;
        }
        else if (c == dc) // r != dr
        {
            path_result p = find_hori_seam(energy_image, dp_buffer);
            image = remove_path_hori(image, p.path);
            ++r;
        }
        else // r != dr && c != dc
        {
            path_result pv = find_vert_seam(energy_image, dp_buffer),
                        ph = find_hori_seam(energy_image, dp_buffer);
            // Ì°ÐÄ
            if (pv.total_energy < ph.total_energy)
            {
                image = remove_path_vert(image, pv.path);
                ++c;
                printf("|");
            }
            else
            {
                image = remove_path_hori(image, ph.path);
                ++r;
                printf("-");
            }
        }
        
        //cv::imwrite("energy_image.png", energy_image);

        // TODO: ey = find_min_energy_path_y(energy_image_y);

        /*for (int x = 0; x < image.cols; ++x)
        {
            image.at<unsigned char>(ph.path[x], 3 * x + 0) = 0;
            image.at<unsigned char>(ph.path[x], 3 * x + 1) = 0;
            image.at<unsigned char>(ph.path[x], 3 * x + 2) = 255;
        }*/
        
        //cv::imwrite(make_filename(i), image);
    }
    delete [] dp_buffer;
    cv::imwrite(filename + ".result.png", image);
    return 0;
}
