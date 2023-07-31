#ifndef METAL_H
#define METAL_H

#include "../material.h"

class Metal : public Material
{
    public:
        color albedo;
        Metal(const color& a) : albedo(a) {}
        virtual bool scatter(
            const Ray& fall_ray, 
            const hit_record& hit, 
            color& attenuation,
            Ray& scattered
        ) const override
        {
            vec3 reflected = reflect(unit_vector(fall_ray.direction()), hit.normal);
            scattered = Ray(hit.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), hit.normal) > 0);
        }
};

#endif