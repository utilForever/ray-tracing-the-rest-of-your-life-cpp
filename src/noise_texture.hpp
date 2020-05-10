// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_NOISE_TEXTURE_HPP
#define RAY_TRACING_NOISE_TEXTURE_HPP

#include "perlin.hpp"
#include "texture.hpp"

class noise_texture final : public texture
{
 public:
    noise_texture() = default;
    noise_texture(double sc) : scale(sc)
    {
        // Do nothing
    }

    color value([[maybe_unused]] double u, [[maybe_unused]] double v,
                const point3& p) const override
    {
        return color(1, 1, 1) * 0.5 *
               (1 + sin(scale * p.z() + 10 * noise.turb(p)));
    }

    perlin noise;
    double scale = 0.0;
};

#endif