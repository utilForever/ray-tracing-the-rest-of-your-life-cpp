// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_IMAGE_TEXTURE_HPP
#define RAY_TRACING_IMAGE_TEXTURE_HPP

#include "texture.hpp"

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(pop)
#endif

#include <iostream>

class image_texture final : public texture
{
 public:
    const static int bytes_per_pixel = 3;

    image_texture() = default;
    image_texture(const char* filename)
    {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(filename, &width, &height, &components_per_pixel,
                         components_per_pixel);

        if (!data)
        {
            std::cerr << "ERROR: Could not load texture image file '"
                      << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture()
    {
        delete data;
    }

    color value(double u, double v,
                [[maybe_unused]] const point3& p) const override
    {
        // If we have no texture data,
        // then return solid cyan as a debugging aid.
        if (data == nullptr)
        {
            return color(0, 1, 1);
        }

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = std::clamp(u, 0.0, 1.0);
        // Flip V to image coordinates
        v = 1.0 - std::clamp(v, 0.0, 1.0);

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        // Clamp integer mapping, since actual coordinates
        // should be less than 1.0
        if (i >= width)
        {
            i = width - 1;
        }
        if (j >= height)
        {
            j = height - 1;
        }

        const auto color_scale = 1.0 / 255.0;
        const auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return color(color_scale * pixel[0], color_scale * pixel[1],
                     color_scale * pixel[2]);
    }

 private:
    unsigned char* data = nullptr;
    int width = 0, height = 0;
    int bytes_per_scanline = 0;
};

#endif