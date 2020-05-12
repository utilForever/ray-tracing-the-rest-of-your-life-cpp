// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_LAMBERTIAN_HPP
#define RAY_TRACING_LAMBERTIAN_HPP

#include "hittable.hpp"
#include "material.hpp"
#include "onb.hpp"
#include "pdf.hpp"
#include "texture.hpp"

#include <utility>

class lambertian final : public material
{
 public:
    lambertian(std::shared_ptr<texture> a) : albedo(std::move(a))
    {
        // Do nothing
    }

    bool scatter([[maybe_unused]] const ray& r_in, const hit_record& rec,
                 scatter_record& srec) const override
    {
        srec.is_specular = false;
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = std::make_shared<cosine_pdf>(rec.normal);

        return true;
    }

    double scattering_pdf([[maybe_unused]] const ray& r_in,
                          const hit_record& rec,
                          const ray& scattered) const override
    {
        const auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine < 0 ? 0 : cosine / pi;
    }

    std::shared_ptr<texture> albedo;
};

#endif