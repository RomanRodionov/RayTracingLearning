#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include "image.h"

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

static const CheckerTexture default_texture = CheckerTexture(0.2, PURPLE_PIZZA, BLACK);

class ImageTexture : public Texture
{
    private:
        Image img;
        bool cycle;
    public:
        ImageTexture() : img(Image()), cycle(true) {}
        ImageTexture(const std::string filename, bool _cycle = true) : img(filename), cycle(_cycle) {}
        ImageTexture(const Image& _img, bool _cycle = true) : img(_img), cycle(_cycle) {}
        ImageTexture(shared_ptr<Image> _img, bool _cycle = true) : img(*_img), cycle(_cycle) {}

        color value(double u, double v, const point3& p) const override
        {
            if (img.width() <= 0)
            {
                return default_texture.value(u, v, p);
            }
            if (cycle)
            {
                double temp;
                u = modf(u, &temp);
                v = 1.0 - modf(v, &temp);
            }
            else
            {
                u = Interval(0, 1).clamp(u);
                v = 1.0 - Interval(0, 1).clamp(v);
            }

            int col = static_cast<int>(u * img.width());
            int row = static_cast<int>(v * img.height());
            uchar* pixel = img.pixel_data(row, col);

            double color_scale = 1.0 / 255.0;
            return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        }
        
};

#endif