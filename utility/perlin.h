#ifndef PERLIN_H
#define PERLIN_H

#include "common.h"

class Perlin
{
    private:
        static const int point_count = 256;
        static const unsigned int mask = point_count - 1;
        vec3* ranvec;
        int* perm_x;
        int* perm_y;
        int* perm_z;

        static int* perlin_generate_perm()
        {
            auto p = new int[point_count];
            for (int i = 0; i < Perlin::point_count; ++i)
            {
                p[i] = i;
            }

            permute(p, point_count);

            return p;
        }

        static void permute(int* p, int n)
        {
            for (int i = n - 1; i > 0; --i)
            {
                std::swap(p[i], p[rand_int(0, i)]);
            }
        }
        static double trilinear_interp(vec3 values[2][2][2], double u, double v, double w)
        {
            double uu = u * u * (3 - 2 * u);
            double vv = v * v * (3 - 2 * v);
            double ww = w * w * (3 - 2 * w);
            double sum = 0;
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    for (int k = 0; k < 2; ++k)
                    {
                        vec3 weight(u - i, v - j, w - k);
                        sum += (i * uu + (1 - i) * (1 - uu)) *
                               (j * vv + (1 - j) * (1 - vv)) *
                               (k * ww + (1 - k) * (1 - ww)) * 
                               dot(values[i][j][k], weight);
                    }

            return sum;
        }
    public:
        Perlin()
        {
            ranvec = new vec3[point_count];
            for (int i = 0; i < point_count; ++i)
            {
                ranvec[i] = unit_vector(vec3::random(-1, 1));
            }

            perm_x = perlin_generate_perm();
            perm_y = perlin_generate_perm();
            perm_z = perlin_generate_perm();
        }
        ~Perlin()
        {
            delete[] ranvec;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }
        double noise(const point3& p) const
        {
            double u = p.x() - floor(p.x());
            double v = p.y() - floor(p.y());
            double w = p.z() - floor(p.z());

            int i = static_cast<int>(floor(p.x()));
            int j = static_cast<int>(floor(p.y()));
            int k = static_cast<int>(floor(p.z()));

            vec3 values[2][2][2];

            for (int di = 0; di < 2; ++di)
                for (int dj = 0; dj < 2; ++dj)
                    for (int dk = 0; dk < 2; ++dk)
                        values[di][dj][dk] = ranvec[perm_x[(i + di) & mask] ^
                                                      perm_y[(j + dj) & mask] ^
                                                      perm_z[(k + dk) & mask]];

            return trilinear_interp(values, u, v, w);
        }
        double turb(const point3& p, int depth = 7.0) const
        {
            double sum = 0.0;
            point3 temp_p = p;
            double weight = 1.0;

            for (int i = 0; i < depth; ++i)
            {
                sum += weight * noise(temp_p);
                weight *= 0.5;
                temp_p *= 2;
            }
            return fabs(sum);
        }
};

#endif