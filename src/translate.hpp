// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_TRANSLATE_HPP
#define RAY_TRACING_TRANSLATE_HPP

#include "hittable.hpp"

#include <utility>

class translate final : public hittable
{
 public:
    translate(std::shared_ptr<hittable> p, const vec3& displacement)
        : ptr(std::move(p)), offset(displacement)
    {
        // Do nothing
    }

    bool hit(const ray& r, double t_min, double t_max,
             hit_record& rec) const override;
    bool bounding_box(double t0, double t1, aabb& output_box) const override;

    std::shared_ptr<hittable> ptr;
    vec3 offset;
};

inline bool translate::hit(const ray& r, double t_min, double t_max,
                           hit_record& rec) const
{
    const ray moved_r{r.origin() - offset, r.direction(), r.time()};
    if (!ptr->hit(moved_r, t_min, t_max, rec))
    {
        return false;
    }

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

inline bool translate::bounding_box(double t0, double t1,
                                    aabb& output_box) const
{
    if (!ptr->bounding_box(t0, t1, output_box))
    {
        return false;
    }

    output_box = aabb{output_box.min() + offset, output_box.max() + offset};

    return true;
}

#endif