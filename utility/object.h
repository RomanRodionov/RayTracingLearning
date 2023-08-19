#ifndef OBJECT_H
#define OBJECT_H

#include "common.h"
#include "ray.h"
#include "aabb.h"

#include <iostream>

class Material;

struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    double u, v;
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
        virtual bool hit(const Ray& ray, Interval ray_t, hit_record& rec) const = 0;
        virtual aabb bounding_box() const = 0;
};

class Translate : public Object
{
    private:
        shared_ptr<Object> object;
        vec3 offset;
        aabb bbox;
    public:
        Translate(shared_ptr<Object> _object, const vec3& displacement)
        : object(_object), offset(displacement)
        {
            bbox = object->bounding_box() + offset;
        }

        bool hit(const Ray& ray, Interval ray_t, hit_record& rec) const override
        {
            Ray offset_r(ray.origin() - offset, ray.direction(), ray.time());
            if (!object->hit(offset_r, ray_t, rec))
                return false;
            rec.p += offset;
            return true;
        }
        aabb bounding_box() const override {return bbox;}
};

class RotateY : public Object
{
    private:
        shared_ptr<Object> object;
        double sin_theta;
        double cos_theta;
        aabb bbox;
    public:
        RotateY(shared_ptr<Object> _object, double angle) : object(_object)
        {
            double radians = degrees_to_radians(angle);
            sin_theta = sin(radians);
            cos_theta = cos(radians);
            bbox = object->bounding_box();

            point3 min( INF,  INF,  INF);
            point3 max(-INF, -INF, -INF);

            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    for (int k = 0; k < 2; ++k)
                    {
                        double x = i * bbox.x.max + (1 - i) * bbox.x.min;
                        double y = j * bbox.y.max + (1 - j) * bbox.y.min;
                        double z = k * bbox.z.max + (1 - k) * bbox.z.min;

                        double new_x =  cos_theta * x + sin_theta * z;
                        double new_z = -sin_theta * x + cos_theta * z;

                        vec3 tester(new_x, y, new_z);

                        for (int c = 0; c < 3; ++c)
                        {
                            min[c] = fmin(min[c], tester[c]);
                            max[c] = fmax(max[c], tester[c]);
                        }
                    }
            bbox = aabb(min, max);
        }

        bool hit(const Ray& ray, Interval ray_t, hit_record& rec) const override
        {
            point3 origin = ray.origin();
            vec3 direction = ray.direction();

            origin[0] = cos_theta * ray.origin()[0] - sin_theta * ray.origin()[2];
            origin[2] = sin_theta * ray.origin()[0] + cos_theta * ray.origin()[2];

            direction[0] = cos_theta * ray.direction()[0] - sin_theta * ray.direction()[2];
            direction[2] = sin_theta * ray.direction()[0] + cos_theta * ray.direction()[2];
            
            Ray rotated_r(origin, direction, ray.time());

            if (!object->hit(rotated_r, ray_t, rec))
                return false;

            point3 p = rec.p;
            p[0] =  cos_theta * rec.p[0] + sin_theta * rec.p[2];
            p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

            point3 normal = rec.normal;
            normal[0] =  cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
            normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
            
            rec.p = p;
            rec.normal = normal;
            
            return true;
        }

        aabb bounding_box() const override {return bbox;}
};

#endif