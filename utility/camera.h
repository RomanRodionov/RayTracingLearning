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
        Camera(double fov,
               double aspect_ratio)
        {       
            double focal_length = 1.0;
            double alpha = degrees_to_radians(fov);
            double viewport_width = 2.0 * tan(alpha / 2.0) * focal_length;
            double viewport_height = viewport_width / aspect_ratio;

            origin = point3(0.0, 0.0, 0.0);
            horizontal = vec3(viewport_width, 0.0, 0.0);
            vertical = vec3(0.0, viewport_height, 0.0);
            ll_corner = origin - horizontal / 2 - vertical / 2 - vec3(0.0, 0.0, focal_length);
        }

        Ray get_ray(double u, double v) const
        {
            return Ray(origin, ll_corner + horizontal * u + vertical * v - origin);
        }
};

#endif