#ifndef AX_BASIC_BUFFER_OPS_HPP
#define AX_BASIC_BUFFER_OPS_HPP

#include "prelude.hpp"
#include "math.hpp"
#include "basic_buffer.hpp"
#include "basic_model.hpp"

namespace ax
{
    void draw_dot(const ax::color& color, int x, int y, ax::basic_buffer& buffer);
    void draw_line(const ax::color& color, int x, int y, int x2, int y2, ax::basic_buffer& buffer);
    void draw_wired_ortho(const ax::color& color, const ax::line2& line, ax::basic_buffer& buffer);
    void draw_wired_ortho(const ax::color& color, const ax::triangle2& triangle, ax::basic_buffer& buffer);
    void draw_wired_ortho(const ax::color& color, const ax::basic_model& model, ax::basic_buffer& buffer);
    void draw_textured_ortho(const ax::v3& light, const ax::basic_surface& surface, const ax::triangle2& uvs, const ax::triangle3& triangle, ax::basic_buffer& buffer);
    void draw_textured_ortho(const ax::v3& light, const ax::basic_model& model, ax::basic_buffer& buffer);
}

#endif
