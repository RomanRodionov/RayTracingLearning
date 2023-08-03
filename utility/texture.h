#ifndef TEXTURE_H
#define TEXTURE_H

#include <cmath>
#include <vector>
#include <iostream>

#include "common.h"
#include "image.h"
#include "color.h"

class Texture
{
    public:
        std::vector<color> data;
        int width, height;
        Texture() : width(0), height(0) {}
        Texture(const shared_ptr<Image>& img)
        {
            load(img);
        }
        void load(const shared_ptr<Image>& img)
        {
            width = img->get_width();
            height = img->get_height();
            data.clear();
            for (int row = 0; row < height; ++row)
            {
                for (int col = 0; col < width; ++col)
                {
                    data.push_back(img->get_color(row, col));
                }
            }
        }
        shared_ptr<Texture> crop(const std::pair<double, double>& pos, const std::pair<double, double>& size)
        {
            auto new_tex = make_shared<Texture>();
            int u = pos.first * width, v = pos.second * height, w = size.first * width, h = size.second * height;
            for (int i = v; i < v + h; ++i)
            {
                for (int j = u; j < u + w; ++j)
                {
                    new_tex->data.push_back(data[i * width + j]);
                }
            }
            new_tex->width = w;
            new_tex->height = h;
            return new_tex;
        }
        color get_color(double u_orig, double v_orig) const
        {
            double temp;
            double u = (modf(u_orig, &temp) + 1.0) * width - 0.5;
            double v = (modf(v_orig, &temp) + 1.0) * height - 0.5;
            double u_alpha = modf(u + 1.0, &temp);
            double v_alpha = modf(v + 1.0, &temp);
            int ul = (int) (u + width) % width;
            int ur = (ul + 1) % width;
            int vu = (int) (v + height) % height;
            int vl = (vu + 1) % height;
            return data[(vu * width) + ul] * (1 - u_alpha) * (1 - v_alpha) +
                   data[(vu * width) + ur] * u_alpha * (1 - v_alpha) +
                   data[(vl * width) + ul] * (1 - u_alpha) * v_alpha +
                   data[(vl * width) + ur] * u_alpha * v_alpha;
        }
};

#endif