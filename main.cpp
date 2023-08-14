#include "main.h"

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
    /*
    double k = (unit_dir.y() + 1.0) * 0.5;
    return SKY * k + WHITE * (1.0 - k);
    */
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
        Ray ray = data.camera->get_ray(u, v);
        pixel_color += ray_color(ray, data.scene, MAX_DEPTH, data.sky_box);
    }
    pixel_color /= samples_per_pixel;
    return pixel_color;
}

int main(int, char**){
    std::srand(100);

    std::string filename = OUTPUT_FILE;
    shared_ptr<Image> image = make_shared<Image>(IMG_HEIGHT, IMG_WIDTH);

    int height = image->get_height(), width = image->get_width();

    const double aspect_ratio = (double) width / (double) height;

    auto sky_box = make_shared<SkyBox>(make_shared<Texture>(make_shared<Image>(SKYBOX_FILE)));

    ObjectsList scene = ObjectsList();

    auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    scene.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = rand_double();
            point3 center(a + 0.9*rand_double(), 0.2, b + 0.9*rand_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.5) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    point3 center2 = center + vec3(0, rand_double(0, 0.5), 0);
                    scene.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.75) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = rand_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    scene.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    scene.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    scene.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    scene.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    scene.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    shared_ptr<ObjectsList> world = make_shared<ObjectsList>(ObjectsList(make_shared<bvh_node>(scene)));

    double focus_dist = (LOOK_FROM - LOOK_AT).length();
    shared_ptr<Camera> camera = make_shared<Camera>(LOOK_FROM, LOOK_AT, VIEW_UP, FOV, aspect_ratio, APERTURE, focus_dist);
    SceneData scene_data = {world, camera, sky_box};

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
    image->save_to_png(filename);
    std::cout << std::endl << "File " << filename << " has been saved" << std::endl;;
}
