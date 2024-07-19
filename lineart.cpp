#include <math.h>
#include <cstdlib> // 引入标准库以使用rand()和srand()
#include <ctime> // 引入时间库以使用time()
#include <iostream>
#include <vector>
#include <utility> // 引入utility库以使用std::pair
#include <filesystem>
#include "image.h"
#include "stb_image.h"
#include "line.h"
#include "threadpool.h"
#include "timer.h"
namespace fs = std::filesystem;
using namespace primitive;
const int try_number = 1000;
std::pair<int, int> PickTwoUniqueNumbers()
{
    int first = rand() % 4; // 生成1到4之间的随机数作为第一个数字
    int second;
    do {
        second = rand() % 4; // 生成第二个数字，直到它与第一个数字不同
    } while (second == first);
    return std::make_pair(first, second); // 返回一对不重复的数字
}

std::pair<double, unsigned char> Energy(const RasterizedLines& lines, const unsigned char* grayImage, int width, int height)
{
    double energy = 0;
    // 计算平均值
    double avg = 0.0;
    int pixelCount = 0;
    for (int i = 0; i < lines.h; i++) {
        auto line = lines.lines[i];
        for (int x = line.left; x <= line.right; x++) {
            avg += grayImage[line.y * width + x];
            pixelCount++;
        }
    }
    if (pixelCount == 0) {
        return std::make_pair(INFINITY, 255);
    }
    avg /= pixelCount;
    for (int i = 0; i < lines.h; i++) {
        auto line = lines.lines[i];
        for (int x = line.left; x <= line.right; x++) {
            energy += pow(grayImage[line.y * width + x] - avg, 2);
        }
    }
    return std::make_pair(sqrt(energy), avg);
}

void FixEdge(int&x, int& y, const int width, const int height, int edgeId)
{
     switch (edgeId) {
        case 0:
           y = 0;
            break;
        case 1:
            x = width - 1;
            break;
        case 2:
            y = height - 1;
            break;
        case 3:
            x = 0;
            break;
        }
}
std::vector<int> GeneratePositions(int width, int height)
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
        FixEdge(positions[0], positions[1], width, height, first);
        FixEdge(positions[2], positions[3], width, height, second);
    }

    return positions;
}

using OptResult = std::pair<double, Line>;
OptResult MinEnergy(const unsigned char* grayImage, const int width, const int height, int workerId)
{
    double minEnergy = INFINITY;
    Line minLine;
    unsigned char avgBlack;
    for (auto i = 0; i < try_number; i++) {
        srand(workerId * try_number + i);
        auto positions = GeneratePositions(width, height);
        Line l(positions);
        RasterizedLines lines = l.Rasterize();
        auto [egy, color] = Energy(lines, grayImage, width, height);
        l.r = l.g = l.b = color;
        if (egy < minEnergy) {
            minEnergy = egy;
            minLine = l;
        }
    }

    return std::make_pair(minEnergy, minLine);
}

int main()
{
    fs::path p = fs::current_path();
    std::cout << "Current path is " << p << std::endl;
    std::string pathStr = p.string();
    auto pos = pathStr.find("primitive");
    auto project_path = fs::path(pathStr.substr(0, pos + std::string("primitive").length()));
    auto imagePath = project_path.append("images").append("lenna.png");
    int width, height, nChannels;
    // stbi_set_flip_vertically_on_load(1);
    unsigned char* imageData = stbi_load(imagePath.string().c_str(), &width, &height, &nChannels, 0);

    printf("width: %d, height: %d, nrComponents: %d\n", width, height, nChannels);
    auto grayImage = rgb2grayMerged(imageData, width, height);

    ThreadPool pool(6);
    // enqueue and store future
    {
        Timer _;
        std::vector<std::future<OptResult>> results;
        // std::vector<OptResult> results;
        for (auto i = 0; i < 6; i++) {
            auto result = pool.enqueue(MinEnergy, grayImage, width, height, i);
            // auto result = MinEnergy(grayImage, width, height, i);
            results.push_back(std::move(result));
        }
        for (auto& result : results) {
            auto r = result.get();
            // auto r = result;
            std::cout << "energy: " << r.first << " " << r.second << std::endl;
        }
    }
    // {
    //     std::cout << "Not using ThreadPool" << std::endl;
    //     Timer _;
    //     for (auto i = 0; i < 6 * try_number; i++) {
    //         srand(i);

    //         auto positions = GeneratePositions(width, height);

    //         Line l(positions);
    //         RasterizedLines lines = l.Rasterize();
    //         lines.r = rand() % 256;
    //         lines.g = rand() % 256;
    //         lines.b = rand() % 256;
    //         auto result = pool.enqueue(Energy, &lines, grayImage, width, height);
    //         auto energy = result.get();
    //     }
    // }

    free(imageData);
    free(grayImage);
}
