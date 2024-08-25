#include <vector>
#include "line.h"
#include "stb_image_write.h"
using namespace std;
namespace primitive {

class DrawingBoard {
private:
    int width { -1 };
    int height { -1 };
    unsigned char* targetImage { nullptr };
    unsigned char* board { nullptr };

    vector<Line> lines;

public:
    int getWidth() const
    {
        return width;
    }
    int getHeight() const
    {
        return height;
    }
    const unsigned char* getTargetImage() const
    {
        return targetImage;
    }
    const unsigned char* getBoard() const
    {
        return board;
    }

public:
    explicit DrawingBoard(int width, int height, const unsigned char* target);
    explicit DrawingBoard(int width, int height);
    void SetTargetImage(const unsigned char* target);
    void SaveImage(const char* filename)
    {
        stbi_write_png(filename, width, height, 3, board, width * 3);
    }
    ~DrawingBoard()
    {
        free(board);
        free(targetImage);
    }

public:
    void DrawLine(const Line& line);
};

}