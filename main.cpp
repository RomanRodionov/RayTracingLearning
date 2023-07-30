#include <iostream>
#include <vector>
#include <array>
#include <string>

#include "utility/vec3.h"
#include "utility/color.h"
#include "utility/image.h"
#include "utility/ray.h"

#define IMG_WIDTH 960
#define IMG_HEIGHT 540

double hit_sphere(const point3& center, double radius, const Ray& ray)
{
    vec3 oc = ray.origin() - center;
    double a = ray.direction().square();
    double hb = dot(oc, ray.direction());
    double c = oc.square() - radius * radius;
    double discr = hb * hb - a * c;
    if (discr < 0)
    {
        return -1.0;
    }
    else
    {
        return (-hb - sqrt(discr)) / a;
    }
}

color ray_color(const Ray& ray)
{
    point3 center = point3(0, 0,-2);
    double t = hit_sphere(center, 1.0, ray);
    if (t >= 0)
    {
        vec3 n = unit_vector(ray.at(t) - center);
        return color(n[0] + 1.0, n[1] + 1.0, n[2] + 1.0) * 0.5;
    }
    vec3 unit_dir = unit_vector(ray.direction());
    double k = (unit_dir.y() + 1.0) * 0.5;
    return color(1.0, 0.5, 0.35) * k + color(1.0, 1.0, 1.0) * (1.0 - k);
}

int main(int, char**){
    std::string filename = "output.png";
    Image image(IMG_HEIGHT, IMG_WIDTH);

    const double aspect_ratio = (double) IMG_WIDTH / (double) IMG_HEIGHT;

    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    point3 origin(0, 0, 0);
    vec3 horizontal(viewport_width, 0, 0);
    vec3 vertical(0, viewport_height, 0);
    point3 ll_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);


    for (int i = 0; i < IMG_HEIGHT; ++i)
    {
        for (int j = 0; j < IMG_WIDTH; ++j)
        {
            double v = (double) i / (IMG_HEIGHT - 1);
            double u = (double) j / (IMG_WIDTH - 1);
            Ray ray(origin, ll_corner + horizontal * u + vertical * v - origin);
            image.draw_pixel(IMG_HEIGHT - i - 1, j, ray_color(ray));
        }
    }
    image.save_to_png(filename);
    std::cout << "File " << filename << " has been saved" << std::endl;;
}
