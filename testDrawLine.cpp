#include "drawingboard.h"
#include <iostream>
using namespace std;
using namespace primitive;
int main()
{
    DrawingBoard board(256, 256);
    Line line(10, 10, 100, 200);
    board.DrawLine(line);
    line.SetStart(line.getX2(), line.getY2());
    line.SetEnd(200, 100);
    board.DrawLine(line);
    board.SaveImage("testDrawLine.png");
    return 0;
}