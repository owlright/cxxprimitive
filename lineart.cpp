#include <math.h>
#include <cstdlib> // 引入标准库以使用rand()和srand()
#include <ctime> // 引入时间库以使用time()
#include <iostream>
#include <vector>
#include <utility> // 引入utility库以使用std::pair
#include "image.h"
#include "stb_image.h"
#include "line.h"
#include "threadpool.h"
#include "timer.h"

using namespace primitive;

std::pair<int, int> PickTwoUniqueNumbers()
{
    int first = rand() % 4; // 生成1到4之间的随机数作为第一个数字
    int second;
    do {
        second = rand() % 4; // 生成第二个数字，直到它与第一个数字不同
    } while (second == first);
    return std::make_pair(first, second); // 返回一对不重复的数字
}

double Energy(const RasterizedLines* lines, const unsigned char* grayImage, int width, int height)
{
    double energy = 0;

    for (int i = 0; i < lines->h; i++) {
        auto line = lines->lines[i];
        for (int x = line.left; x <= line.right; x++) {
            energy += pow(grayImage[line.y * width + x] - lines->r, 2);
        }
    }
    return sqrt(energy);
}

std::vector<int> generatePositions(int width, int height)
{
    auto [first, second] = PickTwoUniqueNumbers();

    std::vector<int> positions(4, -1);
    while (!(positions[0] != positions[2] && positions[1] != positions[3])) {
        for (int i = 0; i < 4; i++) {
            if (i == 0 || i == 2) {
                positions[i] = rand() % width;
            } else {
                positions[i] = rand() % height;
            }
        }
        switch (first) {
        case 1:
            positions[1] = 0;
            break;
        case 2:
            positions[0] = width - 1;
            break;
        case 3:
            positions[1] = height - 1;
            break;
        case 4:
            positions[0] = 0;
            break;
        }
        switch (second) {
        case 1:
            positions[3] = 0;
            break;
        case 2:
            positions[2] = width - 1;
            break;
        case 3:
            positions[3] = height - 1;
            break;
        case 4:
            positions[2] = 0;
            break;
        }
    }

    return positions;
}

int main()
{
    const char* imagePath = "../images/starrynight.jpg";
    int width, height, nChannels;
    // stbi_set_flip_vertically_on_load(1);
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &nChannels, 0);

    printf("width: %d, height: %d, nrComponents: %d\n", width, height, nChannels);
    auto grayImage = rgb2grayMerged(imageData, width, height);
    Line l(145, 0, 0, 34);
    RasterizedLines lines = l.Rasterize();
    const int try_number = 10000;
    // Display(&lines);
    ThreadPool pool(6);
    // enqueue and store future
    {
        std::cout << "Using ThreadPool" << std::endl;
        Timer _;
        for (auto i = 0; i < try_number; i++) {
            srand(i);
            auto positions = generatePositions(width, height);
            // std::cout << positions[0] << " " << positions[1] << " " << positions[2] << " " << positions[3]  << std::endl;
            Line l(positions);
            RasterizedLines lines = l.Rasterize();
            lines.r = rand() % 256;
            lines.g = rand() % 256;
            lines.b = rand() % 256;
            auto result = pool.enqueue(Energy, &lines, grayImage, width, height);
            auto energy = result.get();

            // std::cout << "energy: " << energy << std::endl;
        }
    }
    {
        std::cout << "Not using ThreadPool" << std::endl;
        Timer _;
        for (auto i = 0; i < try_number; i++) {
            srand(i);

            auto positions = generatePositions(width, height);

            Line l(positions);
            RasterizedLines lines = l.Rasterize();
            lines.r = rand() % 256;
            lines.g = rand() % 256;
            lines.b = rand() % 256;
            auto result = pool.enqueue(Energy, &lines, grayImage, width, height);
            auto energy = result.get();
        }
    }
    DestroyRasterizedLines(&lines);
    free(imageData);
    free(grayImage);
}
