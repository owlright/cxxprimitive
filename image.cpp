#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
namespace primitive {

Color Image::RGBAAt(int x, int y) const
{
    int index = (y * width + x) * nChannels;
    if (nChannels == 4) {
        return Color(data[index], data[index + 1], data[index + 2], data[index + 3]);
    }
    return Color(data[index], data[index + 1], data[index + 2], 255);
}

Image::Image(int width, int height)
    : width(width)
    , height(height)
{
    this->nChannels = 3;
    data = (unsigned char*)malloc(width * height * this->nChannels);
}

Image::Image(Color c, int width, int height)
    : width(width)
    , height(height)
{
    this->nChannels = 3;
    data = (unsigned char*)malloc(width * height * this->nChannels);
    for (int i = 0; i < width * height; i++) {
        data[i * 3] = c.r;
        data[i * 3 + 1] = c.g;
        data[i * 3 + 2] = c.b;
    }
}

Image::Image(const std::string& path)
{
    int width, height, nChannels;
    // // stbi_set_flip_vertically_on_load(1);
    data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
    printf("width: %d, height: %d, nrComponents: %d\n", width, height, nChannels);
    this->data = data;
    this->width = width;
    this->height = height;
    this->nChannels = nChannels;
}

Image::Image(unsigned char* data, int width, int height, int nChannels)
    : data(data)
    , width(width)
    , height(height)
    , nChannels(nChannels)
{
}

// Copy constructor
Image::Image(const Image& other)
    : width(other.width)
    , height(other.height)
    , nChannels(other.nChannels)
{
    data = (unsigned char*)malloc(width * height * nChannels);
    memcpy(data, other.data, width * height * nChannels);
}

// Copy assignment operator
Image& Image::operator=(const Image& other)
{
    if (this == &other)
        return *this;

    free(data);

    width = other.width;
    height = other.height;
    nChannels = other.nChannels;
    data = (unsigned char*)malloc(width * height * nChannels);
    memcpy(data, other.data, width * height * nChannels);

    return *this;
}

// Move constructor
Image::Image(Image&& other) noexcept
    : data(other.data)
    , width(other.width)
    , height(other.height)
    , nChannels(other.nChannels)
{
    other.data = nullptr;
    other.width = -1;
    other.height = -1;
    other.nChannels = -1;
}

// Move assignment operator
Image& Image::operator=(Image&& other) noexcept
{
    if (this == &other)
        return *this;

    free(data);

    data = other.data;
    width = other.width;
    height = other.height;
    nChannels = other.nChannels;

    other.data = nullptr;
    other.width = 0;
    other.height = 0;
    other.nChannels = 0;

    return *this;
}

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