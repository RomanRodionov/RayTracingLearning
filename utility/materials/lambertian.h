#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "../material.h"

class Lambertian : public Material
{
    public:
        color albedo;
        Lambertian(const color& a) : albedo(a) {}
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
            scattered = Ray(hit.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

#endif