#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "vec3.h"

#include <iostream>

class Material;

struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    shared_ptr<Material> material;
    inline void set_face_normal(const Ray& ray, const vec3& outward_normal)
    {
        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Object
{
    public:
        virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif