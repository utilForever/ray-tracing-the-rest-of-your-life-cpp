// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_ROTATE_Y_HPP
#define RAY_TRACING_ROTATE_Y_HPP

#include "hittable.hpp"

#include <utility>

class rotate_y final : public hittable
{
 public:
    rotate_y(std::shared_ptr<hittable> p, double angle);

    bool hit(const ray& r, double t_min, double t_max,
             hit_record& rec) const override;
    bool bounding_box(double t0, double t1, aabb& output_box) const override
    {
        output_box = bbox;
        return hasbox;
    }

    std::shared_ptr<hittable> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    aabb bbox;
};

inline rotate_y::rotate_y(std::shared_ptr<hittable> p, double angle)
    : ptr(std::move(p))
{
    const auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    point3 min{infinity, infinity, infinity};
    point3 max{-infinity, -infinity, -infinity};

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                const auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                const auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                const auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

                const auto newx = cos_theta * x + sin_theta * z;
                const auto newz = -sin_theta * x + cos_theta * z;

                vec3 tester{newx, y, newz};

                for (int c = 0; c < 3; ++c)
                {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

inline bool rotate_y::hit(const ray& r, double t_min, double t_max,
                          hit_record& rec) const
{
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

    const ray rotated_r{origin, direction, r.time()};

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
    {
        return false;
    }

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
    p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

    normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
    normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

#endif