#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <limits>

#include <thread>
#include <mutex>

#include <sstream>
#include <iomanip>

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

#define OUTPUT_FILE "output.png"
#define SKYBOX_FILE "./data/cubemap.png"
#define IMG_WIDTH (640 * 3)
#define IMG_HEIGHT (360 * 3)
#define SAMPLES_PER_PIXEL 300
#define FOV 90
#define MAX_DEPTH 50
#define LOOK_FROM point3(-2, 1, 0.5)
#define LOOK_AT point3(0, 0,-3)
#define VIEW_UP point3(0, 1, 0)
#define APERTURE 0.1
#define THREADS_NUM 4

struct SceneData
{
    shared_ptr<ObjectsList> scene; 
    shared_ptr<Camera> camera;
    shared_ptr<SkyBox> sky_box;
};

color ray_color(const Ray& ray, const Object& obj, int depth, shared_ptr<SkyBox> skybox);

#endif