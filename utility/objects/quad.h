#ifndef QUAD_H
#define QUAD_H

#include "../common.h"
#include "../object.h"

#include <cmath>

class Quad : public Object
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
        Quad(const point3& _origin, const vec3& _u, const vec3& _v, shared_ptr<Material> _material)
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
            bbox = aabb(origin, origin + u_vec + v_vec).pad();
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
            double alpha = dot(w, cross(int_vec, v_vec));
            double beta = dot(w, cross(u_vec, int_vec));

            if (!is_interior(alpha, beta, rec))
                return false;

            rec.t = t;
            rec.p = int_p;
            rec.set_face_normal(ray, n);
            rec.material = material;

            return true;
        }

        virtual bool is_interior(double a, double b, hit_record& rec) const
        {
            if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
                return false;
            rec.u = a;
            rec.v = b;
            return true;
        }
};

#endif