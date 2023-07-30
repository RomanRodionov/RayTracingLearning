#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"

struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
};

class Object
{
    public:
        virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif