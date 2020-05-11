// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#ifndef RAY_TRACING_MIXTURE_PDF_HPP
#define RAY_TRACING_MIXTURE_PDF_HPP

#include "pdf.hpp"

class mixture_pdf final : public pdf
{
 public:
    mixture_pdf(std::shared_ptr<pdf> p0, std::shared_ptr<pdf> p1)
    {
        p[0] = std::move(p0);
        p[1] = std::move(p1);
    }

    double value(const vec3& direction) const override
    {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }

    vec3 generate() const override
    {
        if (random_double() < 0.5)
        {
            return p[0]->generate();
        }
        else
        {
            return p[1]->generate();
        }
    }

    std::shared_ptr<pdf> p[2];
};

#endif