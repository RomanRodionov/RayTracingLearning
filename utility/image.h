#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image/stb_image_write.h"

#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <filesystem>

#include "common.h"

inline double linear_to_gamma(double lin_comp)
{
    return sqrt(lin_comp);
}

namespace img
{
    enum state
    {
        STBI,
        CREATED,
        NONE
    };
}

class Image
{
    private:
        int w, h;
        int ch;
        int row_len;
        img::state state;
        uchar* data;
    public:
        Image() : w(0), h(0), ch(0), state(img::NONE), data(nullptr) {}
        Image(int _h, int _w, int _ch = CHANNELS) : row_len(w * _ch)
        {
            state = img::NONE;
            create(_h, _w, _ch);
        }
        Image(const std::string filename)
        {
            state = img::NONE;
            load_png(filename);
        }
        ~Image()
        {
            clear();
        }
        int width() const {return w;}
        int height() const {return h;}
        int channels() const {return ch;}
        bool load_png(std::string filename, int _ch = CHANNELS)
        {
            ch = _ch;
            clear();
            if (std::filesystem::exists(filename))
                data = stbi_load(filename.c_str(), &w, &h, nullptr, ch);
            if (data == nullptr)
            {
                w = h = row_len = 0;
                state = img::NONE;
                std::cerr << "Failed to read " << filename << std::endl;
                return false;
            }
            state = img::STBI;
            row_len = w * ch;
            return true;
        }
        void create(int _h, int _w, int _ch = CHANNELS)
        {
            clear();
            h = _h;
            w = _w;
            ch = _ch;
            data = new uchar[h * w * ch];
            state = img::CREATED;
        }
        void clear()
        {
            if (state == img::STBI) 
                STBI_FREE(data);
            else if (state == img::CREATED)
                delete data;
            state = img::NONE;
        }
        int save_to_png(std::string filename) const
        {
            if (data != nullptr)
            {
                return stbi_write_png(filename.c_str(), w, h, ch, data, row_len);
            }
            std::cerr << "Nothing to save" << std::endl;
            return 1;
        }
        void draw_pixel(int row, int col, color color)
        {
            if (data != nullptr)
            {
                assert(row >= 0 && row < h && col >= 0 && col < w);
                double r = linear_to_gamma(color[0]);
                double g = linear_to_gamma(color[1]);
                double b = linear_to_gamma(color[2]);
                static const Interval intensity(0.000, 0.999);
                data[(w * row + col) * ch]     = static_cast<uchar> (MAX_COLOR * intensity.clamp(r));
                data[(w * row + col) * ch + 1] = static_cast<uchar> (MAX_COLOR * intensity.clamp(g));
                data[(w * row + col) * ch + 2] = static_cast<uchar> (MAX_COLOR * intensity.clamp(b));
            }
        }
        uchar* pixel_data(int row, int col) const
        {
            assert(row >= 0 && row < h && col >= 0 && col < w);
            static uchar default_color[CHANNELS] = ARRAY_FROM_COLOR(PURPLE_PIZZA);
            if (data == NULL)
            {
                return default_color;
            }
            return &data[(w * row + col) * CHANNELS];
        }
        inline color get_color(int row, int col) const
        {
            assert(row >= 0 && row < h && col >= 0 && col < w);
            static uchar default_color[CHANNELS] = ARRAY_FROM_COLOR(PURPLE_PIZZA);
            if (data == NULL)
            {
                double color_scale = 1.0 / 255.0;
                return color(color_scale * default_color[0],
                             color_scale * default_color[1],
                             color_scale * default_color[2]);
            }
            return color((double) data[(w * row + col) * ch] / MAX_COLOR,
                         (double) data[(w * row + col) * ch + 1] / MAX_COLOR,
                         (double) data[(w * row + col) * ch + 2] / MAX_COLOR);
        }
};

#endif