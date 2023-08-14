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

#endif