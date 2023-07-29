#include <iostream>
#include <vector>
#include <array>
#include <string>

#include "source/vec3.h"
#include "source/color.h"
#include "source/image.h"

#define WIDTH 256
#define HEIGHT 256

int main(int, char**){
    std::string filename = "output.png";
    Image image(WIDTH, HEIGHT);
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            color color;
            color[0] = static_cast<float> (i) / (HEIGHT - 1);
            color[1] = static_cast<float> (j) / (WIDTH - 1);
            color[2] = 1.f - static_cast<float> (color[0] + color[1]) / 2;
            image.draw_pixel(i, j, color);
        }
    }
    image.save_to_png(filename);
    std::cout << "File " << filename << " has been saved" << std::endl;;
}
