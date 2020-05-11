// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_XZ_RECT_HPP
#define RAY_TRACING_XZ_RECT_HPP

#include "hittable.hpp"

#include <utility>

class xz_rect final : public hittable
{
 public:
    xz_rect() = default;
    xz_rect(double _x0, double _x1, double _z0, double _z1, double _k,
            std::shared_ptr<material> mat)
        : mp(std::move(mat)), x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k)
    {
        // Do nothing
    }

    bool hit(const ray& r, double t0, double t1,
             hit_record& rec) const override;
    bool bounding_box([[maybe_unused]] double t0, [[maybe_unused]] double t1,
                      aabb& output_box) const override
    {
        // The bounding box must have non-zero width in each dimension,
        // so pad the Y dimension a small amount.
        output_box =
            aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
        return true;
    }

    double pdf_value(const point3& origin, const vec3& v) const override
    {
        hit_record rec;
        if (!this->hit(ray{origin, v}, 0.001, infinity, rec))
        {
            return 0.0;
        }

        const auto area = (x1 - x0) * (z1 - z0);
        const auto distance_squared = rec.t * rec.t * v.length_squared();
        const auto cosine = fabs(dot(v, rec.normal) / v.length());

        return distance_squared / (cosine * area);
    }

    vec3 random(const vec3& origin) const override
    {
        const auto random_point =
            point3{random_double(x0, x1), k, random_double(z0, z1)};
        return random_point - origin;
    }

    std::shared_ptr<material> mp;
    double x0 = 0.0, x1 = 0.0, z0 = 0.0, z1 = 0.0, k = 0.0;
};

inline bool xz_rect::hit(const ray& r, double t0, double t1,
                         hit_record& rec) const
{
    const auto t = (k - r.origin().y()) / r.direction().y();
    if (t < t0 || t > t1)
    {
        return false;
    }

    const auto x = r.origin().x() + t * r.direction().x();
    const auto z = r.origin().z() + t * r.direction().z();

    if (x < x0 || x > x1 || z < z0 || z > z1)
    {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;

    const auto outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

#endif