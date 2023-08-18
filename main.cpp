#include "main.h"

void random_spheres(shared_ptr<Image>& image)
{
    auto sky_box = make_shared<SkyBox>(make_shared<MyImageTexture>(make_shared<Image>(SKYBOX_FILE)));

    ObjectsList scene = ObjectsList();

    auto checker = make_shared<CheckerTexture>(0.3, BROWN, LIGHT_GREY);
    auto ground_material = make_shared<Lambertian>(checker);
    scene.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

    auto earth_texture = make_shared<ImageTexture>("./data/earth.jpg");
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = rand_double();
            point3 center(a + 0.9*rand_double(), 0.2, b + 0.9*rand_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.01)
                {
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Lambertian>(earth_texture);
                    point3 center2 = center + vec3(0, 0.3 + rand_double(0, 0.5), 0);
                    scene.add(make_shared<Sphere>(center2, 0.5, sphere_material));
                } else if (choose_mat < 0.5) {
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

    shared_ptr<Camera> camera = make_shared<Camera>();
    SceneData scene_data = {world, sky_box};

    camera->render(image, scene_data);
}

void marble_sphere(shared_ptr<Image>& image)
{
    auto sky_box = make_shared<SkyBox>(WHITE * 0.1, WHITE * 0.2);
    ObjectsList scene = ObjectsList();

    auto ground_texture = make_shared<ImageTexture>("./data/ground.jpg", 4.0);
    auto ground = make_shared<Lambertian>(ground_texture);

    auto marble_texture = make_shared<MarbleTexture>(1.0 / 4.0);
    auto marble = make_shared<Metal>(marble_texture, 0.5);

    scene.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, marble));
    scene.add(make_shared<Plane>(point3(0,0,0), vec3(1, 0, 0), vec3(0, 0, 1), ground));

    auto difflight = make_shared<DiffuseLight>(WHITE * 4);
    scene.add(make_shared<Sphere>(point3(1.2, 0.2, -0.5), 0.1, difflight));

    shared_ptr<ObjectsList> world = make_shared<ObjectsList>(ObjectsList(make_shared<bvh_node>(scene)));

    CameraSettings settings(point3(1.0, 1.0, -3.0), point3(0, 0, 0), 90);
    settings.aperture = 0.0;
    shared_ptr<Camera> camera = make_shared<Camera>(settings);
    SceneData scene_data = {world, sky_box};

    camera->render(image, scene_data);
}

void quads(shared_ptr<Image>& image)
{
    ObjectsList world = ObjectsList();

    auto left_red     = make_shared<Lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<Lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<Lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<Lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<Lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<Quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<Quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<Quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<Quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<Quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    auto difflight = make_shared<DiffuseLight>(WHITE * 4);
    world.add(make_shared<Sphere>(point3(0, 0, 2), 0.5, difflight));

    shared_ptr<ObjectsList> bvh = make_shared<ObjectsList>(ObjectsList(make_shared<bvh_node>(world)));

    CameraSettings settings(point3(2, 0, 9), point3(0, 0, 0), 120);
    shared_ptr<Camera> camera = make_shared<Camera>(settings);
    SceneData scene_data = {bvh, nullptr};

    camera->render(image, scene_data);
}

int main(int, char**){
    std::srand(0);

    std::string filename = OUTPUT_FILE;
    shared_ptr<Image> image = make_shared<Image>(IMG_HEIGHT, IMG_WIDTH);

    marble_sphere(image);

    image->save_to_png(filename);
    std::cout << std::endl << "File " << filename << " has been saved" << std::endl;;
}
