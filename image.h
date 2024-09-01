#ifndef image_h
#define image_h
#include <ostream>
namespace primitive {
struct Color {
    unsigned char r, g, b, a;
    Color()
        : r(0)
        , g(0)
        , b(0)
        , a(255)
    {
    }
    Color(unsigned char r, unsigned char g, unsigned char b)
        : r(r)
        , g(g)
        , b(b)
        , a(255)
    {
    }
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {
    }
    friend std::ostream& operator<<(std::ostream& os, const Color& color)
    {
        os << "Color(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ", " << (int)color.a << ")";
        return os;
    };
};
void rgb2gray(unsigned char* data, int width, int height);
unsigned char* rgb2grayMerged(unsigned char* data, int width, int height);

}

#endif