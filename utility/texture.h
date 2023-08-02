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
        Texture() {}
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
        color get_color(double u_orig, double v_orig) const
        {
            double temp;
            double u = modf(u_orig, &temp) * width;
            double v = modf(v_orig, &temp) * height;
            double u_alpha = modf(u, &temp);
            double v_alpha = modf(v, &temp);
            int ul = (int) u;
            int ur = (ul + 1) % width;
            int vu = (int) v;
            int vl = (vu + 1) % height;
            return data[(vu * width) + ul] * (1 - u_alpha) * (1 - v_alpha) +
                   data[(vu * width) + ur] * u_alpha * (1 - v_alpha) +
                   data[(vl * width) + ul] * (1 - u_alpha) * v_alpha +
                   data[(vl * width) + ur] * u_alpha * v_alpha;
        }
};

#endif