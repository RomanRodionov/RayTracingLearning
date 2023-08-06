#include "main.h"

color ray_color(const Ray& ray, shared_ptr<Object> scene, int depth, shared_ptr<SkyBox> skybox)
{
    hit_record hit;

    if (depth <= 0) 
    {
        return color(0.0, 0.0, 0.0);
    }

    if (scene->hit(ray, EPS, INF, hit))
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
                   int height, int width,
                   const SceneData& data)
{
    color pixel_color(0, 0, 0);
    for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
    {
        double v = (i + rand_double()) / (height - 1);
        double u = (j + rand_double()) / (width - 1);
        Ray ray = data.camera->get_ray(u, v);
        pixel_color += ray_color(ray, data.scene, MAX_DEPTH, data.sky_box);
    }
    return pixel_color;
}

std::mutex mtx;

void compute_pixel_group(int y, int x,
                   int height, int width, 
                   shared_ptr<Image> image,
                   const SceneData& data)
{
    std::vector<color> group_buffer;
    for (int i = y; i < y + height; ++i)
    {
        for (int j = x; j < x + width; ++j)
        {
            group_buffer.push_back(compute_pixel(i, j, image->get_height(), image->get_width(), data));
        }
    }
    mtx.lock();
    for (int i = y; i < y + height; ++i)
    {
        for (int j = x; j < x + width; ++j)
        {
            image->draw_pixel(image->get_height() - i - 1, j, group_buffer[(i - y) * width + j - x], SAMPLES_PER_PIXEL);
        }
    }
    mtx.unlock();
}

int main(int, char**){
    std::srand(100);

    std::string filename = OUTPUT_FILE;
    shared_ptr<Image> image = make_shared<Image>(IMG_HEIGHT, IMG_WIDTH);

    const double aspect_ratio = (double) IMG_WIDTH / (double) IMG_HEIGHT;

    auto mat_ground = make_shared<Lambertian>(SAND);
    auto glass1 = make_shared<Dielectric>(1.5);
    auto glass2 = make_shared<Dielectric>(2.5);
    auto mat_center = make_shared<Metal>(LIGHT_GREY, 0.0);
    auto mat_right = make_shared<Metal>(LIGHT_AQUA, 0.3);
    auto mat_small = make_shared<Metal>(RED, 0.8);

    auto sky_box = make_shared<SkyBox>(make_shared<Texture>(make_shared<Image>(SKYBOX_FILE)));

    shared_ptr<ObjectsList> scene = make_shared<ObjectsList>();
    scene->add(make_shared<Sphere>(point3(0, 0, -3.0), 1.0, mat_center));
    scene->add(make_shared<Sphere>(point3(-2.0, 0, -3.0), 1.0, glass1));
    scene->add(make_shared<Sphere>(point3(-2.0, 0, -3.0), -0.9, glass1));
    scene->add(make_shared<Sphere>(point3(2.0, 0, -3.0), 1.0, mat_right));
    scene->add(make_shared<Sphere>(point3(0, -10001.0, -3), 10000.0, mat_ground));
    scene->add(make_shared<Sphere>(point3(1.0, -0.75, -1.5), 0.25, mat_small));
    scene->add(make_shared<Sphere>(point3(-0.5, -0.6, -2), 0.4, glass2));

    double focus_dist = (LOOK_FROM - LOOK_AT).length();

    shared_ptr<Camera> camera = make_shared<Camera>(LOOK_FROM, LOOK_AT, VIEW_UP, FOV, aspect_ratio, APERTURE, focus_dist);
    
    SceneData scene_data = {scene, camera, sky_box};;

    ProgressBar bar(IMG_HEIGHT * IMG_WIDTH);
    bar.print("Rendering in progress");

    std::thread threads[THREADS_NUM];

    int group_width = min(max(image->get_width() / THREADS_NUM, MIN_GROUP_WIDTH), MAX_GROUP_WIDTH);
    int group_height = min(max(group_width, MIN_GROUP_HEIGHT), MAX_GROUP_HEIGHT);

    for (int i = 0; i < IMG_HEIGHT / group_height + 1 && min(IMG_HEIGHT - i * group_height, group_height) > 0; ++i)
    {
        int actual_group_height = min(IMG_HEIGHT - i * group_height, group_height);
        for (int j = 0; j < (IMG_WIDTH / group_width + 1) / THREADS_NUM + 1; ++j)
        {
            for (int t = 0; t < THREADS_NUM && (j * THREADS_NUM + t) * group_width < IMG_WIDTH; ++t)
            {
                int actual_group_width = min(group_width, IMG_WIDTH - (j * THREADS_NUM + t) * group_width);
                threads[t] = std::thread(compute_pixel_group, i * group_height, (j * THREADS_NUM + t) * group_width, actual_group_height, actual_group_width, image, scene_data);
                bar.increase(actual_group_height * actual_group_width);
            }
            for (int t = 0; t < THREADS_NUM && (j * THREADS_NUM + t) * group_width < IMG_WIDTH; ++t)
            {
                threads[t].join();
            }
            bar.update();
        }
    }
    image->save_to_png(filename);
    std::cout << std::endl << "File " << filename << " has been saved" << std::endl;;
}
