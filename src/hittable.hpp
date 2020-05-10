// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_HITTABLE_HPP
#define RAY_TRACING_HITTABLE_HPP

#include "aabb.hpp"
#include "ray.hpp"

class material;

struct hit_record
{
    vec3 p;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;
    double t{0.0};
    double u{0.0};
    double v{0.0};
    bool front_face{false};

    void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        // dot(r.direction(), outward_normal) > 0.0  => ray is inside the sphere
        // dot(r.direction(), outward_normal) <= 0.0 => ray is outside the
        // sphere
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
 public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const = 0;
    virtual bool bounding_box(double t0, double t1, aabb& output_box) const = 0;
};

#endif