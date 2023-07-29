#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3
{
    private:
        double data[3];
    public:
        vec3() : data{0, 0, 0} {}
        vec3(double e0, double e1, double e2) : data{e0, e1, e2} {}
        double& x() {return data[0];}
        double& y() {return data[1];}
        double& z() {return data[2];}
        vec3 operator-() const {return vec3(-data[0], -data[1], -data[2]);}
        double& operator[](int idx) {return data[idx];}
        double operator[](int idx) const {return data[idx];}
        vec3 operator+(vec3& vec) const
        {
            return vec3(data[0] + vec.data[0], data[1] + vec.data[1], data[2] + vec.data[2]);
        }
        vec3 operator-(vec3& vec) const
        {
            return vec3(data[0] - vec.data[0], data[1] - vec.data[1], data[2] - vec.data[2]);
        }
        vec3 operator*(vec3& vec) const
        {
            return vec3(data[0] * vec.data[0], data[1] * vec.data[1], data[2] * vec.data[2]);
        }
        vec3 operator/(vec3& vec) const
        {
            return vec3(data[0] / vec.data[0], data[1] / vec.data[1], data[2] / vec.data[2]);
        }
        vec3& operator+=(vec3& vec)
        {
            data[0] += vec.data[0];
            data[1] += vec.data[1];
            data[2] += vec.data[2];
            return *this;
        }
        vec3& operator-=(vec3& vec)
        {
            data[0] -= vec.data[0];
            data[1] -= vec.data[1];
            data[2] -= vec.data[2];
            return *this;
        }
        vec3& operator*=(vec3& vec)
        {
            data[0] *= vec.data[0];
            data[1] *= vec.data[1];
            data[2] *= vec.data[2];
            return *this;
        }
        vec3& operator/=(vec3& vec)
        {
            data[0] /= vec.data[0];
            data[1] /= vec.data[1];
            data[2] /= vec.data[2];
            return *this;
        }
        vec3 operator*(double k) const
        {
            return vec3(data[0] * k, data[1] * k, data[2] * k);
        }
        vec3 operator/(double k) const
        {
            return vec3(data[0] / k, data[1] / k, data[2] / k);
        }
        vec3& operator*=(double k)
        {
            data[0] *= k;
            data[1] *= k;
            data[2] *= k;
            return *this;
        }
        vec3& operator/=(double k)
        {
            data[0] /= k;
            data[1] /= k;
            data[2] /= k;
            return *this;
        }
        double square() const
        {
            return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
        }
        double length() const
        {
            return sqrt(square());
        }
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &vec)
{
    return out << "[" << vec[0] << ", " << vec[1] << ", " << vec[2] << "]" << std::endl;
}

inline double dot(const vec3& a, const vec3& b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline vec3 cross(const vec3& a, const vec3& b)
{
    return vec3(a[1] * b[2] - a[2] * b[1],
                a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0]);
}

inline vec3 unit_vector(vec3 vec)
{
    return vec / vec.length();
}

#endif