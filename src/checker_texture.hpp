// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_CHECKER_TEXTURE_HPP
#define RAY_TRACING_CHECKER_TEXTURE_HPP

#include "texture.hpp"

#include <utility>

class checker_texture final : public texture
{
 public:
    checker_texture() = default;
    checker_texture(std::shared_ptr<texture> t0, std::shared_ptr<texture> t1)
        : odd(std::move(t1)), even(std::move(t0))
    {
        // Do nothing
    }

    color value(double u, double v, const point3& p) const override
    {
        const auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());

        if (sines < 0)
        {
            return odd->value(u, v, p);
        }
        else
        {
            return even->value(u, v, p);
        }
    }

    std::shared_ptr<texture> odd;
    std::shared_ptr<texture> even;
};

#endif