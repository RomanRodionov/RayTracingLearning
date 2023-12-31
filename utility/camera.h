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

struct CameraSettings
{
    point3 look_from;
    point3 look_at;
    double fov;
    vec3 vup;
    double aspect_ratio;
    double aperture;
    double focus_dist;
    CameraSettings( point3 _look_from,
                    point3 _look_at,
                    double _fov,
                    vec3 _vup,
                    double _aspect_ratio,
                    double _aperture,
                    double _focus_dist) :
                look_from(_look_from),
                look_at(_look_at),
                fov(_fov),
                vup(_vup),
                aspect_ratio(_aspect_ratio),
                aperture(_aperture),
                focus_dist(_focus_dist) {}
    CameraSettings(point3 _look_from = LOOK_FROM,
               point3 _look_at = LOOK_AT,
               double _fov = FOV,
               vec3 _vup = VIEW_UP,
               double _aspect_ratio = ASPECT_RATIO,
               double _aperture = APERTURE) :
                CameraSettings(_look_from, _look_at, _fov, _vup, _aspect_ratio, _aperture,
            (_look_at - _look_from).length()) {}
    CameraSettings() : CameraSettings(LOOK_FROM, LOOK_AT, FOV, VIEW_UP, ASPECT_RATIO, APERTURE,
            (LOOK_AT - LOOK_FROM).length()) {}
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
        shared_ptr<Texture> background;
    public:
        Camera(point3 look_from,
               point3 look_at,
               double fov,
               vec3 vup,
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

            background = make_shared<SolidColor>(BLACK);
        }
        Camera(point3 look_from = LOOK_FROM,
               point3 look_at = LOOK_AT,
               double fov = FOV,
               vec3 vup = VIEW_UP,
               double aspect_ratio = ASPECT_RATIO,
               double aperture = APERTURE)
            : Camera(look_from, look_at, fov, vup, aspect_ratio, aperture,
            (look_at - look_from).length()) {}
        Camera(const CameraSettings& settings)
            : Camera(settings.look_from, settings.look_at, settings.fov, settings.vup,
            settings.aspect_ratio, settings.aperture, settings.focus_dist) {}

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
        color ray_color(const Ray& ray, shared_ptr<Object> scene, int depth, shared_ptr<SkyBox> skybox = nullptr)
        {
            hit_record hit;

            if (depth <= 0) 
            {
                return color(0.0, 0.0, 0.0);
            }

            if (!scene->hit(ray, Interval(EPS, INF), hit))
            {
                vec3 dir = unit_vector(ray.direction());
                if (skybox != nullptr)
                    return skybox->get_color(dir);
                else
                {
                    double u, v;
                    Sphere::get_sphere_uv(dir, u, v);
                    return background->value(u, v, dir);
                }
            }
            Ray scattered;
            color attenuation;
            color emitted_light = hit.material->emitted(hit.u, hit.v, hit.p);

            if (!hit.material->scatter(ray, hit, attenuation, scattered))
                return emitted_light;

            color scattered_light = ray_color(scattered, scene, depth - 1, skybox) * attenuation;
            
            return emitted_light + scattered_light;
        }
        color compute_pixel(int i, int j,
                    shared_ptr<Image> image,
                    const SceneData& data,
                    int samples_per_pixel)
        {
            color pixel_color(0, 0, 0);
            int width = image->width(), height = image->height();
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
            int height = image->height();
            int width = image->width();
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