#ifndef SKYBOX_H
#define SKYBOX

#include "common.h"
#include "texture.h"
#include "vec3.h"

class SkyBox
{
    public:
        shared_ptr<Texture> left;
        shared_ptr<Texture> front;
        shared_ptr<Texture> right;
        shared_ptr<Texture> back;
        shared_ptr<Texture> top;
        shared_ptr<Texture> bottom;
        std::pair<double, double> size;
        int width, height;
        SkyBox() {}
        SkyBox(const shared_ptr<Texture>& tex) 
        {
            load_texture(tex);
        }
        void load_texture(const shared_ptr<Texture>& tex)
        {
            std::pair<double, double> size = {1.0 / 4.0, 1.0 / 3.0};
            left = tex->crop({0, size.second}, size);
            front = tex->crop({size.first, size.second}, size);
            right = tex->crop({size.first * 2, size.second}, size);
            back = tex->crop({size.first * 3, size.second}, size);
            top = tex->crop({size.first, 0}, size);
            bottom = tex->crop({size.first, size.second * 2}, size);
            width = tex->width * size.first;
            height = tex->height * size.second;
        }
        color get_color(const vec3& look_at_original)
        {
            const double u_error = 1.0 - 2.0 / width;
            const double v_error = 1.0 - 2.0 / height;
            const double edge = 1 / sqrt(3);
            vec3 look_at = unit_vector(look_at_original);
            if (fabs(look_at.y()) > edge)
            {
                look_at *= edge / fabs(look_at.y());
            }
            double x = look_at.x();
            double y = look_at.y();
            double z = look_at.z();
            double u, v;
            if (fabs(x) >= edge || fabs(z) >= edge)
            {
                if (fabs(z) >= fabs(x))
                {
                    if (z < 0)
                    {
                        u = 0.5 - x / z / 2 * u_error;
                        v = 0.5 + y / z / 2 * v_error;
                        return front->get_color(u, v);
                    }
                    else
                    {
                        u = 0.5 - x / z / 2 * u_error;
                        v = 0.5 - y / z / 2 * v_error;
                        return back->get_color(u, v);
                    }
                }
                if (x > 0)
                {
                    u = 0.5 + z / x / 2 * u_error;
                    v = 0.5 - y / x / 2 * v_error;
                    return right->get_color(u, v);
                }
                else
                {
                    u = 0.5 + z / x / 2 * u_error;
                    v = 0.5 + y / x / 2 * v_error;
                    return left->get_color(u, v);
                }
            }
            if (y > 0)
            {
                u = 0.5 + x / y / 2 * u_error;
                v = 0.5 - z / y / 2 * v_error;
                return top->get_color(u, v);
            }
            u = 0.5 - x / y / 2 * u_error;
            v = 0.5 - z / y / 2 * v_error;
            return bottom->get_color(u, v);
        }
};

#endif