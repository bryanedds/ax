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
        for (VAR i = 0; i < model.get_faces().size(); i++)
        {
            VAL& face = model.get_face(i);
			VAL& triangle = ax::triangle3(model.get_position(face[0]), model.get_position(face[1]), model.get_position(face[2]));
            VAL& triangle_ortho = ax::get_ortho(triangle);
            draw_wired_ortho(color, triangle_ortho, buffer);
        }
    }

    void draw_textured_ortho(float light, const ax::basic_surface& surface, const ax::triangle2& uvs, const ax::triangle3& triangle, ax::basic_buffer& buffer)
    {
        VAL& center_screen = ax::v2(static_cast<float>(buffer.get_width()), static_cast<float>(buffer.get_height())) * 0.5f;
        VAL& triangle_ortho = ax::get_ortho(triangle);
        VAL& triangle_screen = ax::triangle2(
            (std::get<0>(triangle_ortho) + ax::v2(1.0f, 1.0f)).SymMul(center_screen),
            (std::get<1>(triangle_ortho) + ax::v2(1.0f, 1.0f)).SymMul(center_screen),
            (std::get<2>(triangle_ortho) + ax::v2(1.0f, 1.0f)).SymMul(center_screen));
        VAL& bounds_screen = ax::get_bounds(triangle_screen);
        VAL width = static_cast<int>(bounds_screen.second.x);
        VAL height = static_cast<int>(bounds_screen.second.y);
        for (VAR i = static_cast<int>(bounds_screen.first.x); i <= width; ++i)
        {
            for (VAR j = static_cast<int>(bounds_screen.first.y); j <= height; ++j)
            {
                if (ax::get_in_bounds(ax::v2(static_cast<float>(i), static_cast<float>(j)), triangle_screen))
                {
                    VAR& pixel_in_place = buffer.get_pixel_in_place(i, j);
                    VAL& point_screen = ax::v2(static_cast<float>(i), static_cast<float>(j));
                    VAL& coords_screen = ax::get_barycentric_coords(point_screen, triangle_screen);
                    VAL depth =
                        std::get<0>(triangle).z * coords_screen.x +
                        std::get<1>(triangle).z * coords_screen.y +
                        std::get<2>(triangle).z * coords_screen.z;
                    if (depth >= pixel_in_place.depth)
                    {
                        VAL uv =
                            std::get<0>(uvs) * coords_screen.x +
                            std::get<1>(uvs) * coords_screen.y +
                            std::get<2>(uvs) * coords_screen.z;
                        VAL& color_diffuse = surface.get_diffuse_map().sample_diffuse(uv);
                        VAL& color = ax::color(
                            static_cast<uint8_t>(color_diffuse.r * light),
                            static_cast<uint8_t>(color_diffuse.g * light),
                            static_cast<uint8_t>(color_diffuse.b * light),
                            color_diffuse.a);
                        pixel_in_place.depth = depth;
                        pixel_in_place.color = color;
                    }
                }
            }
        }
    }

    void draw_textured_ortho(const ax::v3& light, const ax::basic_model& model, ax::basic_buffer& buffer)
    {
        VAL& forward = ax::v3(0.0f, 0.0f, 1.0f);
        for (VAR i = 0; i < model.get_faces().size(); i++)
        {
            VAL& face = model.get_face(i);
            VAL& triangle = ax::triangle3(model.get_position(face[0]), model.get_position(face[1]), model.get_position(face[2]));
            VAL& normal = ax::get_normal(triangle);
            VAL not_back_face = normal * forward > 0;
            if (not_back_face)
            {
                draw_textured_ortho(
                    std::abs(normal * light),
                    model.get_surface(),
                    ax::triangle2(model.get_uv(i, 0), model.get_uv(i, 1), model.get_uv(i, 2)),
                    triangle,
                    buffer);
            }
        }
    }
}
