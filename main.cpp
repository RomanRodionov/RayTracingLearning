#include "main.h"

void random_spheres(shared_ptr<Image>& image)
{

    int height = image->get_height(), width = image->get_width();

    const double aspect_ratio = (double) width / (double) height;

    auto checker = make_shared<CheckerTexture>(0.3, BROWN, LIGHT_GREY);

    auto sky_box = make_shared<SkyBox>(checker);

    ObjectsList scene = ObjectsList();

    auto ground_material = make_shared<Lambertian>(checker);
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
    SceneData scene_data = {world, sky_box};

    camera->render(image, scene_data);
}

int main(int, char**){
    std::srand(0);

    std::string filename = OUTPUT_FILE;
    shared_ptr<Image> image = make_shared<Image>(IMG_HEIGHT, IMG_WIDTH);

    random_spheres(image);

    image->save_to_png(filename);
    std::cout << std::endl << "File " << filename << " has been saved" << std::endl;;
}
