#include "main.h"

color ray_color(const Ray& ray, const Object& obj)
{
    hit_record rec;
    if (obj.hit(ray, 0.0, std::numeric_limits<double>::max(), rec))
    {
        return (rec.normal + vec3(1.0, 1.0, 1.0)) * 0.5;
    }
    vec3 unit_dir = unit_vector(ray.direction());
    double k = (unit_dir.y() + 1.0) * 0.5;
    return color(1.0, 0.5, 0.35) * k + color(1.0, 1.0, 1.0) * (1.0 - k);
}

int main(int, char**){
    std::string filename = "output.png";
    Image image(IMG_HEIGHT, IMG_WIDTH);

    const double aspect_ratio = (double) IMG_WIDTH / (double) IMG_HEIGHT;

    ObjectsList scene;
    scene.add(make_shared<Sphere>(point3(0, 0, -2.0), 1.0));
    scene.add(make_shared<Sphere>(point3(0, -101.0, -2), 100.0));

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
            color color = ray_color(ray, scene);
            image.draw_pixel(IMG_HEIGHT - i - 1, j, color);
        }
    }
    image.save_to_png(filename);
    std::cout << "File " << filename << " has been saved" << std::endl;;
}
