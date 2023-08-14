#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <algorithm>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

#define EPS 0.0000001

#define SKYBOX_FILE "./data/cubemap.png"
#define IMG_WIDTH (640)
#define IMG_HEIGHT (360)
#define ASPECT_RATIO ((double) IMG_WIDTH / (double) IMG_HEIGHT)
#define SAMPLES_PER_PIXEL 50
#define MAX_DEPTH 50
#define FOV 40
#define LOOK_FROM point3(13,2,3)
#define LOOK_AT point3(0,0,0)
#define VIEW_UP vec3(0,1,0)
#define APERTURE 0.1
#define THREADS_NUM 4

inline double degrees_to_radians(double degrees)
{
    return degrees * PI / 180.0;
}

inline double rand_double()
{
    return rand() / (RAND_MAX + 1.0);
}

inline double rand_double(double min, double max)
{
    return min + (max - min) * rand_double();
}

inline double random_int(int min, int max)
{
    return min + rand() % (max - min + 1);
}

inline int min(int a, int b)
{
    return a > b ? b : a;
}

inline int max(int a, int b)
{
    return a > b ? a : b;
}

#include "vec3.h"
#include "color.h"
#include "interval.h"

#endif