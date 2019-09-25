#ifndef AX_BUFFER_OPS_HPP
#define AX_BUFFER_OPS_HPP

#include "prelude.hpp"
#include "math.hpp"
#include "basic_obj_model.hpp"
#include "buffer.hpp"

namespace ax
{
    template<typename Buffer>
    void draw_line(int x, int y, int x2, int y2, const ax::color& color, Buffer& buffer)
    {
        // constraint
        CONSTRAIN(Buffer, buffer);

        // local functions
        struct local
        {
            static void set_point_normal(int x, int y, const ax::color& c, Buffer& buffer) { buffer.set_point(x, y, c); }
            static void set_point_swapped(int x, int y, const ax::color& c, Buffer& buffer) { buffer.set_point(y, x, c); }
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

    template<typename Buffer>
    void draw_basic_obj_model_wireframe(const ax::color& color, const ax::basic_obj_model& model, Buffer& buffer)
    {
        // constraint
        CONSTRAIN(Buffer, buffer);

        // draw faces
        VAL center = ax::v2(
            static_cast<float>(buffer.get_width()),
            static_cast<float>(buffer.get_height())) *
            0.5f;
        for (VAR i = 0; i < model.nfaces(); i++)
        {
            // draw face edges
            VAL& face = model.face(i);
            for (VAR j = 0; j < 3; j++)
            {
                ax::v3 v = model.vert(face[j]);
                ax::v3 v2 = model.vert(face[(j + 1) % 3]);
                VAL x = static_cast<int>((v.x + 1.0f) * center.x);
                VAL y = static_cast<int>((v.y + 1.0f) * center.y);
                VAL x2 = static_cast<int>((v2.x + 1.0f) * center.x);
                VAL y2 = static_cast<int>((v2.y + 1.0f) * center.y);
                draw_line(x, y, x2, y2, color, buffer);
            }
        }
    }
}

#endif
