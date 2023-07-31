#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "object.h"
#include "vec3.h"

class Material
{
    public:
        virtual bool scatter(
            const Ray& fall_ray, 
            const hit_record& hit, 
            color& attenuation,
            Ray& scattered
        ) const = 0;
};

#endif