#ifndef AX_BASIC_BUFFER_OPS_HPP
#define AX_BASIC_BUFFER_OPS_HPP

#include "prelude.hpp"
#include "math.hpp"
#include "basic_buffer.hpp"
#include "basic_obj_model.hpp"

namespace ax
{
    void draw_line(int x, int y, int x2, int y2, const ax::color& color, ax::basic_buffer& buffer);
    void draw_wireframe_ortho(const ax::color& color, const ax::basic_obj_model& model, ax::basic_buffer& buffer);
}

#endif
