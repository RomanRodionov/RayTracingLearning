#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

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

inline double clamp(double x, double min, double max)
{
    if (x < min) {return min;}
    if (x > max) {return max;}
    return x;
}

#endif