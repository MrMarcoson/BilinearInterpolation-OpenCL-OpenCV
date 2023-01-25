#include <math.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <CL/cl2.hpp>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;

void CPU(cv::Mat input_image, long int image_size, string output_file_name, float x_ratio, float y_ratio) {
    auto start = std::chrono::high_resolution_clock::now();

    cv::Mat output_image(image_size, image_size, input_image.type(), cv::Scalar(255, 0, 0, 0));;
    cv::cvtColor(output_image, output_image, cv::COLOR_BGR2RGBA);

    for(int i = 0; i < image_size; i++) {
        for(int j = 0; j < image_size; j++) {
            int x0 = floor(x_ratio * j);
            int y0 = floor(y_ratio * i);
            int x1 = ceil(x_ratio * j);
            int y1 = ceil(y_ratio * i);

            double xw = (x_ratio * j) - x0;
            double yw = (y_ratio * i) - y0;

            cv::Vec4b a = input_image.at<cv::Vec4b>(y0, x0);
            cv::Vec4b b = input_image.at<cv::Vec4b>(y0, x1);
            cv::Vec4b c = input_image.at<cv::Vec4b>(y1, x0);
            cv::Vec4b d = input_image.at<cv::Vec4b>(y1, x1);

            cv::Vec4b a0;
            cv::Vec4b b0;
            cv::Vec4b c0;
            cv::Vec4b d0;

            for(int i = 0; i < 4; i++) {
                a0[i] = a[i] * (1 - xw) * (1 - yw);
                b0[i] = b[i] * xw * (1 - yw);
                c0[i] = c[i] * yw * (1 - xw);
                d0[i] = d[i] * xw * yw;
            }

            cv::Vec4b output_color(0, 0, 0);
            for(int i = 0; i < 4; i++) {
                output_color[i] = a0[i] + b0[i] + c0[i] + d0[i];
            }

            output_image.at<cv::Vec4b>(i, j) = cv::Vec4b(output_color[0], output_color[1], output_color[2], output_color[3]);
        }
    }

    cv::cvtColor(output_image, output_image, cv::COLOR_RGBA2BGRA);
    cv::imwrite(output_file_name, output_image);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "CPU Execution time " << elapsed_seconds.count() << "s" << std::endl;
}