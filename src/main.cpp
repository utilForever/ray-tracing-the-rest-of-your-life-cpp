// Copyright (c) 2020 Chris Ohk

// I am making my contributions/submissions to this project solely in my
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on Ray Tracing: The Rest of Your Life book.
// References: https://raytracing.github.io

#include "box.hpp"
#include "bvh.hpp"
#include "camera.hpp"
#include "checker_texture.hpp"
#include "common.hpp"
#include "constant_medium.hpp"
#include "cosine_pdf.hpp"
#include "dielectric.hpp"
#include "diffuse_light.hpp"
#include "flip_face.hpp"
#include "hittable_list.hpp"
#include "hittable_pdf.hpp"
#include "image_texture.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "mixture_pdf.hpp"
#include "moving_sphere.hpp"
#include "noise_texture.hpp"
#include "rotate_y.hpp"
#include "solid_color.hpp"
#include "sphere.hpp"
#include "translate.hpp"
#include "xy_rect.hpp"
#include "xz_rect.hpp"
#include "yz_rect.hpp"

#include <iostream>

vec3 ray_color(const ray& r, const color& background, const hittable& world,
               const std::shared_ptr<hittable>& lights, int depth)
{
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
    {
        return color{0, 0, 0};
    }

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
    {
        return background;
    }

    scatter_record srec;
    const color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, srec))
    {
        return emitted;
    }

    if (srec.is_specular)
    {
        return srec.attenuation * ray_color(srec.specular_ray, background,
                                            world, lights, depth - 1);
    }

    const auto light_ptr = std::make_shared<hittable_pdf>(lights, rec.p);
    const mixture_pdf p{light_ptr, srec.pdf_ptr};

    const ray scattered = ray{rec.p, p.generate(), r.time()};
    const auto pdf_val = p.value(scattered.direction());

    return emitted +
           srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered) *
               ray_color(scattered, background, world, lights, depth - 1) /
               pdf_val;
}

hittable_list random_scene()
{
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(
        std::make_shared<solid_color>(0.2, 0.3, 0.1),
        std::make_shared<solid_color>(0.9, 0.9, 0.9));
    world.add(std::make_shared<sphere>(point3{0, -1000, 0}, 1000,
                                       std::make_shared<lambertian>(checker)));

    for (int a = -10; a < 10; ++a)
    {
        for (int b = -10; b < 10; ++b)
        {
            const auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2,
                        b + 0.9 * random_double());

            if ((center - vec3{4, .2, 0}).length() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = vec3::random() * vec3::random();
                    world.add(std::make_shared<moving_sphere>(
                        center, center + vec3{0, random_double(0, .5), 0}, 0.0,
                        1.0, 0.2,
                        std::make_shared<lambertian>(
                            std::make_shared<solid_color>(albedo))));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = vec3::random(.5, 1);
                    auto fuzz = random_double(0, .5);
                    world.add(std::make_shared<sphere>(
                        center, 0.2, std::make_shared<metal>(albedo, fuzz)));
                }
                else
                {
                    // glass
                    world.add(std::make_shared<sphere>(
                        center, 0.2, std::make_shared<dielectric>(1.5)));
                }
            }
        }
    }

    world.add(std::make_shared<sphere>(vec3{0, 1, 0}, 1.0,
                                       std::make_shared<dielectric>(1.5)));
    world.add(std::make_shared<sphere>(
        vec3{-4, 1, 0}, 1.0,
        std::make_shared<lambertian>(
            std::make_shared<solid_color>(0.4, 0.2, 0.1))));
    world.add(std::make_shared<sphere>(
        vec3{4, 1, 0}, 1.0, std::make_shared<metal>(vec3{0.7, 0.6, 0.5}, 0.0)));

    return world;
}

