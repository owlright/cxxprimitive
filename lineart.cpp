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
#include "drawingboard.h"
#include <thread>
#include <ctime>

namespace fs = std::filesystem;
using namespace primitive;
const int try_number = 20000;

std::pair<int, int> PickTwoUniqueNumbers()
{
    int first = rand() % 4; // 生成1到4之间的随机数作为第一个数字
    int second;
    do {
        second = rand() % 4; // 生成第二个数字，直到它与第一个数字不同
    } while (second == first);
    return std::make_pair(first, second); // 返回一对不重复的数字
}

std::pair<double, Color> Energy(const RasterizedLines& lines, const DrawingBoard& board, int width, int height)
{
    auto target = board.getTargetImage();
    auto alreadyTarget = board.getBoard();
    double energy = 0;
    // 计算这条线经过的像素颜色平均值
    double rAvg = 0.0;
    double gAvg = 0.0;
    double bAvg = 0.0;
    int pixelCount = 0;
    for (int i = 0; i < lines.h; i++) {
        auto line = lines.lines[i];
        for (int x = line.left; x <= line.right; x++) {
            rAvg += target[line.y * width * 3 + x] - alreadyTarget[line.y * width * 3 + x];
            gAvg += target[line.y * width * 3 + x + 1] - alreadyTarget[line.y * width * 3 + x + 1];
            bAvg += target[line.y * width * 3 + x + 2] - alreadyTarget[line.y * width * 3 + x + 2];
            if (rAvg > 0 || gAvg > 0 || bAvg > 0) {
                pixelCount++;
            }
        }
    }
    if (pixelCount == 0) { // 如果这条线没有覆盖任何像素，返回无穷大的能量
        return std::make_pair(INFINITY, Color(255, 255, 255));
    }
    rAvg /= pixelCount;
    gAvg /= pixelCount;
    bAvg /= pixelCount;
    // 取平均color，计算这条线和目标图像之间差值的平方和
    for (int i = 0; i < lines.h; i++) {
        auto line = lines.lines[i];
        for (int x = line.left; x <= line.right; x++) {
            energy += pow(target[line.y * width * 3 + x] - rAvg - alreadyTarget[line.y * width * 3 + x], 2);
            energy += pow(target[line.y * width * 3 + x + 1] - gAvg - alreadyTarget[line.y * width * 3 + x + 1], 2);
            energy += pow(target[line.y * width * 3 + x + 2] - bAvg - alreadyTarget[line.y * width * 3 + x + 2], 2);
        }
    }
    return std::make_pair(sqrt(energy), Color(rAvg, gAvg, bAvg));
}

void FixEdge(int& x, int& y, const int width, const int height, int edgeId)
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

OptResult MinEnergy(const DrawingBoard& board, int workerId)
{
    const int width = board.getWidth();
    const int height = board.getHeight();
    double minEnergy = INFINITY;
    Line minLine;
    unsigned char avgBlack;
    for (auto i = 0; i < try_number; i++) {
        srand(time(0));
        auto positions = GeneratePositions(width, height);
        Line l(positions);
        RasterizedLines lines = l.Rasterize();
        auto [egy, color] = Energy(lines, board, width, height);
        l.color = color;
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
    std::shared_ptr<unsigned char> imageData;
    imageData.reset(stbi_load(imagePath.string().c_str(), &width, &height, &nChannels, 0));

    printf("width: %d, height: %d, nrComponents: %d\n", width, height, nChannels);

    const auto processor_count = std::thread::hardware_concurrency();
    printf("This machine has %d processors\n", processor_count);
    ThreadPool pool(6);
    DrawingBoard board(width, height, imageData.get());
    {
        Timer _;
        for (auto lc = 0; lc < 20; lc++) {
            std::vector<std::future<OptResult>> results;
            for (auto i = 0; i < processor_count; i++) {
                auto result = pool.enqueue(MinEnergy, std::ref(board), i);
                results.push_back(std::move(result));
            }
            double minEnergy = INFINITY;
            Line bestLine;
            for (auto& result : results) {
                auto r = result.get();
                if (r.first < minEnergy) {
                    minEnergy = r.first;
                    bestLine = r.second;
                }
            }
            board.DrawLine(bestLine);
            std::cout << "Round " << lc << " Energy: " << minEnergy << " " << bestLine << std::endl;
        }
    }
    board.SaveImage("output.png");
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

    //    free(imageData);
}
