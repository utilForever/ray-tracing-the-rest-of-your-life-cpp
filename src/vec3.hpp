// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_VEC3_HPP
#define RAY_TRACING_VEC3_HPP

#include <algorithm>
#include <iostream>

class vec3
{
 public:
    vec3() : e{0, 0, 0}
    {
        // Do nothing
    }

    vec3(double e0, double e1, double e2) : e{e0, e1, e2}
    {
        // Do nothing
    }

    vec3 operator-() const
    {
        return vec3{-e[0], -e[1], -e[2]};
    }

    double operator[](int i) const
    {
        return e[i];
    }

    double& operator[](int i)
    {
        return e[i];
    }

    vec3& operator+=(const vec3& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    vec3& operator*=(const double t)
    {
        e[0] += t;
        e[1] += t;
        e[2] += t;

        return *this;
    }

    vec3& operator/=(const double t)
    {
        return *this *= (1 / t);
    }

    double x() const
    {
        return e[0];
    }

    double y() const
    {
        return e[1];
    }

    double z() const
    {
        return e[2];
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    void write_color(std::ostream& out, int samples_per_pixel)
    {
        // Divide the color total by the number of samples and gamma-correct
        // for a gamma value of 2.0.
        const auto scale = 1.0 / samples_per_pixel;
        const auto r = sqrt(scale * e[0]);
        const auto g = sqrt(scale * e[1]);
        const auto b = sqrt(scale * e[2]);

        // Write the translated [0,255] value of each color component.
        out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
    }

    static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max),
                    random_double(min, max));
    }

    double e[3];
};

// Type aliases for vec3
using point3 = vec3;    // 3D point
using color = vec3;     // RGB color

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
    return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline vec3 operator*(double t, const vec3& v)
{
    return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline vec3 operator*(const vec3& v, double t)
{
    return t * v;
}

inline vec3 operator/(vec3 v, double t)
{
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v)
{
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
            u.e[2] * v.e[0] - u.e[0] * v.e[2],
            u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}

inline vec3 random_in_unit_sphere()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1)
        {
            continue;
        }

        return p;
    }
}

inline vec3 random_unit_vector()
{
    const auto a = random_double(0, 2 * pi);
    const auto z = random_double(-1, 1);
    const auto r = sqrt(1 - z * z);

    return vec3{r * std::cos(a), r * std::sin(a), z};
}

inline vec3 random_in_hemisphere(const vec3& normal)
{
    const vec3 in_unit_sphere = random_in_unit_sphere();

    // In the same hemisphere as the normal
    if (dot(in_unit_sphere, normal) > 0.0)
    {
        return in_unit_sphere;
    }
    else
    {
        return -in_unit_sphere;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
    const auto cos_theta = dot(-uv, n);
    const vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    const vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n;

    return r_out_parallel + r_out_perp;
}

inline vec3 random_in_unit_disk()
{
    while (true)
    {
        auto p = vec3{random_double(-1, 1), random_double(-1, 1), 0};

        if (p.length_squared() >= 1)
        {
            continue;
        }

        return p;
    }
}

#endif