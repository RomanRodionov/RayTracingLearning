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
        std::vector<uchar> data;
    public:
        Image(int h, int w) : height(h), width(w)
        {
            data = std::vector<uchar>(width * height * CHANNELS);
        }
        Image(std::string filename)
        {
            load_png(filename);
        }
        int get_width() const {return width;}
        int get_height() const {return height;}
        int load_png(std::string filename)
        {
            unsigned char *img = stbi_load(filename.c_str(), &width, &height, nullptr, CHANNELS);
            if (img == NULL) {return 1;}
            data = std::vector<uchar>(img, img + width * height * CHANNELS);
            return 0;
        }
        int save_to_png(std::string filename) const
        {
            return stbi_write_png(filename.c_str(), width, height, CHANNELS, data.data(), width * CHANNELS);
        }
        void draw_pixel(int row, int col, color color, int samples_per_pixel)
        {
            assert(row >= 0 && row < height);
            assert(col >= 0 && col < width);
            color /= samples_per_pixel;
            double r = color[0];
            double g = color[1];
            double b = color[2];
            data[(width * row + col) * CHANNELS]     = static_cast<uchar> (MAX_COLOR * clamp(r, 0.0, 1.0));
            data[(width * row + col) * CHANNELS + 1] = static_cast<uchar> (MAX_COLOR * clamp(g, 0.0, 1.0));
            data[(width * row + col) * CHANNELS + 2] = static_cast<uchar> (MAX_COLOR * clamp(b, 0.0, 1.0));
        }
        inline color get_color(int row, int col) const
        {
            assert(row >= 0 && row < height);
            assert(col >= 0 && col < width);
            return color((double) data[(width * row + col) * CHANNELS] / MAX_COLOR,
                         (double) data[(width * row + col) * CHANNELS + 1] / MAX_COLOR,
                         (double) data[(width * row + col) * CHANNELS + 2] / MAX_COLOR);
        }
};

#endif