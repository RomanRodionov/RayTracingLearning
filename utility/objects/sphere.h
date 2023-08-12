#ifndef SPHERE_H
#define SPHERE_H

#include "../object.h"
#include "../common.h"
#include "../material.h"

class Sphere : public Object
{
    private:
        point3 origin;
        double radius;
        shared_ptr<Material> material;
        bool is_moving;
        vec3 move;
        point3 get_center(double time) const
        {
            return origin + move * time;
        }
    public:
        Sphere() {};
        Sphere(point3 c, double r, shared_ptr<Material> m) 
            : origin(c), radius(r), material(m), is_moving(false) {}
        Sphere(point3 c1, point3 c2, double r, shared_ptr<Material> m) 
            : origin(c1), radius(r), material(m), is_moving(true) 
        {
            move = c2 - c1;
        }
        virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
};

bool Sphere::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
    point3 center = is_moving ? get_center(ray.time()) : origin;
    vec3 oc = ray.origin() - center;
    double a = ray.direction().square();
    double hb = dot(oc, ray.direction());
    double c = oc.square() - radius * radius;
    double discr = hb * hb - a * c;
    if (discr < 0) {return false;}
    double sqrtd = sqrt(discr);
    double root = (-hb - sqrtd) / a;
    if (root > t_max) {return false;}
    if (root <= t_min)
    {
        root = (-hb + sqrtd) / a;
        if (root <= t_min || root > t_max) {return false;}
    }

    rec.t = root;
    rec.p = ray.at(root);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(ray, outward_normal);
    rec.material = material;

    return true;
}

#endif