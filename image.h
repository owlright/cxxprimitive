#ifndef image_h
#define image_h
namespace primitive {
void rgb2gray(unsigned char* data, int width, int height);
unsigned char* rgb2grayMerged(unsigned char* data, int width, int height);
}

#endif