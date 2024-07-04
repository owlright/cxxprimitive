#include <math.h>
#include "image.h"
#include "stb_image.h"
double CalculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int main()
{
    const char* imagePath = "../images/lenna.png";
    int width, height, nChannels;
    // stbi_set_flip_vertically_on_load(1);
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &nChannels, 0);
    rgb2gray(imageData, width, height);
    printf("width: %d, height: %d, nrComponents: %d\n", width, height, nChannels);
    int circle_center_x = width / 2;
    int circle_center_y = height / 2;
    double radius = CalculateDistance(circle_center_x, circle_center_x, 0, width);
    printf("radius: %f\n", radius);
    free(imageData);
}
