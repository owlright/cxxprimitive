#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
namespace primitive {
void rgb2gray(unsigned char* data, int width, int height)
{
    for (int i = 0; i < width * height; i++) {
        unsigned char r = data[i * 3];
        unsigned char g = data[i * 3 + 1];
        unsigned char b = data[i * 3 + 2];
        unsigned char gray = 0.299 * r + 0.587 * g + 0.114 * b;
        data[i * 3] = gray;
        data[i * 3 + 1] = gray;
        data[i * 3 + 2] = gray;
    }
}
unsigned char* rgb2grayMerged(unsigned char* data, int width, int height)
{
    assert(data != nullptr && width > 0 && height > 0);
    unsigned char* gray_data = (unsigned char*)malloc(width * height);
    for (int i = 0; i < width * height; i++) {
        unsigned char r = data[i * 3];
        unsigned char g = data[i * 3 + 1];
        unsigned char b = data[i * 3 + 2];
        unsigned char gray = 0.299 * r + 0.587 * g + 0.114 * b;
        gray_data[i] = gray;
    }
    return gray_data;
}

}