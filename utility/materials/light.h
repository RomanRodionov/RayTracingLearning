#ifndef LIGHT_H
#define LIGHT_H

#include "../material.h"

class DiffuseLight : public Material
{
    private:
        shared_ptr<Texture> emit;
    public:
        DiffuseLight(shared_ptr<Texture> _emit) : emit(_emit) {}
        DiffuseLight(color _color) : emit(make_shared<SolidColor>(_color)) {}

        bool scatter(
            const Ray& fall_ray, 
            const hit_record& hit, 
            color& attenuation,
            Ray& scattered
        ) const override
        {
            return false;
        }

        color emitted(double u, double v, const point3& p) const override
        {
            return emit->value(u, v, p);
        }
};

#endif