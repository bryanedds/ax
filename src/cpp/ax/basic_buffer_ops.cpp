#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <string>
#include <cmath>

#include "ax/basic_buffer_ops.hpp"

namespace ax
{
    void draw_dot(const ax::color& color, int x, int y, ax::basic_buffer& buffer)
    {
        buffer.set_pixel(x, y, { 0.0f, ax::zero<ax::v3>(), color });
    }

    void draw_line(const ax::color& color, int x, int y, int x2, int y2, ax::basic_buffer& buffer)
    {
        // local functions
        struct local
        {
            static void set_point_normal(int x, int y, float z, const ax::color& c, ax::basic_buffer& buffer) { buffer.set_pixel(x, y, { z, ax::zero<ax::v3>(), c }); }
            static void set_point_swapped(int x, int y, float z, const ax::color& c, ax::basic_buffer& buffer) { buffer.set_pixel(y, x, { z, ax::zero<ax::v3>(), c }); }
        };

        // determine steepness
        VAL steep = std::abs(x - x2) < std::abs(y - y2);

        // transpose
        if (steep)
        {
            std::swap(x, y);
            std::swap(x2, y2);
        }

        // invert
        if (x > x2)
        {
            std::swap(x, x2);
            std::swap(y, y2);
        }

        // draw loop
        VAL x_delta = x2 - x;
        VAL y_delta = y2 - y;
        VAL error_delta = std::abs(y_delta) * 2;
        VAL error_heading = y2 > y ? 1 : -1;
        VAL set_point_local = steep ? &local::set_point_swapped : &local::set_point_normal;
        for (VAR error = 0, i = x, j = y; i < x2; ++i)
        {
            set_point_local(i, j, 0.0f, color, buffer);
            error += error_delta;
            if (error > x_delta)
            {
                j += error_heading;
                error -= x_delta * 2;
            }
        }
    }

    void draw_wired_ortho(const ax::color& color, const ax::line2& line, ax::basic_buffer& buffer)
    {
        VAL& center = ax::v2(static_cast<float>(buffer.get_width()), static_cast<float>(buffer.get_height())) * 0.5f;
        VAL x = static_cast<int>((line.first.x + 1.0f) * center.x);
        VAL y = static_cast<int>((line.first.y + 1.0f) * center.y);
        VAL x2 = static_cast<int>((line.second.x + 1.0f) * center.x);
        VAL y2 = static_cast<int>((line.second.y + 1.0f) * center.y);
        ax::draw_line(color, x, y, x2, y2, buffer);
    }

    void draw_wired_ortho(const ax::color& color, const ax::triangle2& triangle, ax::basic_buffer& buffer)
    {
        ax::draw_wired_ortho(color, ax::line2(std::get<0>(triangle), std::get<1>(triangle)), buffer);
        ax::draw_wired_ortho(color, ax::line2(std::get<1>(triangle), std::get<2>(triangle)), buffer);
        ax::draw_wired_ortho(color, ax::line2(std::get<2>(triangle), std::get<0>(triangle)), buffer);
    }

    void draw_wired_ortho(const ax::color& color, const ax::basic_model& model, ax::basic_buffer& buffer)
    {
        for (VAR i = 0_z; i < model.get_faces().size(); ++i)
        {
            VAL& face = model.get_face(i);
            VAL& triangle = ax::triangle3(model.get_position(face[0]), model.get_position(face[1]), model.get_position(face[2]));
            VAL& triangle_ortho = ax::get_ortho(triangle);
            draw_wired_ortho(color, triangle_ortho, buffer);
        }
    }

