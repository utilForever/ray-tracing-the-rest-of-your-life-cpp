// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_YZ_RECT_HPP
#define RAY_TRACING_YZ_RECT_HPP

#include "hittable.hpp"

#include <utility>

class yz_rect final : public hittable
{
 public:
    yz_rect() = default;
    yz_rect(double _y0, double _y1, double _z0, double _z1, double _k,
            std::shared_ptr<material> mat)
        : mp(std::move(mat)), y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k)
    {
        // Do nothing
    }

    bool hit(const ray& r, double t0, double t1,
             hit_record& rec) const override;
    bool bounding_box([[maybe_unused]] double t0, [[maybe_unused]] double t1,
                      aabb& output_box) const override
    {
        // The bounding box must have non-zero width in each dimension,
        // so pad the X dimension a small amount.
        output_box =
            aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
        return true;
    }

    std::shared_ptr<material> mp;
    double y0 = 0.0, y1 = 0.0, z0 = 0.0, z1 = 0.0, k = 0.0;
};

inline bool yz_rect::hit(const ray& r, double t0, double t1,
                         hit_record& rec) const
{
    const auto t = (k - r.origin().x()) / r.direction().x();
    if (t < t0 || t > t1)
    {
        return false;
    }

    const auto y = r.origin().y() + t * r.direction().y();
    const auto z = r.origin().z() + t * r.direction().z();

    if (y < y0 || y > y1 || z < z0 || z > z1)
    {
        return false;
    }

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;

    const auto outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

#endif