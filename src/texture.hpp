// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_TEXTURE_HPP
#define RAY_TRACING_TEXTURE_HPP

#include "common.hpp"

class texture
{
 public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const point3& p) const = 0;
};

#endif