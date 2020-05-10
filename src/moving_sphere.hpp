// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_MOVING_SPHERE_HPP
#define RAY_TRACING_MOVING_SPHERE_HPP

#include "hittable.hpp"
#include "vec3.hpp"

#include <utility>

class moving_sphere final : public hittable
{
 public:
    moving_sphere() = default;
    moving_sphere(vec3 cen0, vec3 cen1, double t0, double t1, double r,
                  std::shared_ptr<material> m)
        : center0(cen0),
          center1(cen1),
          time0(t0),
          time1(t1),
          radius(r),
          mat_ptr(std::move(m))
    {
        // Do nothing
    }

    bool hit(const ray& r, double t_min, double t_max,
             hit_record& rec) const override;
    bool bounding_box(double t0, double t1, aabb& output_box) const override;

    vec3 center(double time) const;

    vec3 center0, center1;
    double time0{0.0}, time1{0.0};
    double radius{0.0};
    std::shared_ptr<material> mat_ptr;
};

inline bool moving_sphere::hit(const ray& r, double t_min, double t_max,
                               hit_record& rec) const
{
    const vec3 oc = r.origin() - center(r.time());
    const auto a = r.direction().length_squared();
    const auto half_b = dot(oc, r.direction());
    const auto c = oc.length_squared() - radius * radius;
    const auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0)
    {
        const auto root = sqrt(discriminant);
        auto temp = (-half_b - root) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.at(rec.t);

            const vec3 outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;

            return true;
        }

        temp = (-half_b + root) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.at(rec.t);

            const vec3 outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;

            return true;
        }
    }

    return false;
}

inline bool moving_sphere::bounding_box(double t0, double t1,
                                        aabb& output_box) const
{
    const aabb box0(center(t0) - vec3(radius, radius, radius),
                    center(t0) + vec3(radius, radius, radius));
    const aabb box1(center(t1) - vec3(radius, radius, radius),
                    center(t1) + vec3(radius, radius, radius));

    output_box = surrounding_box(box0, box1);

    return true;
}

inline vec3 moving_sphere::center(double time) const
{
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

#endif