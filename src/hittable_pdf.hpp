// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_HITTABLE_PDF_HPP
#define RAY_TRACING_HITTABLE_PDF_HPP

#include "hittable.hpp"
#include "pdf.hpp"

class hittable_pdf final : public pdf
{
 public:
    hittable_pdf(std::shared_ptr<hittable> p, const point3& origin)
        : o(origin), ptr(std::move(p))
    {
        // Do nothing
    }

    double value(const vec3& direction) const override
    {
        return ptr->pdf_value(o, direction);
    }

    vec3 generate() const override
    {
        return ptr->random(o);
    }

    point3 o;
    std::shared_ptr<hittable> ptr;
};

#endif