// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_CAMERA_HPP
#define RAY_TRACING_CAMERA_HPP

#include "common.hpp"

class camera
{
 public:
    // vfov: top to bottom, in degrees
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect,
           double aperture, double focus_dist, double t0 = 0, double t1 = 0)
    {
        origin = lookfrom;
        lens_radius = aperture / 2;
        time0 = t0;
        time1 = t1;

        const auto theta = degrees_to_radians(vfov);
        const auto half_height = tan(theta / 2);
        const auto half_width = aspect * half_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u -
                            half_height * focus_dist * v - focus_dist * w;

        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }

    ray get_ray(double s, double t) const
    {
        const vec3 rd = lens_radius * random_in_unit_disk();
        const vec3 offset = u * rd.x() + v * rd.y();

        return ray(
            origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset,
            random_double(time0, time1));
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    // shutter open/close times
    double time0, time1;
};

#endif