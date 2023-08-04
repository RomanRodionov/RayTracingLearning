#include "main.h"

color ray_color(const Ray& ray, const Object& scene, int depth, shared_ptr<SkyBox> skybox)
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
            return attenuation * ray_color(scattered, scene, depth - 1, skybox);
        }
        return color(0, 0, 0);
    }
    return skybox->get_color(ray.direction());
    /*
    double k = (unit_dir.y() + 1.0) * 0.5;
    return SKY * k + WHITE * (1.0 - k);
    */
}

int main(int, char**){
    std::srand(100);

    std::string filename = OUTPUT_FILE;
    Image image(IMG_HEIGHT, IMG_WIDTH);

    const double aspect_ratio = (double) IMG_WIDTH / (double) IMG_HEIGHT;

    auto mat_ground = make_shared<Lambertian>(SAND);
    auto glass1 = make_shared<Dielectric>(1.5);
    auto glass2 = make_shared<Dielectric>(2.5);
    auto mat_center = make_shared<Metal>(LIGHT_GREY, 0.0);
    auto mat_right = make_shared<Metal>(LIGHT_AQUA, 0.3);
    auto mat_small = make_shared<Metal>(RED, 0.8);

    auto sky_box = make_shared<SkyBox>(make_shared<Texture>(make_shared<Image>(SKYBOX_FILE)));

    ObjectsList scene;
    scene.add(make_shared<Sphere>(point3(0, 0, -3.0), 1.0, mat_center));
    scene.add(make_shared<Sphere>(point3(-2.0, 0, -3.0), 1.0, glass1));
    scene.add(make_shared<Sphere>(point3(-2.0, 0, -3.0), -0.9, glass1));
    scene.add(make_shared<Sphere>(point3(2.0, 0, -3.0), 1.0, mat_right));
    scene.add(make_shared<Sphere>(point3(0, -10001.0, -3), 10000.0, mat_ground));
    scene.add(make_shared<Sphere>(point3(1.0, -0.75, -1.5), 0.25, mat_small));
    scene.add(make_shared<Sphere>(point3(-0.5, -0.6, -2), 0.4, glass2));

    double focus_dist = (LOOK_FROM - LOOK_AT).length();

    Camera camera(LOOK_FROM, LOOK_AT, VIEW_UP, FOV, aspect_ratio, APERTURE, focus_dist);
    
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
                pixel_color += ray_color(ray, scene, MAX_DEPTH, sky_box);
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
