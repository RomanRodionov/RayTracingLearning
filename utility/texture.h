#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

class Texture 
{
    public:
        virtual ~Texture() = default;
        virtual color value(double u, double v, const point3& p) const = 0;
};

class SolidColor : public Texture
{
    private:
        color color_value;
    public:
        SolidColor(color _color) : color_value(_color) {}
        SolidColor(double red, double green, double blue) : SolidColor(color(red, green ,blue)) {}
        
        color value(double u, double v, const point3& p) const override
        {
            return color_value;
        }
};

class GradientTexture : public Texture
{
    private:
        shared_ptr<Texture> first;
        shared_ptr<Texture> second;
    public:
        GradientTexture(shared_ptr<Texture> _first, shared_ptr<Texture> _second) :
            first(_first), second(_second) {}
        
        GradientTexture(color _first, color _second) :
            first(make_shared<SolidColor>(_first)),
            second(make_shared<SolidColor>(_second)) {}

        color value(double u, double v, const point3& p) const override
        {
            return first->value(u, v, p) * (1.0 - v) + second->value(u, v, p) * v;
        }
};

class CheckerTexture : public Texture
{
    private:
        double inv_scale;
        shared_ptr<Texture> even;
        shared_ptr<Texture> odd;
    public:
        CheckerTexture(double _scale, shared_ptr<Texture> _even, shared_ptr<Texture> _odd) :
            inv_scale(1.0 / _scale), even(_even), odd(_odd) {}
        
        CheckerTexture(double _scale, color _color1, color _color2) :
            inv_scale(1.0 / _scale), even(make_shared<SolidColor>(_color1)),
            odd(make_shared<SolidColor>(_color2)) {}

        color value(double u, double v, const point3& p) const override
        {
            int xInt = static_cast<int>(std::floor(inv_scale * p.x()));
            int yInt = static_cast<int>(std::floor(inv_scale * p.y()));
            int zInt = static_cast<int>(std::floor(inv_scale * p.z()));

            bool isEven = (xInt + yInt + zInt) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }
};

#endif