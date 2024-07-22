#ifndef image_h
#define image_h
namespace primitive {
struct Color {
    unsigned char r, g, b;
    Color()
        : r(0)
        , g(0)
        , b(0)
    {
    }
    Color(unsigned char r, unsigned char g, unsigned char b)
        : r(r)
        , g(g)
        , b(b)
    {
    }
};
void rgb2gray(unsigned char* data, int width, int height);
unsigned char* rgb2grayMerged(unsigned char* data, int width, int height);
}

#endif