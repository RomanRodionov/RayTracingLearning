#include "main.h"

color ray_color(const Ray& ray, const Object& scene, int depth)
{
    hit_record hit;

    if (depth <= 0) 
    {
        return color(0.0, 0.0, 0.0);
    }

    if (scene.hit(ray, EPS, INF, hit))
    {
        Ray scattered;
        color attenuation;
        if (hit.material->scatter(ray, hit, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, scene, depth - 1);
        }
        return color(0, 0, 0);
    }
    vec3 unit_dir = unit_vector(ray.direction());
    double k = (unit_dir.y() + 1.0) * 0.5;
    return SKY * k + WHITE * (1.0 - k);
}

int main(int, char**){
    std::srand(100);

    std::string filename = "output.png";
    Image image(IMG_HEIGHT, IMG_WIDTH);

    const double aspect_ratio = (double) IMG_WIDTH / (double) IMG_HEIGHT;

    auto mat_ground = make_shared<Lambertian>(SAND);
    auto mat_left = make_shared<Lambertian>(BLUE);
    auto mat_center = make_shared<Metal>(LIGHT_GREY);
    auto mat_right = make_shared<Metal>(GREEN);

    ObjectsList scene;
    scene.add(make_shared<Sphere>(point3(0, 0, -2.0), 1.0, mat_center));
    scene.add(make_shared<Sphere>(point3(-2.0, 0, -2.0), 1.0, mat_left));
    scene.add(make_shared<Sphere>(point3(2.0, 0, -2.0), 1.0, mat_right));
    scene.add(make_shared<Sphere>(point3(0, -101.0, -2), 100.0, mat_ground));

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
                pixel_color += ray_color(ray, scene, MAX_DEPTH);
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
