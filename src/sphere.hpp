// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_SPHERE_HPP
#define RAY_TRACING_SPHERE_HPP

#include "hittable.hpp"
#include "vec3.hpp"

#include <utility>

void get_sphere_uv(const vec3& p, double& u, double& v);
vec3 random_to_sphere(double radius, double distance_squared);

class sphere final : public hittable
{
 public:
    sphere() = default;
    sphere(vec3 cen, double r, std::shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(std::move(m))
    {
        // Do nothing
    }

    bool hit(const ray& r, double t_min, double t_max,
             hit_record& rec) const override;
    bool bounding_box(double t0, double t1, aabb& output_box) const override;

    double pdf_value(const point3& o, const vec3& v) const override;

    vec3 random(const vec3& p) const override;

    vec3 center;
    double radius{ 0.0 };
    std::shared_ptr<material> mat_ptr;
};

inline bool sphere::hit(const ray& r, double t_min, double t_max,
                        hit_record& rec) const
{
    const vec3 oc = r.origin() - center;
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

            const vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;

            return true;
        }

        temp = (-half_b + root) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.at(rec.t);

            const vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;

            return true;
        }
    }

    return false;
}

inline bool sphere::bounding_box([[maybe_unused]] double t0,
                                 [[maybe_unused]] double t1,
                                 aabb& output_box) const
{
    output_box = aabb(center - vec3(radius, radius, radius),
                      center + vec3(radius, radius, radius));

    return true;
}

inline double sphere::pdf_value(const point3& o, const vec3& v) const
{
    hit_record rec;
    if (!this->hit(ray(o, v), 0.001, infinity, rec))
    {
        return 0.0;
    }

    const auto cos_theta_max =
        sqrt(1 - radius * radius / (center - o).length_squared());
    const auto solid_angle = 2 * pi * (1 - cos_theta_max);

    return 1 / solid_angle;
}

inline vec3 sphere::random(const vec3& p) const
{
    const vec3 direction = center - p;
    const auto distance_squared = direction.length_squared();

    onb uvw;
    uvw.build_from_w(direction);

    return uvw.local(random_to_sphere(radius, distance_squared));
}

inline void get_sphere_uv(const vec3& p, double& u, double& v)
{
    const auto phi = atan2(p.z(), p.x());
    const auto theta = asin(p.y());
    u = 1 - (phi + pi) / (2 * pi);
    v = (theta + pi / 2) / pi;
}

inline vec3 random_to_sphere(double radius, double distance_squared)
{
    const auto r1 = random_double();
    const auto r2 = random_double();
    const auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

    const auto phi = 2 * pi * r1;
    const auto x = cos(phi) * sqrt(1 - z * z);
    const auto y = sin(phi) * sqrt(1 - z * z);

    return vec3{x, y, z};
}

#endif