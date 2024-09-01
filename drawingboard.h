#include <vector>
#include "line.h"
#include "stb_image_write.h"
using namespace std;
namespace primitive {

struct DrawingBoard {
    double score;
    int width { -1 };
    int height { -1 };
    Image target;
    Image current;
    Color background;
    vector<Line> lines;
    explicit DrawingBoard(const Image& target ,Color background , int numWorkers);
    void SaveImage(const char* filename)
    {
        stbi_write_png(filename, width, height, 3, current.data, width * 3);
    }

public:
    void DrawLine(const Line& line);
};

}