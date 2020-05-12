// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_DIELECTRIC_HPP
#define RAY_TRACING_DIELECTRIC_HPP

#include "hittable.hpp"
#include "material.hpp"

class dielectric final : public material
{
 public:
    dielectric(double ri) : ref_idx(ri)
    {
        // Do nothing
    }

    bool scatter(const ray& r_in, const hit_record& rec,
                 scatter_record& srec) const override
    {
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        srec.attenuation = color{1.0, 1.0, 1.0};

        const vec3 unit_direction = unit_vector(r_in.direction());
        const double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        const double etai_over_etat =
            (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);
        if (etai_over_etat * sin_theta > 1.0)
        {
            const vec3 reflected = reflect(unit_direction, rec.normal);
            srec.specular_ray = ray{rec.p, reflected, r_in.time()};

            return true;
        }

        const double reflect_prob = schlick(cos_theta, etai_over_etat);
        if (random_double() < reflect_prob)
        {
            const vec3 reflected = reflect(unit_direction, rec.normal);
            srec.specular_ray = ray{rec.p, reflected, r_in.time()};

            return true;
        }

        const vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        srec.specular_ray = ray{rec.p, refracted, r_in.time()};

        return true;
    }

    double ref_idx;
};

#endif