hittable_list two_spheres()
{
    hittable_list objects;

    auto checker = std::make_shared<checker_texture>(
        std::make_shared<solid_color>(0.2, 0.3, 0.1),
        std::make_shared<solid_color>(0.9, 0.9, 0.9));

    objects.add(std::make_shared<sphere>(
        point3(0, -10, 0), 10, std::make_shared<lambertian>(checker)));
    objects.add(std::make_shared<sphere>(
        point3(0, 10, 0), 10, std::make_shared<lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres()
{
    hittable_list objects;

    const auto pertext = std::make_shared<noise_texture>(4.0);

    objects.add(std::make_shared<sphere>(
        point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
    objects.add(std::make_shared<sphere>(
        point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    return objects;
}

hittable_list earth()
{
    auto earth_texture = std::make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = std::make_shared<lambertian>(earth_texture);
    const auto globe =
        std::make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list simple_light()
{
    hittable_list objects;

    auto pertext = std::make_shared<noise_texture>(4);
    objects.add(std::make_shared<sphere>(
        point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
    objects.add(std::make_shared<sphere>(
        point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    auto difflight =
        std::make_shared<diffuse_light>(std::make_shared<solid_color>(4, 4, 4));
    objects.add(std::make_shared<sphere>(point3(0, 7, 0), 2, difflight));
    objects.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

hittable_list cornell_box(camera& cam, double aspect)
{
    hittable_list world;

    auto red = std::make_shared<lambertian>(
        std::make_shared<solid_color>(.65, .05, .05));
    auto white = std::make_shared<lambertian>(
        std::make_shared<solid_color>(.73, .73, .73));
    auto green = std::make_shared<lambertian>(
        std::make_shared<solid_color>(.12, .45, .15));
    auto light = std::make_shared<diffuse_light>(
        std::make_shared<solid_color>(15, 15, 15));

    world.add(std::make_shared<flip_face>(
        std::make_shared<yz_rect>(0, 555, 0, 555, 555, green)));
    world.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    world.add(std::make_shared<flip_face>(
        std::make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    world.add(std::make_shared<flip_face>(
        std::make_shared<xz_rect>(0, 555, 0, 555, 555, white)));
    world.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    world.add(std::make_shared<flip_face>(
        std::make_shared<xy_rect>(0, 555, 0, 555, 555, white)));

    std::shared_ptr<material> aluminum =
        std::make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    std::shared_ptr<hittable> box1 =
        std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), aluminum);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, vec3(265, 0, 295));
    world.add(std::move(box1));

    std::shared_ptr<material> glass = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(190, 90, 190), 90, glass));

    const point3 lookfrom(278, 278, -800);
    const point3 lookat(278, 278, 0);
    const vec3 vup(0, 1, 0);
    const auto dist_to_focus = 10.0;
    const auto aperture = 0.0;
    const auto vfov = 40.0;
    const auto t0 = 0.0;
    const auto t1 = 1.0;

    cam = camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus,
                 t0, t1);

    return world;
}

hittable_list cornell_smoke()
{
    hittable_list objects;

    auto red = std::make_shared<lambertian>(
        std::make_shared<solid_color>(.65, .05, .05));
    auto white = std::make_shared<lambertian>(
        std::make_shared<solid_color>(.73, .73, .73));
    auto green = std::make_shared<lambertian>(
        std::make_shared<solid_color>(.12, .45, .15));
    auto light =
        std::make_shared<diffuse_light>(std::make_shared<solid_color>(7, 7, 7));

    objects.add(std::make_shared<flip_face>(
        std::make_shared<yz_rect>(0, 555, 0, 555, 555, green)));
    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(std::make_shared<flip_face>(
        std::make_shared<xz_rect>(0, 555, 0, 555, 555, white)));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<flip_face>(
        std::make_shared<xy_rect>(0, 555, 0, 555, 555, white)));

    std::shared_ptr<hittable> box1 =
        std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, vec3(265, 0, 295));

    std::shared_ptr<hittable> box2 =
        std::make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, vec3(130, 0, 65));

    objects.add(std::make_shared<constant_medium>(
        box1, 0.01, std::make_shared<solid_color>(0, 0, 0)));
    objects.add(std::make_shared<constant_medium>(
        box2, 0.01, std::make_shared<solid_color>(1, 1, 1)));

    return objects;
}

hittable_list final_scene()
{
    hittable_list boxes1;
    auto ground = std::make_shared<lambertian>(
        std::make_shared<solid_color>(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++)
    {
        for (int j = 0; j < boxes_per_side; j++)
        {
            const auto w = 100.0;
            const auto x0 = -1000.0 + i * w;
            const auto z0 = -1000.0 + j * w;
            const auto y0 = 0.0;
            const auto x1 = x0 + w;
            const auto y1 = random_double(1, 101);
            const auto z1 = z0 + w;

            boxes1.add(std::make_shared<box>(point3(x0, y0, z0),
                                             point3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;

    objects.add(std::make_shared<bvh_node>(boxes1, 0, 1));

    auto light =
        std::make_shared<diffuse_light>(std::make_shared<solid_color>(7, 7, 7));
    objects.add(std::make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material = std::make_shared<lambertian>(
        std::make_shared<solid_color>(0.7, 0.3, 0.1));
    objects.add(std::make_shared<moving_sphere>(center1, center2, 0, 1, 50,
                                                moving_sphere_material));

    objects.add(std::make_shared<sphere>(point3(260, 150, 45), 50,
                                         std::make_shared<dielectric>(1.5)));
    objects.add(std::make_shared<sphere>(
        point3(0, 150, 145), 50,
        std::make_shared<metal>(color(0.8, 0.8, 0.9), 10.0)));

    auto boundary = std::make_shared<sphere>(point3(360, 150, 145), 70,
                                             std::make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(std::make_shared<constant_medium>(
        boundary, 0.2, std::make_shared<solid_color>(0.2, 0.4, 0.9)));
    boundary = std::make_shared<sphere>(point3(0, 0, 0), 5000,
                                        std::make_shared<dielectric>(1.5));
    objects.add(std::make_shared<constant_medium>(
        boundary, .0001, std::make_shared<solid_color>(1, 1, 1)));

    auto emat = std::make_shared<lambertian>(
        std::make_shared<image_texture>("earthmap.jpg"));
    objects.add(std::make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = std::make_shared<noise_texture>(0.1);
    objects.add(std::make_shared<sphere>(
        point3(220, 280, 300), 80, std::make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = std::make_shared<lambertian>(
        std::make_shared<solid_color>(.73, .73, .73));
    const int ns = 1000;
    for (int j = 0; j < ns; j++)
    {
        boxes2.add(std::make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    objects.add(std::make_shared<translate>(
        std::make_shared<rotate_y>(std::make_shared<bvh_node>(boxes2, 0.0, 1.0),
                                   15),
        vec3(-100, 270, 395)));

    return objects;
}

int main()
{
    const int image_width = 600;
    const int image_height = 600;
    const int samples_per_pixel = 1000;
    const int max_depth = 50;
    const auto aspect_ratio = static_cast<double>(image_width) / image_height;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    const color background{0, 0, 0};

    camera cam;
    const auto world = cornell_box(cam, aspect_ratio);

    const auto lights = std::make_shared<hittable_list>();
    lights->add(std::make_shared<xz_rect>(213, 343, 227, 332, 554,
                                          std::make_shared<material>()));
    lights->add(std::make_shared<sphere>(point3{190, 90, 190}, 90,
                                         std::make_shared<material>()));

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color;

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                const auto u = (i + random_double()) / (image_width - 1);
                const auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color +=
                    ray_color(r, background, world, lights, max_depth);
            }

            pixel_color.write_color(std::cout, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}