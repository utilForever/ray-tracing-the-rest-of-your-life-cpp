// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_PERLIN_HPP
#define RAY_TRACING_PERLIN_HPP

#include "common.hpp"

inline double trilinear_interp(double c[2][2][2], double u, double v, double w)
{
    auto accum = 0.0;

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                accum += (i * u + (1 - i) * (1 - u)) *
                         (j * v + (1 - j) * (1 - v)) *
                         (k * w + (1 - k) * (1 - w)) * c[i][j][k];
            }
        }
    }

    return accum;
}

class perlin
{
 public:
    perlin()
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

    ~perlin()
    {
        delete[] ranvec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double noise(const point3& p) const
    {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());
        const int i = static_cast<int>(floor(p.x()));
        const int j = static_cast<int>(floor(p.y()));
        const int k = static_cast<int>(floor(p.z()));
        vec3 c[2][2][2];

        for (int di = 0; di < 2; ++di)
        {
            for (int dj = 0; dj < 2; ++dj)
            {
                for (int dk = 0; dk < 2; ++dk)
                {
                    c[di][dj][dk] =
                        ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^
                               perm_z[(k + dk) & 255]];
                }
            }
        }

        return perlin_interp(c, u, v, w);
    }

    double turb(const point3& p, int depth = 7) const
    {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i)
        {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return fabs(accum);
    }

 private:
    static int* perlin_generate_perm()
    {
        const auto p = new int[point_count];

        for (int i = 0; i < point_count; ++i)
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
            const int target = random_int(0, i);

            const int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double perlin_interp(vec3 c[2][2][2], double u, double v, double w)
    {
        const auto uu = u * u * (3 - 2 * u);
        const auto vv = v * v * (3 - 2 * v);
        const auto ww = w * w * (3 - 2 * w);
        auto accum = 0.0;

        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    vec3 weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) *
                             (j * vv + (1 - j) * (1 - vv)) *
                             (k * ww + (1 - k) * (1 - ww)) *
                             dot(c[i][j][k], weight_v);
                }
            }
        }

        return accum;
    }

    static const int point_count = 256;
    vec3* ranvec;
    int* perm_x;
    int* perm_y;
    int* perm_z;
};

#endif