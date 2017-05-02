#include <iostream>
#include <string>
#include <tuple>
#include <opencv2/opencv.hpp>

typedef std::tuple<int, std::vector<int> > dp_t;

bool cmp_lt(int a, int b)
{
    return a < b;
}

bool cmp_gt(int a, int b)
{
    return a > b;
}

template <typename COMPARE_FUNC>
dp_t find_min_energy_path_x(const cv::Mat &energy_image_x, COMPARE_FUNC cmp)
{
    dp_t *energy = new dp_t[energy_image_x.rows], *last_energy = new dp_t[energy_image_x.rows];
    for (int y = 1; y < energy_image_x.rows - 1; ++y)
    {
        last_energy[y] = dp_t(energy_image_x.at<unsigned char>(y, 0), std::vector<int>());
    }
    for (int x = 1; x < energy_image_x.cols; ++x)
    {
        for (int y = 1; y < energy_image_x.rows - 1; ++y)
        {
            // (x - 1, y)
            int energy_min = std::get<0>(last_energy[y]);
            int energy_min_y = y;

            // (x - 1, y - 1)
            if (y - 1 >= 1 && cmp(std::get<0>(last_energy[y - 1]), energy_min))
            {
                energy_min = std::get<0>(last_energy[y - 1]);
                energy_min_y = y - 1;
            }

            // (x - 1, y + 1)
            if (y + 1 < energy_image_x.rows - 1 && cmp(std::get<0>(last_energy[y + 1]), energy_min))
            {
                energy_min = std::get<0>(last_energy[y + 1]);
                energy_min_y = y + 1;
            }

            energy_min += energy_image_x.at<unsigned char>(y, x);

            dp_t &e = energy[y] = dp_t(energy_min,
                std::vector<int>(std::get<1>(last_energy[energy_min_y])));
            std::get<1>(e).push_back(energy_min_y);

        }
        std::swap(energy, last_energy);
    }

    int energy_min = std::get<0>(last_energy[1]);
    int energy_min_y = 1;
    for (int y = 2; y < energy_image_x.rows - 1; ++y)
    {
        if (cmp(std::get<0>(last_energy[y]), energy_min))
        {
            energy_min = std::get<0>(last_energy[y]);
            energy_min_y = y;
        }
    }

    std::vector<int> energy_min_path(std::get<1>(last_energy[energy_min_y]));
    energy_min_path.push_back(energy_min_y);

    // assert energy_min_path.size() == energy_image_x.cols()

    delete[] energy;
    delete[] last_energy;

    std::cout << energy_min << std::endl;
    return dp_t(energy_min, energy_min_path);
}

cv::Mat remove_path_x(const cv::Mat &image, const std::vector<int> &path)
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

cv::Mat dup_path_x(const cv::Mat &image, const std::vector<int> &path)
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

int main()
{
    std::string filename = "test.jpg";
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (!image.data)
    {
        std::cout << "Open input file failed." << std::endl;
        exit(1);
    }

    for (int i = 0; i < 600; ++i)
    {
        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

        //cv::imwrite("gray_image.png", gray_image);
        cv::Mat energy_image_x;
        cv::Sobel(gray_image, energy_image_x, -1, 1, 0, 3);
        //cv::imwrite("energy_image_x.png", energy_image_x);
        cv::Mat energy_image_y;
        cv::Sobel(gray_image, energy_image_y, -1, 1, 1, 3);
        //cv::imwrite("energy_image_y.png", energy_image_y);

        dp_t ex = find_min_energy_path_x(energy_image_y, cmp_lt);
        // TODO: ey = find_min_energy_path_y(energy_image_y);
        std::vector<int> &energy_min_path = std::get<1>(ex);

        /*for (int x = 0; x < image.cols; ++x)
        {
            image.at<unsigned char>(energy_min_path[x], 3 * x + 0) = 0;
            image.at<unsigned char>(energy_min_path[x], 3 * x + 1) = 0;
            image.at<unsigned char>(energy_min_path[x], 3 * x + 2) = 255;
        }*/
        image = remove_path_x(image, energy_min_path);
        //cv::imwrite(std::string("result") + (char)('A' + i) + ".png", image);
    }
    cv::imwrite("result.png", image);
    return 0;
}

