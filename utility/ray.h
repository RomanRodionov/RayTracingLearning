#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray 
{
    private:
        vec3 orig;
        vec3 dir; 
        double tm;      
    public:
        Ray() {}
        Ray(const point3& origin, const vec3& direction, double time = 0.0) 
            : orig(origin), dir(direction), tm(time) {}
        point3 origin() const {return orig;}
        vec3 direction() const {return dir;}
        double time() const {return tm;}
        vec3 at(double t) const {return orig + dir * t;}
};

#endif