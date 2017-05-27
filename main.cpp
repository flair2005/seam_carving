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

void test_insert()
{
    std::string filename = "test.jpg";
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (!image.data)
    {
        std::cout << "Open input file failed." << std::endl;
        exit(1);
    }

    int dr = 600, dc = 0;
    int r = 0, c = 0;

    weighted_int_t *dp_buffer = new weighted_int_t[(image.cols + dc + 1) * (image.rows + dr + 1)];
    while (r + c < dr + dc)
    {
        fprintf(stderr, "\rProcessing... %5.2f%%", (r + c) * 100.0 / (dr + dc));
        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
        //cv::imwrite("gray_image.png", gray_image);
        cv::Mat energy_image = sobel_energy(gray_image);
        cv::imwrite("sobel_energy_image.png", energy_image);
        // cv::imwrite("laplacian_energy_image.png", laplacian_energy(gray_image));

        if (r == dr) // c != dc
        {
            path_result p = find_vert_seam(energy_image, dp_buffer);
            image = remove_path_vert(image, p.path);
            ++c;
        }
        else if (c == dc) // r != dr
        {
            path_result p = find_hori_seam(energy_image, dp_buffer, dr - r);
            image = insert_path_hori(image, p.path);
            ++r;
        }
        else // r != dr && c != dc
        {
            path_result pv = find_vert_seam(energy_image, dp_buffer),
                ph = find_hori_seam(energy_image, dp_buffer);
            // 贪心
            if (pv.total_energy < ph.total_energy)
            {
                image = remove_path_vert(image, pv.path);
                ++c;
                printf("|");
            }
            else
            {
                image = insert_path_hori(image, ph.path);
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
    delete[] dp_buffer;
    cv::imwrite(filename + ".result.png", image);
}

int main()
{
    //test_insert();
    //return 0;

    std::string filename = "testcase/1.jpg";
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (!image.data)
    {
        std::cout << "Open input file failed." << std::endl;
        exit(1);
    }

    int dr = 220, dc = 300;
    int r = 0, c = 0;

    weighted_int_t *dp_buffer = new weighted_int_t[(image.cols + 1) * (image.rows + 1)];
    while (r + c < dr + dc)
    {
        //fprintf(stderr, "\rProcessing... %5.2f%%", (r + c) * 100.0 / (dr + dc));
        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
        cv::Mat energy_image = sobel_energy(gray_image);

        if (r + c == 0)
        {
            cv::imwrite("energy_image.result.png", energy_image);
        }

        if (r == dr) // c != dc
        {
            path_result p = find_vert_seam(energy_image, dp_buffer);
            image = remove_path_vert(image, p.path);
            ++c;
            fprintf(stderr, "|");
        }
        else if (c == dc) // r != dr
        {
            path_result p = find_hori_seam(energy_image, dp_buffer);
            image = remove_path_hori(image, p.path);
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
                image = remove_path_vert(image, pv.path);
                ++c;
                fprintf(stderr, "|");
            }
            else
            {
                image = remove_path_hori(image, ph.path);
                ++r;
                fprintf(stderr, "-");
            }
        }

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
