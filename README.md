# Interpolacja Bilinearna przy pomocy karty graficznej i frameworka OpenCL
Marek Kasprowicz

# Interpolacja Bilinearna [^1]

Proces stosowany przy skalowaniu zbiorów danych, najczęściej obrazów. Polega on na wyznaczniu wartości dla nowej struktury danych w oparciu o informacje zawarte w pierwszej. 

![obraz](https://user-images.githubusercontent.com/67783947/214377688-c7ffba05-5f7d-4443-bfb1-53f71f2a41e7.png)

![obraz](https://user-images.githubusercontent.com/67783947/214378363-b7889de7-4306-46a0-b8bd-8c7442ad65f6.png)


# OpenCL [^3][^4]

Opensourcowy framework stosowany do tworzenia oprogramowania na różnych ALU. W przeciwieństwie do CUDA nie jest ograniczony tylko do konkretnego rodzaju hardware. Raz napisany kod w OpenCL można uruchomić na niemalże każdym sprzęcie, wliczając w to GPU, CPU lub FPGA). Aktaulnie jest w posiadaniu grupy Khronos i najwyższą jego wersją jest 3.0. Natywnie kod OpenCL tworzy się za pomocą języka C, jednak Khronos wprowadził headery interpretujące polecenia na obiekty C++, co znacznie poprawiło czytelność kodu i zmniejszyło "biolerplate". 
Istnieją również nakładki na inne języki takie jak:

  1. Python - https://pypi.org/project/pyopencl/
  2. Java - https://github.com/gpu/JOCL
  3. C# - https://www.nuget.org/packages/OpenCL.Net
  4. C++ - https://github.com/KhronosGroup/OpenCL-CLHPP

# Omówienie kodu

## Platformy i urządzenia

## Kontekst, kolejka zadań, program i kod jądra

## Kernel

## Załadowanie i konwersja obrazu dla GPU
Template project for image processing in OpenCL[^5]. Uses OpenCV to input and output files. 
Program loads image to OpenCV Mat, then maps it as RGBA to 1D array of structure:

```
  pixel: 0, 0     pixel: 0, 1   pixel: n, n
{ r, g, b, a, r, g, b, a, ... r, g, b, a }
```
Image is enqueued to GPU device as OpenCL Image2D object, and copies it to output image.
GPU returns array of same structure as above, that is parsed to Mat object and converted again to bgr.

## Wywołanie programu jądra

## Odczyt danych i zapis obrazu

# Biblioteki, kompilacja i uruchamianie

## OpenCL
```
sudo apt install opencl-headers ocl-icd-opencl-dev -y
```

## OpenCV
```
sudo apt install python3-opencv libopencv-dev 
```

## Kompilacja
```
g++ main.cpp -lOpenCL `pkg-config --cflags --libs opencv4`
```

## Wywołanie
```
./a.out [plik_wejsciowy] [plik_wyjsciowy] [rozmiar_nowego_pliku]
```
- [plik_wejsciowy] - Nazwa pliku wejściowego. Np. "in.png"
- [plik_wyjsciowy] - Nazwa pliku wyjściowego. Np. "out.png"
- [rozmiar_nowego_pliku] - Miara do której będzie skalowany obraz, podany w pixelach. Np. 100 

# Komentarz osobisty
![obraz](https://user-images.githubusercontent.com/67783947/214385523-95a341e2-3cbd-456c-84bf-f466d6a8860c.png)


# Źródła 

[^1]: Bilinear Interpolation Calculator https://www.omnicalculator.com/math/bilinear-interpolation
[^2]: Understanding Bilinear Image Resizing https://chao-ji.github.io/jekyll/update/2018/07/19/BilinearResize.html
[^3]: Khronos OpenCL Guide: https://github.com/KhronosGroup/OpenCL-Guide
[^4]: OpenCL Programming by Example By Ravishekhar Banger, Koushik Bhattacharyya https://www.packtpub.com/product/opencl-programming-by-example/9781849692342
[^5]: OpenCV C++ Tutorial https://www.opencv-srf.com/2017/11/load-and-display-image.html
