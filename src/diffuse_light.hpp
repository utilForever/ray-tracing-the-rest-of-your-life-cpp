// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_DIFFUSE_LIGHT_HPP
#define RAY_TRACING_DIFFUSE_LIGHT_HPP

#include "material.hpp"
#include "texture.hpp"

#include <utility>

class diffuse_light final : public material
{
 public:
    diffuse_light(std::shared_ptr<texture> a) : emit(std::move(a))
    {
        // Do nothing
    }

    bool scatter([[maybe_unused]] const ray& r_in,
                 [[maybe_unused]] const hit_record& rec,
                 [[maybe_unused]] color& alb, [[maybe_unused]] ray& scattered,
                 [[maybe_unused]] double& pdf) const override
    {
        return false;
    }

    color emitted(double u, double v, const point3& p) const override
    {
        return emit->value(u, v, p);
    }

    std::shared_ptr<texture> emit;
};

#endif