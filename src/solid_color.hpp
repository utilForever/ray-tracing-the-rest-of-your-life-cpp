// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_SOLID_COLOR_HPP
#define RAY_TRACING_SOLID_COLOR_HPP

#include "texture.hpp"

class solid_color final : public texture
{
 public:
    solid_color() = default;
    solid_color(color c) : color_value(c)
    {
        // Do nothing
    }
    solid_color(double red, double green, double blue)
        : solid_color(color(red, green, blue))
    {
        // Do nothing
    }

    color value([[maybe_unused]] double u, [[maybe_unused]] double v,
                [[maybe_unused]] const point3& p) const override
    {
        return color_value;
    }

 private:
    color color_value;
};

#endif