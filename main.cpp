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

cv::Mat load_image(string file_name)
{
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

    cv::Mat input_image = load_image(input_file_name);
    float x_ratio = ((float)(input_image.size().width - 1)  / (float)(image_size - 1));
    float y_ratio = ((float)(input_image.size().height - 1)  / (float)(image_size - 1));

    GPU(input_image, image_size, "gpu_" + output_file_name, x_ratio, y_ratio);
    CPU(input_image, image_size, "cpu_" + output_file_name, x_ratio, y_ratio);

    return 0;
}
