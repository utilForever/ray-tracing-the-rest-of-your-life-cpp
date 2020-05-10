// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_MATERIAL_HPP
#define RAY_TRACING_MATERIAL_HPP

#include "common.hpp"

struct hit_record;

class material
{
 public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         vec3& attenuation, ray& scattered) const = 0;

    virtual color emitted([[maybe_unused]] double u, [[maybe_unused]] double v,
                          [[maybe_unused]] const point3& p) const
    {
        return color(0, 0, 0);
    }
};

inline double schlick(double cosine, double ref_idx)
{
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

#endif