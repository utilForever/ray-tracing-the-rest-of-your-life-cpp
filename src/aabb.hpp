// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_AABB_HPP
#define RAY_TRACING_AABB_HPP

#include "common.hpp"

class aabb
{
 public:
    aabb() = default;
    aabb(const point3& a, const point3& b) : _min(a), _max(b)
    {
        // Do nothing
    }

    point3 min() const
    {
        return _min;
    }

    point3 max() const
    {
        return _max;
    }

    bool hit(const ray& r, double tmin, double tmax) const;

    point3 _min;
    point3 _max;
};

inline bool aabb::hit(const ray& r, double tmin, double tmax) const
{
    for (int a = 0; a < 3; ++a)
    {
        const auto invD = 1.0f / r.direction()[a];
        auto t0 = (min()[a] - r.origin()[a]) * invD;
        auto t1 = (max()[a] - r.origin()[a]) * invD;

        if (invD < 0.0f)
        {
            std::swap(t0, t1);
        }

        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;

        if (tmax <= tmin)
        {
            return false;
        }
    }

    return true;
}

inline aabb surrounding_box(aabb box0, aabb box1)
{
    const point3 small(fmin(box0.min().x(), box1.min().x()),
                       fmin(box0.min().y(), box1.min().y()),
                       fmin(box0.min().z(), box1.min().z()));
    const point3 big(fmax(box0.max().x(), box1.max().x()),
                     fmax(box0.max().y(), box1.max().y()),
                     fmax(box0.max().z(), box1.max().z()));

    return aabb(small, big);
}

#endif