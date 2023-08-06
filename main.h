#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <limits>

#include <thread>
#include <mutex>

#include "utility/vec3.h"
#include "utility/color.h"
#include "utility/image.h"
#include "utility/ray.h"
#include "utility/objects/sphere.h"
#include "utility/objects_list.h"
#include "utility/materials/lambertian.h"
#include "utility/materials/metal.h"
#include "utility/materials/dielectric.h"
#include "utility/camera.h"
#include "utility/common.h"
#include "utility/texture.h"
#include "utility/skybox.h"
#include "utility/progress_bar.h"

#define OUTPUT_FILE "output.png"
#define SKYBOX_FILE "./data/cubemap.png"
#define IMG_WIDTH (640)
#define IMG_HEIGHT (360)
#define SAMPLES_PER_PIXEL 150
#define FOV 90
#define MAX_DEPTH 50
#define LOOK_FROM point3(-2, 1, 0.5)
#define LOOK_AT point3(0, 0,-3)
#define VIEW_UP point3(0, 1, 0)
#define APERTURE 0.1
#define THREADS_NUM 4
#define MIN_GROUP_HEIGHT 50
#define MIN_GROUP_WIDTH 50
#define MAX_GROUP_HEIGHT 100
#define MAX_GROUP_WIDTH 100

struct SceneData
{
    shared_ptr<ObjectsList> scene; 
    shared_ptr<Camera> camera;
    shared_ptr<SkyBox> sky_box;
};

inline int min(int a, int b)
{
    return a > b ? b : a;
}

inline int max(int a, int b)
{
    return a > b ? a : b;
}

color ray_color(const Ray& ray, const Object& obj, int depth, shared_ptr<SkyBox> skybox);

color compute_pixel(int i, int j,
                   int height, int width,
                   const SceneData& data);

void compute_pixel_group(int y, int x,
                   int height, int width, 
                   shared_ptr<Image> image,
                   const SceneData& data);

#endif