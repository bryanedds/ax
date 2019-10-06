#include "../../hpp/ax/impl/basic_buffer_ops.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <string>
#include <cmath>

namespace ax
{
    ax::box2 get_bounds(const ax::triangle2& tri)
    {
        VAL left = std::min(
            std::get<0>(tri).x,
            std::min(std::get<1>(tri).x, std::get<2>(tri).x));
        VAL right = std::max(
            std::get<0>(tri).x,
            std::max(std::get<1>(tri).x, std::get<2>(tri).x));
        VAL bottom = std::min(
            std::get<0>(tri).y,
            std::min(std::get<1>(tri).y, std::get<2>(tri).y));
        VAL top = std::max(
            std::get<0>(tri).y,
            std::max(std::get<1>(tri).y, std::get<2>(tri).y));
        return
            { {left, bottom},
              {right, top} };
    }

    ax::box2 get_intersection(const ax::box2& box, const ax::box2& box2)
    {
        return
            { {std::min(box.first.x, box2.first.x),
               std::min(box.first.y, box2.first.y)},
              {std::max(box.second.x, box2.second.x),
               std::max(box.second.y, box2.second.y)} };
    }

    ax::v3 get_barycentric_coords(ax::v2 point, const ax::triangle2& tri)
    {
        VAL& a = ax::v3(std::get<2>(tri).x - std::get<0>(tri).x, std::get<1>(tri).x - std::get<0>(tri).x, std::get<0>(tri).x - point.x);
        VAL& b = ax::v3(std::get<2>(tri).y - std::get<0>(tri).y, std::get<1>(tri).y - std::get<0>(tri).y, std::get<0>(tri).y - point.y);
        VAL& u = a ^ b;
        if (std::abs(u.z) < 1.0f)
        {
            // we have a degenerate triangle
            // NOTE: I don't have high confidence in the math here being correct. It was converted from
            // integer math and I suspect I thereby lost something.
            return ax::v3(-1.0f, 1.0f, 1.0f);
        }
        return ax::v3(
            1.0f - (u.x + u.y) / u.z,
            u.y / u.z,
            u.x / u.z);
    }

    bool get_in_bounds(const ax::v2& point, const ax::triangle2& tri)
    {
        VAL& coords = ax::get_barycentric_coords(point, tri);
        return coords.x >= 0 && coords.y >= 0 && coords.z >= 0;
    }

    void draw_dot(const ax::color& color, int x, int y, ax::basic_buffer& buffer)
    {
        buffer.set_point(x, y, color);
    }

    void draw_line(const ax::color& color, int x, int y, int x2, int y2, ax::basic_buffer& buffer)
    {
        // local functions
        struct local
        {
            static void set_point_normal(int x, int y, const ax::color& c, ax::basic_buffer& buffer) { buffer.set_point(x, y, c); }
            static void set_point_swapped(int x, int y, const ax::color& c, ax::basic_buffer& buffer) { buffer.set_point(y, x, c); }
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
            set_point_local(i, j, color, buffer);
            error += error_delta;
            if (error > x_delta)
            {
                j += error_heading;
                error -= x_delta * 2;
            }
        }
    }

    void draw_wire_ortho(const ax::color& color, const ax::line2& line, ax::basic_buffer& buffer)
    {
        VAL& center = ax::v2(
            static_cast<float>(buffer.get_width()),
            static_cast<float>(buffer.get_height())) *
            0.5f;
        VAL x = static_cast<int>((line.first.x + 1.0f) * center.x);
        VAL y = static_cast<int>((line.first.y + 1.0f) * center.y);
        VAL x2 = static_cast<int>((line.second.x + 1.0f) * center.x);
        VAL y2 = static_cast<int>((line.second.y + 1.0f) * center.y);
        ax::draw_line(color, x, y, x2, y2, buffer);
    }

    void draw_wire_ortho(const ax::color& color, const ax::triangle2& tri, ax::basic_buffer& buffer)
    {
        ax::draw_wire_ortho(color, ax::line2(std::get<0>(tri), std::get<1>(tri)), buffer);
        ax::draw_wire_ortho(color, ax::line2(std::get<1>(tri), std::get<2>(tri)), buffer);
        ax::draw_wire_ortho(color, ax::line2(std::get<2>(tri), std::get<0>(tri)), buffer);
    }

    void draw_wire_ortho(const ax::color& color, const ax::basic_obj_model& model, ax::basic_buffer& buffer)
    {
        for (VAR i = 0; i < model.nfaces(); i++)
        {
            VAL& face = model.face(i);
            VAL& a = model.vert(face[0]);
            VAL& b = model.vert(face[1]);
            VAL& c = model.vert(face[2]);
            VAL& tri = ax::triangle2(ax::v2(a.x, a.y), ax::v2(b.x, b.y), ax::v2(c.x, c.y));
            draw_wire_ortho(color, tri, buffer);
        }
    }

    void draw_filled_ortho(const ax::color& color, const ax::line2& line, ax::basic_buffer& buffer)
    {
        ax::draw_wire_ortho(color, line, buffer);
    }

    void draw_filled_ortho(const ax::color& color, const ax::triangle2& tri, ax::basic_buffer& buffer)
    {
        VAL& center_screen = ax::v2(
            static_cast<float>(buffer.get_width()),
            static_cast<float>(buffer.get_height())) *
            0.5f;
        VAL& tri_screen = ax::triangle2(
            (std::get<0>(tri) + ax::v2(1.0f, 1.0f)).SymMul(center_screen),
            (std::get<1>(tri) + ax::v2(1.0f, 1.0f)).SymMul(center_screen),
            (std::get<2>(tri) + ax::v2(1.0f, 1.0f)).SymMul(center_screen));
        VAL& bounds_screen = ax::get_bounds(tri_screen);
        VAL width = static_cast<int>(bounds_screen.second.x);
        VAL height = static_cast<int>(bounds_screen.second.y);
        for (VAR i = static_cast<int>(bounds_screen.first.x); i <= width; ++i)
        {
            for(VAR j = static_cast<int>(bounds_screen.first.y); j <= height; ++j)
            {
                if (ax::get_in_bounds(ax::v2(static_cast<float>(i), static_cast<float>(j)), tri_screen))
                {
                    buffer.set_point(i, j, color);
                }
            }
        }
    }

    void draw_filled_ortho(const ax::color& color, const ax::basic_obj_model& model, ax::basic_buffer& buffer)
    {
        for (VAR i = 0; i < model.nfaces(); i++)
        {
            VAL& face = model.face(i);
            VAL& a = model.vert(face[0]);
            VAL& b = model.vert(face[1]);
            VAL& c = model.vert(face[2]);
            VAL& tri = ax::triangle2(ax::v2(a.x, a.y), ax::v2(b.x, b.y), ax::v2(c.x, c.y));
            VAL& normal = ((b - a) ^ (c - a)).NormalizeSafe();
            VAL& forward = ax::v3(0.0f, 0.0f, 1.0f);
            VAL backface = normal * forward <= 0;
            if (!backface)
            {
                VAL& light = ax::v3(0.0f, 0.0f, -1.0f);
                VAL intensity = normal * light;
                VAL& color2 = ax::color(
                    static_cast<uint8_t>(color.r * intensity),
                    static_cast<uint8_t>(color.g * intensity),
                    static_cast<uint8_t>(color.b * intensity),
                    color.a);
                draw_filled_ortho(color2, tri, buffer);
            }
        }
    }
}
