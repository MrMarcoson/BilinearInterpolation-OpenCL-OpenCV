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

string get_kernel(string file) {
    std::ifstream stream(file);
    std::stringstream kernel;
    kernel << stream.rdbuf();
    
    return kernel.str();
}

void setup_device(cl::Platform &platform, cl::Device &device) {
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() == 0) {
        std::cout << " 0 platforms \n";
        exit(1);
    }

    platform = all_platforms[0];

    std::vector<cl::Device> all_devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if (all_devices.size() == 0) {
        std::cout << " 0 devices\n";
        exit(1);
    }

    device = all_devices[0];
}

int main(int argc, char **argv) {
    string input_file_name = (string)argv[1];
    string output_file_name = (string)argv[2];
    long int image_size = atoi(argv[3]); 

    auto start = std::chrono::high_resolution_clock::now();

    cl::Platform platform;
    cl::Device device;
    setup_device(platform, device);
    std::cout << platform.getInfo<CL_PLATFORM_NAME>() << ": " << device.getInfo<CL_DEVICE_NAME>() << "\n";

    cl::Context context({ device });
    cl::CommandQueue queue(context, device, CL_QUEUE_PROFILING_ENABLE);

    cl::Program::Sources sources;
    string kernel_code = get_kernel("bilinear_interpolation.cl");
    sources.push_back({ kernel_code.c_str(), kernel_code.length() });

    cl::Program program(context, sources);
    if(program.build({ device }) != CL_SUCCESS) {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
        exit(1);
    }

    cl::Kernel kernel(program, "bilinear_interpolation");

    cv::Mat input_image = load_image(input_file_name);
    std::vector<uchar> input_arr;

    if(input_image.isContinuous()){
        input_arr.assign(input_image.datastart, input_image.dataend);
    }

    cl::ImageFormat format(CL_RGBA, CL_UNORM_INT8);
	cl::Image2D Input_Image(context, CL_MEM_READ_ONLY, format, input_image.size().width, input_image.size().height);
	cl::Image2D Output_Image(context, CL_MEM_WRITE_ONLY, format, image_size, image_size);

	std::array<size_t, 3> origin { 0, 0, 0 };
    std::array<size_t, 3> input_region { input_image.size().width, input_image.size().height, 1 };
	std::array<size_t, 3> output_region { image_size, image_size, 1 };

	queue.enqueueWriteImage(Input_Image, CL_TRUE, origin, input_region, 0, 0, &input_arr[0]);
	
    float x_ratio = ((float)(input_image.size().width - 1)  / (float)(image_size - 1));
    float y_ratio = ((float)(input_image.size().height - 1)  / (float)(image_size - 1));

    kernel.setArg(0, Input_Image);
	kernel.setArg(1, Output_Image);
    kernel.setArg(2, sizeof(float), &x_ratio);
	kernel.setArg(3, sizeof(float), &y_ratio);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(image_size, image_size), cl::NullRange, NULL);

    std::vector<uchar> output_arr(image_size * image_size * 4);
    queue.enqueueReadImage(Output_Image, CL_TRUE, origin, output_region, 0, 0, &output_arr[0]);
    cv::Mat output_image(image_size, image_size, CV_8UC4, output_arr.data());
    cv::cvtColor(output_image, output_image, cv::COLOR_RGBA2BGRA);
    cv::imwrite(output_file_name, output_image);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Execution time " << elapsed_seconds.count() << "s" << std::endl;

    return 0;
}
