#include <iostream>
#include <vector>
#include <array>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#include "utility/vec3.h"

int main(int, char**){
    const int WIDTH = 256, HEIGHT = 256, CHANNELS = 3;
    std::string filename = "output.png";
    std::vector<std::array<unsigned char, CHANNELS>> img_data;
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            float r_f = static_cast<float> (i) / (HEIGHT - 1);
            float g_f = static_cast<float> (j) / (WIDTH - 1);
            float b_f = 1.f - static_cast<float> (r_f + g_f) / 2;
            unsigned char r = static_cast<unsigned char> (r_f * 255.f);
            unsigned char g = static_cast<unsigned char> (g_f * 255.f);
            unsigned char b = static_cast<unsigned char> (b_f * 255.f);
            img_data.push_back({r, g, b});
        }
    }
    stbi_write_png(filename.c_str(), WIDTH, HEIGHT, CHANNELS, img_data.data(), WIDTH * CHANNELS);
    std::cout << "File " << filename << " has been written" << std::endl;;
}
