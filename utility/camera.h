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
        vec3 w, u, v;
        double lens_radius;
    public:
        Camera(point3 look_from,
               point3 look_at,
               vec3 vup,
               double fov,
               double aspect_ratio,
               double aperture,
               double focus_dist)
        {       
            double focal_length = 1.0;
            double alpha = degrees_to_radians(fov);
            double viewport_width = 2.0 * tan(alpha / 2.0) * focal_length;
            double viewport_height = viewport_width / aspect_ratio;

            w = unit_vector(look_from - look_at);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = look_from;
            horizontal = u * viewport_width * focus_dist;
            vertical = v * viewport_height * focus_dist;
            ll_corner = origin - horizontal / 2 - vertical / 2 - w * focus_dist;

            lens_radius = aperture / 2;
        }

        Ray get_ray(double u_p, double v_p) const
        {
            vec3 rd = random_in_unit_disk() * lens_radius;
            vec3 offset = u * rd.x() + v * rd.y();
            vec3 shifted_origin = origin + offset;
            vec3 ray_dir = ll_corner + horizontal * u_p + vertical * v_p - shifted_origin;
            double ray_time = rand_double();

            return Ray(shifted_origin, ray_dir, ray_time);
        }
};

#endif