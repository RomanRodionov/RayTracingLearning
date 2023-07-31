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
#include "utility/camera.h"
#include "utility/common.h"

#define IMG_WIDTH 640
#define IMG_HEIGHT 360
#define SAMPLES_PER_PIXEL 100
#define FOV 120
#define MAX_DEPTH 50
#define EPS 0.0000001

color ray_color(const Ray& ray, const Object& obj, int depth);

#endif