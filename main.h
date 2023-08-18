#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <limits>

#include "utility/common.h"
#include "utility/image.h"
#include "utility/ray.h"
#include "utility/objects/sphere.h"
#include "utility/objects/plane.h"
#include "utility/objects/quad.h"
#include "utility/objects_list.h"
#include "utility/materials/lambertian.h"
#include "utility/materials/metal.h"
#include "utility/materials/dielectric.h"
#include "utility/materials/light.h"
#include "utility/camera.h"
#include "utility/my_image_texture.h"
#include "utility/skybox.h"
#include "utility/progress_bar.h"
#include "utility/bvh.h"

#define OUTPUT_FILE "output.png"

#endif