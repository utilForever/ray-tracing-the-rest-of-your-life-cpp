// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_FLIP_FACE_HPP
#define RAY_TRACING_FLIP_FACE_HPP

#include "hittable.hpp"

#include <utility>

class flip_face final : public hittable
{
 public:
    flip_face(std::shared_ptr<hittable> p) : ptr(std::move(p))
    {
        // Do nothing
    }

    bool hit(const ray& r, double t_min, double t_max,
             hit_record& rec) const override
    {
        if (!ptr->hit(r, t_min, t_max, rec))
        {
            return false;
        }

        rec.front_face = !rec.front_face;
        return true;
    }

    bool bounding_box(double t0, double t1, aabb& output_box) const override
    {
        return ptr->bounding_box(t0, t1, output_box);
    }

    std::shared_ptr<hittable> ptr;
};

#endif