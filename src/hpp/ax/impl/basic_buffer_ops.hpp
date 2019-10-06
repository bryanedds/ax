#ifndef AX_BASIC_BUFFER_OPS_HPP
#define AX_BASIC_BUFFER_OPS_HPP

#include "prelude.hpp"
#include "math.hpp"
#include "basic_buffer.hpp"
#include "basic_obj_model.hpp"

namespace ax
{
    ax::box2 get_bounds(const ax::triangle2& tri); // TODO: move to math.hpp
    ax::box2 get_intersection(const ax::box2& box, const ax::box2& box2); // TODO: move to math.hpp
    bool get_in_bounds(const ax::v2& point, const ax::triangle2& tri); // TODO: move to math.hpp
    ax::v3 get_barycentric_coords(ax::v2 point, const ax::triangle2& tri);

    void draw_wire(const ax::color& color, int x, int y, int x2, int y2, ax::basic_buffer& buffer);
    void draw_wire_ortho(const ax::color& color, const ax::line2& line, ax::basic_buffer& buffer);
    void draw_wire_ortho(const ax::color& color, const ax::triangle2& tri, ax::basic_buffer& buffer);
    void draw_wire_ortho(const ax::color& color, const ax::basic_obj_model& model, ax::basic_buffer& buffer);
    void draw_filled_ortho(const ax::color& color, const ax::line2& line, ax::basic_buffer& buffer);
    void draw_filled_ortho(const ax::color& color, const ax::triangle2& tri, ax::basic_buffer& buffer);
    void draw_filled_ortho(const ax::color& color, const ax::basic_obj_model& model, ax::basic_buffer& buffer);
}

#endif
