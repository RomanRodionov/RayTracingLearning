#ifndef PLANE_H
#define PLANE_H

#include "../common.h"
#include "../object.h"

class Plane : public Object
{
    private:
        point3 origin;
        vec3 n;
        double D;
        vec3 u_vec, v_vec;
        vec3 w;
        shared_ptr<Material> material;
        aabb bbox;
    public:
        Plane(const point3& _origin, const vec3& _u, const vec3& _v, shared_ptr<Material> _material)
        : origin(_origin), u_vec(_u), v_vec(_v), material(_material)
        {
            n = cross(u_vec, v_vec);
            w = n / dot(n, n);
            n = unit_vector(n);
            D = dot(n, _origin);

            set_bounding_box();
        }
        virtual void set_bounding_box()
        {
            bbox = aabb(universe, universe, universe);
        }
        aabb bounding_box() const override {return bbox;}
        bool hit(const Ray& ray, Interval ray_t, hit_record& rec) const override
        {
            double dir_proj = dot(n, ray.direction());
            if (abs(dir_proj) < EPS)
                return false;
            double t = (D - dot(n, ray.origin())) / dir_proj;
            if (!ray_t.contains(t))
            {
                return false;
            }

            point3 int_p = ray.at(t);
            vec3 int_vec = int_p - origin;
            rec.u = dot(w, cross(int_vec, v_vec));
            rec.v = dot(w, cross(u_vec, int_vec));

            rec.t = t;
            rec.p = int_p;
            rec.set_face_normal(ray, n);
            rec.material = material;

            return true;
        }
};

#endif