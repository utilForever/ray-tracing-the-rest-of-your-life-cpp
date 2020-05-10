// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_ISOTROPIC_HPP
#define RAY_TRACING_ISOTROPIC_HPP

#include "hittable.hpp"
#include "material.hpp"
#include "texture.hpp"

#include <utility>

class isotropic final : public material
{
 public:
    isotropic(std::shared_ptr<texture> a) : albedo(std::move(a))
    {
        // Do nothing
    }

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
                 ray& scattered) const override
    {
        scattered = ray{rec.p, random_in_unit_sphere(), r_in.time()};
        attenuation = albedo->value(rec.u, rec.v, rec.p);

        return true;
    }

    std::shared_ptr<texture> albedo;
};

#endif