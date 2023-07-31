#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image/stb_image_write.h"

#include <vector>
#include <array>
#include <string>

#include "color.h"
#include "common.h"

class Image
{
    private:
        int width, height;
        std::vector<std::array<uchar, CHANNELS>> data;
    public:
        Image(int h, int w) : height(h), width(w)
        {
            data = std::vector<std::array<uchar, CHANNELS>>(width * height);
        }
        int save_to_png(std::string filename)
        {
            return stbi_write_png(filename.c_str(), width, height, CHANNELS, data.data(), width * CHANNELS);
        }
        void draw_pixel(int row, int col, color color, int samples_per_pixel)
        {
            assert(row >= 0 && row < height);
            assert(col >= 0 && col < width);
            color /= samples_per_pixel;
            //gamma=2
            double r = sqrt(color[0]);
            double g = sqrt(color[1]);
            double b = sqrt(color[2]);
            data[width * row + col] = { static_cast<uchar> (MAX_COLOR * clamp(r, 0.0, 1.0)),
                                        static_cast<uchar> (MAX_COLOR * clamp(g, 0.0, 1.0)),
                                        static_cast<uchar> (MAX_COLOR * clamp(b, 0.0, 1.0))};
        }
};

#endif