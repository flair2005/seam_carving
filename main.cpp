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

int main()
{
    //test_insert();
    //return 0;

    std::string filename = "testcase/5.jpg";
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (!image.data)
    {
        std::cout << "Open input file failed." << std::endl;
        exit(1);
    }

    object_remove(image, load_mask("testcase/5_mask.png"));

    // draw_seam(image, 20, 20, std::vector<unsigned char> {255, 255, 127});
    /*for (int i = 0; i < 2; ++i)
    {
        image_extend(image, 256, 256);
    }*/
    
    cv::imwrite(filename + ".result.png", image);
    return 0;
}
