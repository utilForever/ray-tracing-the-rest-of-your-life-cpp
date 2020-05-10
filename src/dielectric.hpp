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

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
                 ray& scattered) const override
    {
        attenuation = vec3{1.0, 1.0, 1.0};
        const double etai_over_etat =
            rec.front_face ? (1.0 / ref_idx) : ref_idx;

        const vec3 unit_direction = unit_vector(r_in.direction());
        const double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
        const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0)
        {
            const vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray{rec.p, reflected};

            return true;
        }

        const double reflect_prob = schlick(cos_theta, etai_over_etat);
        if (random_double() < reflect_prob)
        {
            const vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray{rec.p, reflected};

            return true;
        }

        const vec3 refracted =
            refract(unit_direction, rec.normal, etai_over_etat);
        scattered = ray{rec.p, refracted};

        return true;
    }

    double ref_idx;
};

#endif