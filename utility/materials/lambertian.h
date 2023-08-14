#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "../material.h"

class Lambertian : public Material
{
    public:
        shared_ptr<Texture> albedo;
        Lambertian(const shared_ptr<Texture>& _texture) : albedo(_texture) {}
        Lambertian(const color& _color) : albedo(make_shared<SolidColor>(_color)) {}
        virtual bool scatter(
            const Ray& fall_ray, 
            const hit_record& hit, 
            color& attenuation,
            Ray& scattered
        ) const override
        {
            vec3 scatter_direction = hit.normal + random_unit_vector();
            if (scatter_direction.near_zero())
            {
                scatter_direction = hit.normal;
            }
            scattered = Ray(hit.p, scatter_direction, fall_ray.time());
            attenuation = albedo->value(hit.u, hit.v, hit.p);
            return true;
        }
};

#endif