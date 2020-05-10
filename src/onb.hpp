// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_ONB_HPP
#define RAY_TRACING_ONB_HPP

#include "vec3.hpp"

class onb
{
 public:
    vec3 operator[](int i) const
    {
        return axis[i];
    }

    vec3 u() const
    {
        return axis[0];
    }
    vec3 v() const
    {
        return axis[1];
    }
    vec3 w() const
    {
        return axis[2];
    }

    vec3 local(double a, double b, double c) const
    {
        return a * u() + b * v() + c * w();
    }

    vec3 local(const vec3& a) const
    {
        return a.x() * u() + a.y() * v() + a.z() * w();
    }

    void build_from_w(const vec3& n);

    vec3 axis[3];
};

inline void onb::build_from_w(const vec3& n)
{
    axis[2] = unit_vector(n);
    const vec3 a = (fabs(w().x()) > 0.9) ? vec3{0, 1, 0} : vec3{1, 0, 0};
    axis[1] = unit_vector(cross(w(), a));
    axis[0] = cross(w(), v());
}

#endif