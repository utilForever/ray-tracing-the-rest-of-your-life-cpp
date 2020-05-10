// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_BVH_HPP
#define RAY_TRACING_BVH_HPP

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

#include <algorithm>

class bvh_node final : public hittable
{
 public:
    bvh_node() = default;
    bvh_node(hittable_list& list, double time0, double time1)
        : bvh_node(list.objects, 0, list.objects.size(), time0, time1)
    {
        // Do nothing
    }
    bvh_node(std::vector<std::shared_ptr<hittable>>& objects, std::size_t start,
             std::size_t end, double time0, double time1);

    bool hit(const ray& r, double t_min, double t_max,
             hit_record& rec) const override;
    bool bounding_box(double t0, double t1, aabb& output_box) const override;

    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb box;
};

inline bool box_compare(const std::shared_ptr<hittable> a,
                        const std::shared_ptr<hittable> b, int axis)
{
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    return box_a.min().e[axis] < box_b.min().e[axis];
}

inline bool box_x_compare(const std::shared_ptr<hittable> a,
                          const std::shared_ptr<hittable> b)
{
    return box_compare(a, b, 0);
}

inline bool box_y_compare(const std::shared_ptr<hittable> a,
                          const std::shared_ptr<hittable> b)
{
    return box_compare(a, b, 1);
}

inline bool box_z_compare(const std::shared_ptr<hittable> a,
                          const std::shared_ptr<hittable> b)
{
    return box_compare(a, b, 2);
}

inline bvh_node::bvh_node(std::vector<std::shared_ptr<hittable>>& objects,
                          std::size_t start, std::size_t end, double time0,
                          double time1)
{
    const int axis = random_int(0, 2);
    const auto comparator = (axis == 0)
                                ? box_x_compare
                                : (axis == 1) ? box_y_compare : box_z_compare;
    const size_t object_span = end - start;

    if (object_span == 1)
    {
        left = right = objects[start];
    }
    else if (object_span == 2)
    {
        if (comparator(objects[start], objects[start + 1]))
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else
    {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = std::make_shared<bvh_node>(objects, start, mid, time0, time1);
        right = std::make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;

    if (!left->bounding_box(time0, time1, box_left) ||
        !right->bounding_box(time0, time1, box_right))
    {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    box = surrounding_box(box_left, box_right);
}

inline bool bvh_node::hit(const ray& r, double t_min, double t_max,
                          hit_record& rec) const
{
    if (!box.hit(r, t_min, t_max))
    {
        return false;
    }

    const bool hit_left = left->hit(r, t_min, t_max, rec);
    const bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

inline bool bvh_node::bounding_box([[maybe_unused]] double t0,
                                   [[maybe_unused]] double t1,
                                   aabb& output_box) const
{
    output_box = box;

    return true;
}

#endif