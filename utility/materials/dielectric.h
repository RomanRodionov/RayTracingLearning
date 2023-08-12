#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "../material.h"
#include "../color.h"

class Dielectric : public Material
{
    public:
        double ir; //Refraction Index
        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}
        virtual bool scatter(
            const Ray& fall_ray, 
            const hit_record& hit, 
            color& attenuation,
            Ray& scattered
        ) const override
        {
            attenuation = WHITE;
            double refraction_ratio = hit.front_face ? (1.0/ir) : ir;
            vec3 unit_dir = unit_vector(fall_ray.direction());
            double cos_theta = dot(-unit_dir, hit.normal);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
            vec3 dir_out;
            bool can_refract = refraction_ratio * sin_theta <= 1.0;
            if (!can_refract || reflectance(cos_theta, refraction_ratio) > rand_double())
            {
                dir_out = reflect(unit_dir, hit.normal);
            }
            else
            {
                dir_out = refract(unit_dir, hit.normal, refraction_ratio);
            }
            scattered = Ray(hit.p, dir_out, fall_ray.time());
            return true;
        }
    private:
        static double reflectance(double cosine, double ref_idx)
        {
            //Schlick's approximation
            double r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};

#endif