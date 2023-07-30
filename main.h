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
#include "utility/camera.h"
#include "utility/common.h"

#define IMG_WIDTH 960
#define IMG_HEIGHT 540
#define SAMPLES_PER_PIXEL 50
#define FOV 120

color ray_color(const Ray& ray);

#endif