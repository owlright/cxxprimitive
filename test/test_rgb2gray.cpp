#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"
int main()
{
    const char* imagePath = "../images/lenna.png";
    int width, height, nChannels;
    // stbi_set_flip_vertically_on_load(1);
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &nChannels, 0);
    rgb2gray(imageData, width, height);
    printf("width: %d, height: %d, nrComponents: %d\n", width, height, nChannels);
    stbi_write_png("gray_lenna.png", width, height, nChannels, imageData, width * nChannels);
    free(imageData);
}
