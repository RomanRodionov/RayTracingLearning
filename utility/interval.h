#ifndef INTERVAL_H
#define INTERVAL_H

#include "common.h"

class Interval
{
    public:
        double min, max;
        Interval() : min(+INF), max(-INF) {}
        Interval(double _min, double _max) : min(_min), max(_max) {}
        Interval(const Interval& a, const Interval& b) :
            min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

        double size() const
        {
            return max - min;
        }
        bool contains(double x) const 
        {
            return min <= x && x <= max;
        }
        bool surrounds(double x) const
        {
            return min < x && x < max;
        }
        double clamp(double x) const
        {
            if (x < min) {return min;}
            if (x > max) {return max;}
            return x;
        }
        Interval expand(double delta) const
        {
            auto padding = delta / 2;
            return Interval(min - padding, max + padding);
        }
        
        static const Interval empty, universe;
};

const static Interval empty(+INF, -INF);
const static Interval universe(-INF, +INF);

Interval operator+(const Interval& ival, double offset)
{
    return Interval(ival.min + offset, ival.max + offset);
}

Interval operator+(double offset, const Interval& ival)
{
    return ival + offset;
}

#endif