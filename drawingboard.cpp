#include "drawingboard.h"

namespace primitive {

DrawingBoard::DrawingBoard(int width, int height, const unsigned char* target)
    : width(width)
    , height(height)
    , targetImage(targetImage)
{
    targetImage = (unsigned char*)malloc(width * height * 3);
    memcpy(targetImage, target, width * height * 3);
    board = (unsigned char*)malloc(width * height * 3);
    memset(board, 255, width * height * 3);
}

DrawingBoard::DrawingBoard(int width, int height)
    : width(width)
    , height(height)
{
    board = (unsigned char*)malloc(width * height * 3);
    memset(board, 255, width * height * 3);
}

void DrawingBoard::SetTargetImage(const unsigned char* target)
{
    targetImage = (unsigned char*)malloc(width * height * 3);
    memcpy(targetImage, target, width * height * 3);
}

void DrawingBoard::DrawLine(const Line& line)
{
    auto lines = line.Rasterize();
    auto color = line.color;
    for (int i = 0; i < lines.h; i++) {
        auto l = lines.lines[i];
        for (int x = l.left; x <= l.right; x++) {
            board[l.y * width * 3 + x] = color.r;
            board[l.y * width * 3 + x + 1] = color.g;
            board[l.y * width * 3 + x + 2] = color.b;
            // 已经画过的线，从目标图像中减去
            // targetImage[l.y * width * 3 + x] -= color.r;
            // targetImage[l.y * width * 3 + x + 1] -= color.g;
            // targetImage[l.y * width * 3 + x + 2] -= color.b;
        }
    }
    this->lines.push_back(line);
}

} // namespace primitive
