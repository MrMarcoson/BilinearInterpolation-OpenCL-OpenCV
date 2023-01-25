/*
    Marek Kasprowicz, 303020
    Zadanie na Programowanie GPU

    Zadanie wykonane przy pomocy OpenCL, wymagane jest urzadzenie obslugujace ta biblioteke i sterowniki do niego.
    Program byl testowany na karcie AMD Radeon 6700XT.

    Na podstawie: https://www.omnicalculator.com/math/bilinear-interpolation

    Zalenosci:
    sudo apt install opencl-headers ocl-icd-opencl-dev -y
    sudo apt install python3-opencv libopencv-dev 

    Kompilacja:
    g++ main.cpp -lOpenCL `pkg-config --cflags --libs opencv4`

    Wywolanie
    ./a.out [plik_wejsciowy] [plik_wyjsciowy] [rozmiar_nowego_pliku (np. 100)]
*/
#include <math.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <CL/cl2.hpp>
#include <opencv2/opencv.hpp>
#include <chrono>
#include "GPU.cpp"
#include "CPU.cpp"

using namespace std;

cv::Mat load_image(string file_name) {
    cv::Mat image = cv::imread(file_name);
    if (image.empty()) {
        cout << "Image not loaded" << endl;
    }

    cv::cvtColor(image, image, cv::COLOR_BGR2RGBA);
    return image;
}

int main(int argc, char **argv) {
    string input_file_name = (string)argv[1];
    string output_file_name = (string)argv[2];
    long int image_size = atoi(argv[3]);
    int test = atoi(argv[4]);

    cv::Mat input_image = load_image(input_file_name);
    float x_ratio = ((float)(input_image.size().width - 1)  / (float)(image_size - 1));
    float y_ratio = ((float)(input_image.size().height - 1)  / (float)(image_size - 1));

    double GPU_time = GPU(input_image, image_size, x_ratio, y_ratio, output_file_name, test);
    std::cout << "GPU execution time " << GPU_time << "s" << std::endl;

    double CPU_time = CPU(input_image, image_size, x_ratio, y_ratio, output_file_name, test);
    std::cout << "CPU execution time " << CPU_time << "s" << std::endl;

    if(test == 1) {
        fstream gpu_test_data;
        gpu_test_data.open("gpu.csv", ios::out|ios::binary);
        fstream cpu_test_data;
        cpu_test_data.open("cpu.csv", ios::out|ios::binary);

        for(int i = 100; i <= 10000; i += 100) {
            image_size = i;
            float x_ratio = ((float)(input_image.size().width - 1)  / (float)(image_size - 1));
            float y_ratio = ((float)(input_image.size().height - 1)  / (float)(image_size - 1));

            gpu_test_data << i << ";" << GPU(input_image, image_size, x_ratio, y_ratio, output_file_name, 1) << std::endl;
            cpu_test_data << i << ";" << CPU(input_image, image_size, x_ratio, y_ratio, output_file_name, 1) << std::endl;
        }

        gpu_test_data.close();
        cpu_test_data.close();
    }

    return 0;
}
