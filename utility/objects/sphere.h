#ifndef SPHERE_H
#define SPHERE_H

#include "../object.h"
#include "../vec3.h"

class Sphere : public Object
{
    public:
        point3 center;
        double radius;
        Sphere() {};
        Sphere(point3 c, double r) : center(c), radius(r) {};
        virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
};

bool Sphere::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
    vec3 oc = ray.origin() - center;
    double a = ray.direction().square();
    double hb = dot(oc, ray.direction());
    double c = oc.square() - radius * radius;
    double discr = hb * hb - a * c;
    if (discr < 0) {return false;}
    double sqrtd = sqrt(discr);
    double root = (-hb - sqrtd) / a;
    if (root > t_max) {return false;}
    if (root < t_min)
    {
        root = (-hb + sqrtd) / a;
        if (root < t_min || root > t_max) {return false;}
    }

    rec.t = root;
    rec.p = ray.at(root);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(ray, outward_normal);

    return true;
}

#endif