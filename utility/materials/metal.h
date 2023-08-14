#ifndef METAL_H
#define METAL_H

#include "../material.h"

class Metal : public Material
{
    public:
        shared_ptr<Texture> albedo;
        double fuzz;
        Metal(const shared_ptr<Texture>& _texture, const double& f) : albedo(_texture), fuzz(f < 1.0 ? f : 1.0) {}
        Metal(const color& _color, const double& f) : albedo(make_shared<SolidColor>(_color)), fuzz(f < 1.0 ? f : 1.0) {}
        virtual bool scatter(
            const Ray& fall_ray, 
            const hit_record& hit, 
            color& attenuation,
            Ray& scattered
        ) const override
        {
            vec3 reflected = reflect(unit_vector(fall_ray.direction()), hit.normal);
            scattered = Ray(hit.p, reflected + random_in_unit_sphere() * fuzz, fall_ray.time());
            attenuation = albedo->value(hit.u, hit.v, hit.p);
            return (dot(scattered.direction(), hit.normal) > 0);
        }
};

#endif