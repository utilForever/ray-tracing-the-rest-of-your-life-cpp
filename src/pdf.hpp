// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_PDF_HPP
#define RAY_TRACING_PDF_HPP

#include "common.hpp"

inline vec3 random_cosine_direction()
{
    const auto r1 = random_double();
    const auto r2 = random_double();
    const auto z = sqrt(1 - r2);

    const auto phi = 2 * pi * r1;
    const auto x = cos(phi) * sqrt(r2);
    const auto y = sin(phi) * sqrt(r2);

    return vec3{x, y, z};
}

#endif