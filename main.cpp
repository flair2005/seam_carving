#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <sstream>

#include "util.h"
#include "resizing.h"

std::string make_filename(int i)
{
    std::stringstream ss;
    ss << "result_" << i << ".png";
    return ss.str();
}

energy_func_t get_energy_func(const std::string &name)
{
    if (name == "sobel")
    {
        return &sobel_energy;
    }
    else if (name == "scharr")
    {
        return &scharr_energy;
    }
    else if (name == "laplacian")
    {
        return &laplacian_energy;
    }
    else if (name == "roberts_cross")
    {
        // TODO
        return nullptr;
    }
    return nullptr;
}

int main()
{
    //// draw seams
    //std::vector<std::string> files = 
    //    { "testcase/1.jpg", "testcase/2.png", "testcase/3.jpg",
    //      "testcase/4.jpg", "testcase/5.jpg", "testcase/6.jpg",
    //      "testcase/cat.jpg", "testcase/cat_hd.jpg", "testcase/lena.png",
    //      "testcase/test.jpg", "testcase/twd2.jpg"
    //    };

    //for (const std::string &filename : files)
    //{
    //    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    //    draw_seam(image, 10, 10, std::vector<unsigned char> { 63, 63, 255 },
    //              get_energy_func("sobel"));
    //    cv::imwrite(filename + ".seam.png", image);
    //}

    //{
    //    std::string filename = "testcase/1.jpg";
    //    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    //    image_resize(image, 600, 1200, get_energy_func("sobel"));
    //    cv::imwrite(filename + ".resize.png", image);
    //}

    /*{
        std::string filename = "testcase/1.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        object_remove(image, load_mask("testcase/1_mask.png"), get_energy_func("sobel"));
        cv::imwrite(filename + ".remove.png", image);
    }*/

    /*{
        std::string filename = "testcase/2.png";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 340, 340, get_energy_func("sobel"));
        cv::imwrite(filename + ".resize.png", image);
    }*/

    /*{
        std::string filename = "testcase/2.png";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
            draw_seam(image, 10, 10, std::vector<unsigned char> { 63, 63, 255 },
                      get_energy_func("laplacian"));
        cv::imwrite(filename + ".seam_laplacian.png", image);
    }

    {
        std::string filename = "testcase/2.png";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 340, 340, get_energy_func("laplacian"));
        cv::imwrite(filename + ".resize_laplacian.png", image);
    }*/

    {
        std::string filename = "testcase/2.png";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
        cv::Mat energy_image1 = get_energy_func("sobel")(gray_image);
        cv::imwrite("testcase/2.png.energy_sobel.png", energy_image1);
        cv::Mat energy_image2 = get_energy_func("laplacian")(gray_image);
        cv::imwrite("testcase/2.png.energy_laplacian.png", energy_image2);
    }

    /*{
        std::string filename = "testcase/3.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 270, 225, get_energy_func("sobel"));
        cv::imwrite(filename + ".resize.png", image);
    }

    {
        std::string filename = "testcase/4.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 300, 200, get_energy_func("sobel"));
        cv::imwrite(filename + ".resize.png", image);
    }*/

    /*{
        std::string filename = "testcase/5.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 1000, 400, get_energy_func("sobel"));
        cv::imwrite(filename + ".resize.png", image);
    }*/

    /*{
        std::string filename = "testcase/5.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        object_remove(image, load_mask("testcase/5_mask.png"), get_energy_func("sobel"));
        cv::imwrite(filename + ".remove.png", image);
    }*/

    /*{
        std::string filename = "testcase/6.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 2059, 1260, get_energy_func("sobel"));
        cv::imwrite(filename + ".resize.png", image);
    }*/

    /*{
        std::string filename = "testcase/6.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        object_remove(image, load_mask("testcase/6_mask.png"), get_energy_func("sobel"));
        cv::imwrite(filename + ".remove.png", image);
    }*/

    /*{
        std::string filename = "testcase/cat.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 350, 180, get_energy_func("sobel"));
        cv::imwrite(filename + ".resize.png", image);
    }*/

    /*{
        std::string filename = "testcase/cat.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        object_remove(image, load_mask("testcase/cat_mask.png"), get_energy_func("sobel"));
        cv::imwrite(filename + ".remove.png", image);
    }*/

    /*{
        std::string filename = "testcase/cat_hd.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 2556, 2264, get_energy_func("sobel"));
        cv::imwrite(filename + ".resize.png", image);
    }*/

    /*{
        std::string filename = "testcase/lena.png";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        for (int i = 0; i < 2; ++i)
        {
            image_extend(image, 256, 256, get_energy_func("sobel"));
        }
        cv::imwrite(filename + ".resize.png", image);
    }*/

    /*{
        std::string filename = "testcase/test.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 1920, 1080, get_energy_func("sobel"));
        cv::imwrite(filename + ".resize.png", image);
    }*/
    
    /*{
        std::string filename = "testcase/twd2.jpg";
        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        image_resize(image, 128, 128, get_energy_func("sobel"));
        cv::imwrite(filename + ".resize.png", image);
    }*/

    return 0;
}