    void draw_textured_ortho(const ax::v3& light, const ax::basic_surface& surface, const ax::triangle2& uvs, const ax::triangle3& triangle, ax::basic_buffer& buffer)
    {
        // compute triangle space (aka, tangent-space)
        VAL& triangle_tangent = (std::get<1>(triangle) - std::get<0>(triangle)).NormalizeSafe();
        VAL& triangle_tangent_2 = (std::get<2>(triangle) - std::get<0>(triangle)).NormalizeSafe();
        VAL& triangle_normal = (triangle_tangent ^ triangle_tangent_2).NormalizeSafe();
        VAL& triangle_binormal = (triangle_tangent ^ triangle_normal).NormalizeSafe();
        VAL& triangle_space = ax::matrix4(
            triangle_tangent.x,     triangle_tangent.y,     triangle_tangent.z,     0.0f,
            triangle_binormal.x,    triangle_binormal.y,    triangle_binormal.z,    0.0f,
            triangle_normal.x,      triangle_normal.y,      triangle_normal.z,      0.0f,
            0.0f,                   0.0f,                   0.0f,                   1.0f);

        // compute triangle in screen-space
        VAL& center_screen = ax::v2(static_cast<float>(buffer.get_width()), static_cast<float>(buffer.get_height())) * 0.5f;
        VAL& triangle_ortho = ax::get_ortho(triangle);
        VAL& triangle_screen = ax::triangle2(
            (std::get<0>(triangle_ortho) + ax::one<ax::v2>()).SymMul(center_screen),
            (std::get<1>(triangle_ortho) + ax::one<ax::v2>()).SymMul(center_screen),
            (std::get<2>(triangle_ortho) + ax::one<ax::v2>()).SymMul(center_screen));

        // compute bounds in screen-space
        VAL& bounds_screen = ax::get_bounds(triangle_screen);
        VAL width_screen = static_cast<int>(bounds_screen.second.x);
        VAL height_screen = static_cast<int>(bounds_screen.second.y);

        // render triangle pixels in the screen-space of the computed bounds
        for (VAR j = static_cast<int>(bounds_screen.first.y); j <= height_screen; ++j)
        {
            for (VAR i = static_cast<int>(bounds_screen.first.x); i <= width_screen; ++i)
            {
                // check that pixel is inside triangle
                if (ax::get_in_bounds(ax::v2(static_cast<float>(i), static_cast<float>(j)), triangle_screen))
                {
                    // get a reference to the current pixel
                    VAR& pixel_in_place = buffer.get_pixel_in_place(i, j);

                    // compute the write-depth in screen-space
                    VAL& point_screen = ax::v2(static_cast<float>(i), static_cast<float>(j));
                    VAL& coords_screen = ax::get_barycentric_coords(point_screen, triangle_screen);
                    VAL depth_screen =
                        std::get<0>(triangle).z * coords_screen.x +
                        std::get<1>(triangle).z * coords_screen.y +
                        std::get<2>(triangle).z * coords_screen.z;

                    // ensure write-depth is greater than depth of current pixel
                    if (depth_screen >= pixel_in_place.depth)
                    {
                        // compute uv coordinate in screen-space
                        VAL uv_screen =
                            std::get<0>(uvs) * coords_screen.x +
                            std::get<1>(uvs) * coords_screen.y +
                            std::get<2>(uvs) * coords_screen.z;

                        // sample the specular map
                        VAL specular = surface.get_specular_map().sample_specular(uv_screen);

                        // sample the tangent map, computing its normal
                        VAL& tangent = surface.get_tangent_map().sample_tangent(uv_screen);
                        VAR normal = tangent + ax::v3(0.5f);
                        if (normal.x > 1.0f) normal.x -= 1.0f;
                        if (normal.y > 1.0f) normal.y -= 1.0f;
                        if (normal.z > 1.0f) normal.z -= 1.0f;

                        // sample the diffuse map
                        VAL& diffuse = surface.get_diffuse_map().sample_diffuse(uv_screen);

                        // compute the light normal in triangle-space
                        VAL& normal_triangle = triangle_space * normal;
                        VAL light_normal_triangle = std::abs(light * normal_triangle);

                        // compute the color in screen-space
                        VAL& color_screen = ax::color(
                            static_cast<uint8_t>(ax::saturate(diffuse.r * light_normal_triangle + diffuse.r * specular, 255.0f)),
                            static_cast<uint8_t>(ax::saturate(diffuse.g * light_normal_triangle + diffuse.g * specular, 255.0f)),
                            static_cast<uint8_t>(ax::saturate(diffuse.b * light_normal_triangle + diffuse.b * specular, 255.0f)),
                            diffuse.a);

                        // update the current pixel
                        pixel_in_place = ax::basic_pixel(depth_screen, triangle_normal, color_screen);
                    }
                }
            }
        }
    }

    void draw_textured_ortho(const ax::v3& light, const ax::basic_model& model, ax::basic_buffer& buffer)
    {
        VAL& forward = ax::v3(0.0f, 0.0f, 1.0f);
        VAL& surface = model.get_surface();
        for (VAR i = 0_z; i < model.get_faces().size(); ++i)
        {
            VAL& face = model.get_face(i);
            VAL& triangle = ax::triangle3(model.get_position(face[0]), model.get_position(face[1]), model.get_position(face[2]));
            VAL& normal = ax::get_normal(triangle);
            VAL not_back_face = normal * forward > 0;
            if (not_back_face)
            {
                VAL& uvs = ax::triangle2(model.get_uv(i, 0), model.get_uv(i, 1), model.get_uv(i, 2));
                draw_textured_ortho(light, surface, uvs, triangle, buffer);
            }
        }
    }
}
