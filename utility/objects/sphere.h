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
        aabb bbox;
        point3 get_center(double time) const
        {
            return origin + move * time;
        }
    public:
        Sphere() {};
        Sphere(point3 _center, double _radius, shared_ptr<Material> _material) 
            : origin(_center), radius(_radius), material(_material), is_moving(false) 
        {
            auto rvec = vec3(_radius, radius, radius);
            bbox = aabb(_center - rvec, _center + rvec);
        }
        Sphere(point3 _center1, point3 _center2, double _radius, shared_ptr<Material> _material) 
            : origin(_center1), radius(_radius), material(_material), is_moving(true) 
        {
            auto rvec = vec3(_radius, radius, radius);
            aabb bbox1 = aabb(_center1 - rvec, _center1 + rvec);
            aabb bbox2 = aabb(_center2 - rvec, _center2 + rvec);
            bbox = aabb(bbox1, bbox2);

            move = _center2 - _center1;
        }
        static void get_sphere_uv(const point3& p, double& u, double& v)
        {
            double theta = acos(-p.y());
            double phi = atan2(-p.z(), p.x()) + PI;

            u = phi / (2 * PI);
            v = theta / PI;
        }
        aabb bounding_box() const override { return bbox;}
        virtual bool hit(const Ray& ray, Interval ray_t, hit_record& rec) const override;
};

bool Sphere::hit(const Ray& ray, Interval ray_t, hit_record& rec) const
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
    if (root > ray_t.max) {return false;}
    if (root <= ray_t.min)
    {
        root = (-hb + sqrtd) / a;
        if (root <= ray_t.min || root > ray_t.max) {return false;}
    }

    rec.t = root;
    rec.p = ray.at(root);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(ray, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.material = material;

    return true;
}

#endif