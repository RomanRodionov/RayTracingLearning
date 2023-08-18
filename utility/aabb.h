#ifndef AABB_H
#define AABB_H

#include "common.h"
#include "ray.h"

#define DELTA 0.0001

class aabb
{
    public:
        Interval x, y, z;

        aabb() {}
        aabb(const Interval& ix, const Interval& iy, const Interval& iz) :
            x(ix), y(iy), z(iz) {}
        aabb(const point3& a, const point3& b)
        {
            x = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
            y = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
            z = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
        }
        aabb(const aabb& bbox1, const aabb& bbox2)
        {
            x = Interval(bbox1.x, bbox2.x);
            y = Interval(bbox1.y, bbox2.y);
            z = Interval(bbox1.z, bbox2.z);
        }

        aabb pad()
        {
            double delta = DELTA;
            Interval new_x = (x.size() >= delta) ? x : x.expand(delta);
            Interval new_y = (y.size() >= delta) ? y : y.expand(delta);
            Interval new_z = (z.size() >= delta) ? z : z.expand(delta); 

            return aabb(new_x, new_y, new_z);
        }
        const Interval& axis(int n) const
        {
            switch (n)
            {
            case 1:
                return y;
            case 2:
                return z;
            default://case 0:
                return x;
            }
        }
        bool hit(const Ray& ray, Interval ray_t) const
        {
            for (int a = 0; a < 3; ++a)
            {
                auto invD = 1 / ray.direction()[a];
                auto orig = ray.origin()[a];

                auto t0 = (axis(a).min - orig) * invD;
                auto t1 = (axis(a).max - orig) * invD;

                if (invD < 0)
                {
                    std::swap(t0, t1);
                }

                if (t0 > ray_t.min) ray_t.min = t0;
                if (t1 < ray_t.max) ray_t.max = t1;
                if (ray_t.max <= ray_t.min)
                {
                    return false;
                }
            }
            return true;
        }
};

#endif