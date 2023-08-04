#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <limits>

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

#define IMG_WIDTH (640 * 1)
#define IMG_HEIGHT (360 * 1)
#define SAMPLES_PER_PIXEL 50
#define FOV 120
#define MAX_DEPTH 50

color ray_color(const Ray& ray, const Object& obj, int depth, shared_ptr<SkyBox> skybox);

#endif