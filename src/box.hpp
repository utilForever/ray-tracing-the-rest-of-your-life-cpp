// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_BOX_HPP
#define RAY_TRACING_BOX_HPP

#include "flip_face.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "xy_rect.hpp"
#include "xz_rect.hpp"
#include "yz_rect.hpp"

class box final : public hittable
{
 public:
    box() = default;
    box(const point3& p0, const point3& p1,
        const std::shared_ptr<material>& ptr);

    bool hit(const ray& r, double t0, double t1,
             hit_record& rec) const override;
    bool bounding_box([[maybe_unused]] double t0, [[maybe_unused]] double t1,
                      aabb& output_box) const override
    {
        output_box = aabb(box_min, box_max);
        return true;
    }

    point3 box_min;
    point3 box_max;
    hittable_list sides;
};

inline box::box(const point3& p0, const point3& p1,
                const std::shared_ptr<material>& ptr)
    : box_min(p0), box_max(p1)
{
    sides.add(
        std::make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(std::make_shared<flip_face>(std::make_shared<xy_rect>(
        p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr)));

    sides.add(
        std::make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(std::make_shared<flip_face>(std::make_shared<xz_rect>(
        p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr)));

    sides.add(
        std::make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(std::make_shared<flip_face>(std::make_shared<yz_rect>(
        p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr)));
}

inline bool box::hit(const ray& r, double t0, double t1, hit_record& rec) const
{
    return sides.hit(r, t0, t1, rec);
}

#endif