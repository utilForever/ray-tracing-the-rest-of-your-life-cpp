// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_XY_RECT_HPP
#define RAY_TRACING_XY_RECT_HPP

#include "hittable.hpp"

#include <utility>

class xy_rect final : public hittable
{
 public:
    xy_rect() = default;
    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k,
            std::shared_ptr<material> mat)
        : mp(std::move(mat)), x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k)
    {
        // Do nothing
    }

    bool hit(const ray& r, double t0, double t1,
             hit_record& rec) const override;
    bool bounding_box([[maybe_unused]] double t0, [[maybe_unused]] double t1,
                      aabb& output_box) const override
    {
        // The bounding box must have non-zero width in each dimension,
        // so pad the Z dimension a small amount.
        output_box =
            aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
        return true;
    }

    std::shared_ptr<material> mp;
    double x0 = 0.0, x1 = 0.0, y0 = 0.0, y1 = 0.0, k = 0.0;
};

inline bool xy_rect::hit(const ray& r, double t0, double t1,
                         hit_record& rec) const
{
    const auto t = (k - r.origin().z()) / r.direction().z();
    if (t < t0 || t > t1)
    {
        return false;
    }

    const auto x = r.origin().x() + t * r.direction().x();
    const auto y = r.origin().y() + t * r.direction().y();

    if (x < x0 || x > x1 || y < y0 || y > y1)
    {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;

    const auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

#endif