#ifndef METAL_H
#define METAL_H

#include "../material.h"

class Metal : public Material
{
    public:
        color albedo;
        double fuzz;
        Metal(const color& a, const double& f) : albedo(a), fuzz(f < 1.0 ? f : 1.0) {}
        virtual bool scatter(
            const Ray& fall_ray, 
            const hit_record& hit, 
            color& attenuation,
            Ray& scattered
        ) const override
        {
            vec3 reflected = reflect(unit_vector(fall_ray.direction()), hit.normal);
            scattered = Ray(hit.p, reflected + random_in_unit_sphere() * fuzz, fall_ray.time());
            attenuation = albedo;
            return (dot(scattered.direction(), hit.normal) > 0);
        }
};

#endif