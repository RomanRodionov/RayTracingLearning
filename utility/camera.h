#ifndef CAMERA_H
#define CAMERA_H

#include <omp.h>

#include "common.h"
#include "ray.h"
#include "skybox.h"
#include "object.h"
#include "objects_list.h"
#include "progress_bar.h"

class Camera;

struct SceneData
{
    shared_ptr<ObjectsList> scene;
    shared_ptr<SkyBox> sky_box;
};

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
        Camera(point3 look_from = LOOK_FROM,
               point3 look_at = LOOK_AT,
               vec3 vup = VIEW_UP,
               double fov = FOV,
               double aspect_ratio = ASPECT_RATIO,
               double aperture = APERTURE)
            : Camera(look_from, look_at, vup, fov, aspect_ratio, aperture,
            (look_at - look_from).length()) {}

        Ray get_ray(double u_p, double v_p) const
        {
            vec3 rd = random_in_unit_disk() * lens_radius;
            vec3 offset = u * rd.x() + v * rd.y();
            vec3 shifted_origin = origin + offset;
            vec3 ray_dir = ll_corner + horizontal * u_p + vertical * v_p - shifted_origin;
            double ray_time = rand_double();

            return Ray(shifted_origin, ray_dir, ray_time);
        }
    private:
        color ray_color(const Ray& ray, shared_ptr<Object> scene, int depth, shared_ptr<SkyBox> skybox)
        {
            hit_record hit;

            if (depth <= 0) 
            {
                return color(0.0, 0.0, 0.0);
            }

            if (scene->hit(ray, Interval(EPS, INF), hit))
            {
                Ray scattered;
                color attenuation;
                if (hit.material->scatter(ray, hit, attenuation, scattered))
                {
                    return attenuation * ray_color(scattered, scene, depth - 1, skybox);
                }
                return color(0, 0, 0);
            }
            return skybox->get_color(ray.direction());
        }
        color compute_pixel(int i, int j,
                    shared_ptr<Image> image,
                    const SceneData& data,
                    int samples_per_pixel)
        {
            color pixel_color(0, 0, 0);
            int width = image->get_width(), height = image->get_height();
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                double v = (i + rand_double()) / (height - 1);
                double u = (j + rand_double()) / (width - 1);
                Ray ray = get_ray(u, v);
                pixel_color += Camera::ray_color(ray, data.scene, MAX_DEPTH, data.sky_box);
            }
            pixel_color /= samples_per_pixel;
            return pixel_color;
        }
    public:
        void render(shared_ptr<Image> image, const SceneData& scene_data)
        {
            int height = image->get_height();
            int width = image->get_width();
            ProgressBar bar(height * width);
            bar.print("Rendering in progress");

            for (int i = 0; i < height; ++i)
            {
                #pragma omp parallel for
                for (int j = 0; j < width; ++j)
                {
                    color pixel = compute_pixel(i, j, image, scene_data, SAMPLES_PER_PIXEL);
                    #pragma omp critical
                    {
                        image->draw_pixel(height - i - 1, j, pixel);
                    }  
                }
                bar.increase(width);
                bar.update();
            }
        }
};

#endif