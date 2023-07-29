#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray 
{
    private:
        vec3 orig;
        vec3 dir;       
    public:
        Ray() {}
        Ray(const point3& origin, const vec3& direction) 
            : orig(origin), dir(direction) {}
        point3 origin() const {return orig;}
        vec3 direction() const {return dir;}
        vec3 at(double t) const {return orig + dir * t;}
};

#endif