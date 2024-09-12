#include <vector>
#include "line.h"
#include "Worker.h"
#include "state.h"
#include "stb_image_write.h"
using namespace std;
namespace primitive {

using State = primitive::State;

enum class ShapeType {
    Line,
    Triangle,
    Rectangle,
    Circle,
    Ellipse,
    Polyline,
    Polygon,
    Bezier,
    Arc,
    ShapeTypeCount
};

struct DrawingBoard {
    double score;
    int width { -1 };
    int height { -1 };
    Image target;
    Image current;
    Color background;
    vector<Line> lines;
    vector<Worker> workers;
    int step(ShapeType shapeType, int alpha, int repeat);
    void add(IShape* shape, int alpha);
    explicit DrawingBoard(const Image& target, Color background, int numWorkers);
    void SaveImage(const char* filename)
    {
        stbi_write_png(filename, width, height, 3, current.data, width * 3);
    }

    State runWorkers(ShapeType shapeType, int a, int n, int age, int m);
    void DrawLine(const Line& line);
};

}