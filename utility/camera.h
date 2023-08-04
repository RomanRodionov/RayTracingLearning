#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class Camera
{
    private:
        point3 origin;
        point3 ll_corner;
        vec3 horizontal;
        vec3 vertical;
    public:
        Camera(point3 look_from,
               point3 look_at,
               vec3 vup,
               double fov,
               double aspect_ratio)
        {       
            double focal_length = 1.0;
            double alpha = degrees_to_radians(fov);
            double viewport_width = 2.0 * tan(alpha / 2.0) * focal_length;
            double viewport_height = viewport_width / aspect_ratio;

            vec3 w = unit_vector(look_from - look_at);
            vec3 u = unit_vector(cross(vup, w));
            vec3 v = cross(w, u);

            origin = look_from;
            horizontal = u * viewport_width;
            vertical = v * viewport_height;
            ll_corner = origin - horizontal / 2 - vertical / 2 - w;
        }

        Ray get_ray(double u, double v) const
        {
            return Ray(origin, ll_corner + horizontal * u + vertical * v - origin);
        }
};

#endif