// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_COSINE_PDF_HPP
#define RAY_TRACING_COSINE_PDF_HPP

#include "onb.hpp"
#include "pdf.hpp"

class cosine_pdf final : public pdf
{
 public:
    cosine_pdf(const vec3& w)
    {
        uvw.build_from_w(w);
    }

    double value(const vec3& direction) const override
    {
        const auto cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine / pi;
    }

    vec3 generate() const override
    {
        return uvw.local(random_cosine_direction());
    }

    onb uvw;
};

#endif