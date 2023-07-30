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

    Camera camera(FOV, aspect_ratio);
    
    std::cout << "Rendering in progress: 000%";
    int pixel_count = 0, image_size = IMG_HEIGHT * IMG_WIDTH;
    for (int i = 0; i < IMG_HEIGHT; ++i)
    {
        for (int j = 0; j < IMG_WIDTH; ++j)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
            {
                double v = (i + rand_double()) / (IMG_HEIGHT - 1);
                double u = (j + rand_double()) / (IMG_WIDTH - 1);
                Ray ray = camera.get_ray(u, v);
                pixel_color += ray_color(ray, scene);
            }
            image.draw_pixel(IMG_HEIGHT - i - 1, j, pixel_color, SAMPLES_PER_PIXEL);
            
            pixel_count++;
            double progress = (int)((double) pixel_count / image_size * 100.0);
            std::ostringstream ss;
            ss << std::setw(3) << std::setfill('0') << progress;
            std::string progress_str = ss.str();
            std::cout << "\b\b\b\b" << progress_str << "%";
        }
    }
    image.save_to_png(filename);
    std::cout << std::endl << "File " << filename << " has been saved" << std::endl;;
}
