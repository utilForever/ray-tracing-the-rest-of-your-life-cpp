// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_HITTABLE_LIST_HPP
#define RAY_TRACING_HITTABLE_LIST_HPP

#include "hittable.hpp"

#include <memory>
#include <utility>
#include <vector>

class hittable_list final : public hittable
{
 public:
    hittable_list() = default;
    hittable_list(std::shared_ptr<hittable> object)
    {
        add(std::move(object));
    }

    void clear()
    {
        objects.clear();
    }

    void add(std::shared_ptr<hittable>&& object)
    {
        objects.emplace_back(object);
    }

    bool hit(const ray& r, double t_min, double t_max,
             hit_record& rec) const override;
    bool bounding_box(double t0, double t1, aabb& output_box) const override;

    double pdf_value(const point3& o, const vec3& v) const override;

    vec3 random(const vec3& p) const override;

    std::vector<std::shared_ptr<hittable>> objects;
};

inline bool hittable_list::hit(const ray& r, double t_min, double t_max,
                               hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

inline bool hittable_list::bounding_box(double t0, double t1,
                                        aabb& output_box) const
{
    if (objects.empty())
    {
        return false;
    }

    const aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects)
    {
        if (!object->bounding_box(t0, t1, output_box))
        {
            return false;
        }

        output_box =
            first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

inline double hittable_list::pdf_value(const point3& o, const vec3& v) const
{
    const auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto& object : objects)
    {
        sum += weight * object->pdf_value(o, v);
    }

    return sum;
}

inline vec3 hittable_list::random(const vec3& o) const
{
    const auto int_size = static_cast<int>(objects.size());
    return objects[random_int(0, int_size - 1)]->random(o);
}

#endif